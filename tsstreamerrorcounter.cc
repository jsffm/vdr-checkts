#include "string.h"
#include <stdio.h>
#include "tsstreamerrorcounter.h"

// --- cTsStreamErrorCounter -------------------------------------------------

cTsStreamErrorCounter::cTsStreamErrorCounter(void)
{
  errorCount = 0;
  for(int i=0; i < MAX_PIDS; i++) pids[i] = -1;
  memset(counters, 0, sizeof(counters));
}

void cTsStreamErrorCounter::CheckTsPackets(uchar* Data, int Length)
{
  while (Length >= TS_SIZE) {
        int pid = ((Data[1] & 0x1F) << 8) + Data[2];
        char ctr = (Data[3] & 0x0F);
        char adaption = (Data[3] & 0x30) >> 4;

        if (adaption != 2)
           CheckTsPacketContinuity(pid, ctr);

        Length -= TS_SIZE;
        Data += TS_SIZE;
       }
}

void cTsStreamErrorCounter::CheckTsPacketContinuity(int pid, int counter)
{
  for (int i=0; i < MAX_PIDS; i++) {
      if (pids[i] == -1) {
         pids[i] = pid;
         counters[i] = counter;
         break;
         }
      if (pids[i] == pid) {
         if (((counter - counters[i]) & 0x0F) != 1)
            errorCount++;
         counters[i] = counter;
         break;
         }
      }
}
