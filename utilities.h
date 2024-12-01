#ifndef UTILITIES
#define UTILITIES

#include <stdint.h>

/*  Packet structure
    ----------------

         0        1        2       3       4       5
        +--------+--------+-------+-------+-------+-------+
        | Packet | Seq No | Data  | Data  | CRC   | CRC   |
        |  Type  |        |       |       |       |       |
        +--------+--------+-------+-------+-------+-------+

*/

#define PKT_SIZE 6
#define PKT_DATA_SIZE 2

#define PKT_TYPE_OFFSET 0
#define PKT_SEQNUM_OFFSET 1
#define PKT_DATA_OFFSET 2
#define PKT_CRC_OFFSET PKT_DATA_OFFSET + PKT_DATA_SIZE

#define PKT_TYPE_DATA 1
#define PKT_TYPE_ACK 2
#define PKT_TYPE_NAK 3
#define WINDOW 3 // Window size of Go-Back-N ARQ
/* Struct with defined the packet format.
 * N.B., you may cast this to and from a char[] or uint8_t[]  */
typedef struct __attribute__((packed)) packet_t {
    /* the type of packet, PKT_TYPE_{DATA, ACK, NAK} as defined in macros above
     */
    uint8_t type;

    /* the sequence number of this packet */
    uint8_t sequence_number;

    /* payload */
    uint8_t data[PKT_DATA_SIZE];

    /* crc checksum*/
    uint8_t crc_sum[2];

} packet_t;

/**
 * Builds a packet by editing the empty packet_t.
 *   The packet array is expected to be of defined size PACKET_SIZE.
 *   The type field is expected to be one of the defined packet types.
 *   The data array is expected to be the size of the defined DATA_LENGTH.
 *   num is the packet number of the current packet.
 */
void build_packet(packet_t *packet, unsigned char type, char data[], unsigned char num);
// void build_packet(packet_t *packet, uint8_t type, char data[], uint8_t num);

/**
 * Prints the given packet in a neat one-line format
 */
void print_packet(packet_t *packet);

#endif
