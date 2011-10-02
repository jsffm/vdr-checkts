#ifndef __TSCHECKER_H
#define __TSCHECKER_H

#define TS_SYNC_BYTE          0x47
#define TS_SIZE               188

// TS stream error counter:
// Count (continuity) errors in a TS packet stream

#define MAX_PIDS 20
typedef unsigned char uchar;

class cTsStreamErrorCounter {
private:
  int errorCount;
  int pids[MAX_PIDS];
  uchar counters[MAX_PIDS];
public:
  cTsStreamErrorCounter(void);
  void CheckTsPackets(uchar* Data, int Length);
      ///< Check for errors in the TS packets pointed to by Data. Length is the
      ///< number of bytes Data points to, and must be a multiple of 188.
  int ErrorCount(void) const { return errorCount; };
      ///< Return the number of errors found in the analyzed TS packet stream.
      ///< Right now, only continuity errors are reported.
private:
  void CheckTsPacketContinuity(int pid, int counter);
  };


#endif // __TSCHECKER_H
