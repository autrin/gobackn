#include "utilities.h"
#include "ccitt16.h"
#include <stdint.h>
#include <string.h>

void build_packet(packet_t *packet, uint8_t type, char data[], uint8_t num) {
    packet->type = type;
    packet->sequence_number = num;

    memcpy(packet->data, data, PKT_DATA_SIZE);

    // union to split the CRC output into two bytes.
    // Suggestion taken from GedasL at
    // http://www.avrfreaks.net/forum/c-programming-how-split-int16-bits-2-char8bit
    union short_split {
        short int CRC;
        unsigned char bytes[2];
    } splitter;

    splitter.CRC = calculate_CCITT16((unsigned char*)&packet, PKT_SIZE - 2, GENERATE_CRC);
    memcpy(packet->crc_sum, splitter.bytes, 2);
}

void print_packet(packet_t *packet) {
    // print packet type
    switch (packet->type) {
    case PKT_TYPE_DATA:
        printf("[DAT");
        break;
    case PKT_TYPE_ACK:
        printf("[ACK");
        break;
    case PKT_TYPE_NAK:
        printf("[NAK");
        break;
    default:
        printf("[???");
    }

    // print sequence number
    printf("|%d|", packet->sequence_number);

    // print packet data
    for (int i = 0; i < PKT_DATA_SIZE; i++) {
        if (packet->data[i] == '\0')
            printf(" ");
        else
            printf("%c", packet->data[i]);
    }

    // print CRC
    printf("|%x%x]\n", packet->crc_sum[0], packet->crc_sum[1]);
}
