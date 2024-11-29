#include <stdio.h>
#include <assert.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define GENERATE_CRC 1 // Returns the checksum of cData[] with length iLen as a short int
#define CHECK_CRC 2 // Uses the last two bytes of cData[] as CRC check bits to check cData[]; returns either 0 or 1:
                    // CRC_CHECK_SUCCESSFUL or CRC_CHECK_FAILURE
#define CRC_CHECK_SUCCESSFUL 0
#define CRC_CHECK_FAILURE 1

#define CCITT_POLYNOMS 0x8408
#define FEEDBACK(a) (a = (a ^ CCITT_POLYNOMS))

  short int calculate_CCITT16(unsigned char cData[], unsigned int iLen, unsigned int iAction);
  // iAction is defined as either GENERATE_CRC or CHECK_CRC
#ifdef __cplusplus
}
#endif
