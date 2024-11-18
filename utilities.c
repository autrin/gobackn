#include "utilities.h"
#include "ccitt16.h"
#include <stdint.h>
#include <string.h>

void build_packet(packet_t *packet, unsigned char type, char data[], unsigned char num)
{	
	int i, j;
    unsigned char packet_raw[PKT_SIZE];
	//Set type and sequence number
	packet_raw[0] = type;
	packet_raw[1] = num;
	
	//set data field
	for(i = PKT_DATA_OFFSET, j = 0; i < PKT_DATA_SIZE + PKT_DATA_OFFSET; ++i, ++j)
	{
		packet_raw[i] = data[j]; //packet[2] = data[0]; packet[3] = data[1]
	}
	
	//union to split the CRC output into two bytes.
	//Suggestion taken from GedasL at http://www.avrfreaks.net/forum/c-programming-how-split-int16-bits-2-char8bit
	union short_split
	{
		short int CRC;
		unsigned char bytes[2];
	}splitter;
	
	splitter.CRC = calculate_CCITT16(packet_raw, PKT_SIZE - 2, GENERATE_CRC);
	//set the two CRC bytes
	packet_raw[PKT_SIZE - 1] = splitter.bytes[0];
	packet_raw[PKT_SIZE - 2] = splitter.bytes[1];

    memcpy(packet, packet_raw, PKT_SIZE);
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
