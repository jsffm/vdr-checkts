#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include "tsstreamerrorcounter.h"

#define TS_READ_BUFFER_SIZE 50

int maxErrors = 0;
const char* recordingDir;

void displayHelp(void)
{
  printf("Usage: vdr-checkts [OPTIONS] RECORDING_DIR\n\n"
         "  -m errors, --max-errors=ERROR Stop scanning if error count reaches ERROR\n"
         "  -h,        --help             print this help and exit\n"
         "\n"
         );
}

int processCliArguments(int argc, char *argv[])
{
  static struct option long_options[] = {
      { "max-errors", required_argument, NULL, 'm' },
      { "help",       no_argument,       NULL, 'h' },
      { NULL,         no_argument,       NULL,  0  }
    };

  int c;
  while ((c = getopt_long(argc, argv, "m:h", long_options, NULL)) != -1) {
        switch (c) {
          case 'm': maxErrors = strtol(optarg, (char**) NULL, 10);
                    if (errno != 0 || maxErrors <= 0) {
                       fprintf(stderr, "Invalid max error argument: %s\n", optarg);
                       return -2;
                       }
                    break;
          case 'h': displayHelp();
                    return -1;
          }
        }
  if (optind != argc - 1)
  {
    displayHelp();
    return -1;
  }
  recordingDir = argv[optind];
  return 0;
}

int checkRecording(void)
{
  int fileNumber = 1;
  cTsStreamErrorCounter tsStreamErrorCounter;
  while(!maxErrors || tsStreamErrorCounter.ErrorCount() < maxErrors) {
    char fileName[PATH_MAX];

    sprintf(fileName, "%s/%.5d.ts", recordingDir, fileNumber);

    FILE *file = fopen64(fileName, "rb");
    if (file) {
      unsigned char buffer[TS_READ_BUFFER_SIZE * TS_SIZE];
      int read;
      while (true) {
        read = fread(buffer, TS_SIZE, TS_READ_BUFFER_SIZE, file);
        if (read <= 0) break;
        tsStreamErrorCounter.CheckTsPackets(buffer, read * TS_SIZE);
        if (maxErrors && tsStreamErrorCounter.ErrorCount() >= maxErrors)
          break;
        }
      }
    else
      break;
    fclose(file);
    fileNumber++;
    }

  printf("Errors: %d\n", tsStreamErrorCounter.ErrorCount());
  return tsStreamErrorCounter.ErrorCount();
}

int main(int argc, char *argv[])
{
  if (int res = processCliArguments(argc, argv) != 0) return res;

  if (checkRecording() == 0)
    return 0;
  else
    return 1;
}
