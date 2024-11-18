#ifndef INTRODUCE_ERROR
#define INTRODUCE_ERROR

#include <stdlib.h>

/* introduce_bit_error
 * Given a byte-array, data, of length len, iterates over each byte and 
 * introduces bit-error based on the bit error rate (BER), p
 
 * Warning: 
 *   Modifies data in place! Make sure to have a copy of your
 *   if you need it later  */
void introduce_bit_error(char *data, size_t len, double p);

#endif