#include <stdlib.h>

static void introduce_bit_error_in_byte(char *data, double p) {
	char c = 0x01;
	for (int i = 0; i < 8; i++) {
      if ((double)rand() / RAND_MAX <= p) {
        *data ^= c;
      }
      c = c << 1;
    }
}

void introduce_bit_error(char *data, size_t len, double p) {
  for (int i = 0; i < len; i++) {
    introduce_bit_error_in_byte(&data[i], p);
  }
}

