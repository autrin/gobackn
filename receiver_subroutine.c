#include "ccitt16.h"
#include "utilities.h"
#include <stdio.h>
#include <sys/socket.h>

void secondary(int client_sock) {
    int read_size;
    unsigned char read_msg[150];
    unsigned char expected = 0;

    printf("\n---------Beginning subroutine---------\n");

    while ((read_size = recv(client_sock, read_msg, PKT_SIZE, 0)) > 0) {

        // check the CRC
        if (calculate_CCITT16(read_msg, PKT_SIZE, CHECK_CRC) ==
            CRC_CHECK_SUCCESSFUL) {

            if (read_msg[1] == expected) {
                // intact and in-order, send next ACK
                print_packet((packet_t *)&read_msg);
                expected++;
                packet_t ack;
                char pkt_data[PKT_DATA_SIZE] = {'\0', '\0'};
                build_packet(&ack, PKT_TYPE_ACK, pkt_data, expected);

                printf("\t-> Sending: ", expected);
                print_packet(&ack);
                if (send(client_sock, (char *)&ack, sizeof(ack), 0) < 0)
                    perror("Send failed\n");

            } else {
                // intact but out-of-order, send DUP ACK
                printf("Received packet %d out of order\n", read_msg[1]);
                packet_t ack;
                char pkt_data[PKT_DATA_SIZE] = {'\0', '\0'};
                build_packet(&ack, PKT_TYPE_ACK, pkt_data, expected);

                printf("\t-> Sending: ", expected);
                print_packet(&ack);
                if (send(client_sock, (char *)&ack, sizeof(ack), 0) < 0)
                    perror("Send failed");
            }

        } else {
            // not intact, NAK
            printf("Received corrupted packet: ");
            print_packet((packet_t *)&read_msg);
            packet_t nak;
            char pkt_data[] = "\0\0";
            build_packet(&nak, PKT_TYPE_NAK, pkt_data, expected);
            printf("\t-> Sending: ", expected);
            print_packet(&nak);
            if (send(client_sock, (char *)&nak, sizeof(nak), 0) < 0)
                perror("Send failed\n");
        }
    }

    if (read_size == 0) {
        puts("Client disconnected");
        fflush(stdout);
    } else if (read_size == -1) {
        perror("recv failed");
    }
}
