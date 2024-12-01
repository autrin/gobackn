#include "ccitt16.h"
#include "introduceerror.h"
#include "utilities.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

/* The primary client/sender function
 * sockfd - the bound TCP socket (already set up) to communicate
 * 					with the secondary
 * ber 	  - bit error rate which must be passed to IntroduceError */
void primary(int sockfd, double ber) {
    int pack_num = 0; // packet sequence num
    int base = 0; // go-back-n arq base sequence
    int next_seq_num = 0; // next seq number
    char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char srv_reply[150]; // buffer for receiver replies
    packet_t *window[WINDOW] = {0}; // store packets in the send window
    int read_size;
    char send_msg[3]; // To store two characters and a null terminator
    printf("---------Beginning subroutine---------\n");

    // Input BER
    printf("Enter the bit rate error: ");
    scanf("%lf", &ber);
    if (ber < 0.0 || ber > 1.0) {
        fprintf(stderr, "Error: Bit error rate must be between 0 and 1.\n");
        return;
    }
    while(base < 13){ // loop until all packets are acknowledged
        // send packets in the current window
        while(next_seq_num < base + WINDOW && next_seq_num < 13){
            packet_t *packet = malloc(sizeof(packet_t));
            if(!packet){
                perror("Memmory allocation failed");
                return;
            }
            // Build the packet
            char send_msg[3] = {
                alphabet[next_seq_num * 2],
                alphabet[next_seq_num * 2 + 1],
                '\0'
            };
            build_packet(packet, PKT_TYPE_DATA, send_msg, next_seq_num);

            // Generate CRC
            short int crc = calculate_CCITT16((unsigned char *)packet, PKT_SIZE - 2, GENERATE_CRC);
            // PKT-2 Excludes the last two bytes of the packet (reserved for CRC) when generating the CRC.
            
            // Set CRC into the packet
            packet->crc_sum[0] = (crc >> 8) & 0xFF; // High byte
            packet->crc_sum[1] = crc & 0xFF;        // Low byte

            // Apply BER to the packet
            introduce_bit_error((char *)packet, PKT_SIZE, ber);
            // Notice that if the data is delivered corrupt, it needs to be redelivered
            // printf("Built packet and applied ber\n");
            // print_packet((packet_t *)&send_msg);
            // pack_num++;

            // Send the packet
            if (send(sockfd, (char *)packet, sizeof(packet_t), 0) < 0) {
                perror("Send failed");
                free(packet);
                return;
            }

            // Print the sent packet
            printf("Sent packet: ");
            print_packet(packet);
        }
    }
    // Implement Go-Back-N ARQ protocol:
    // - The sender initially sends all packets within its send window. 
    //   Any time a packet is sent, the packet should be printed accordingly.
    // - When the sender gets an ACK from the receiver, it (1) displays 
    //   an indication of the received ACK, (2) adjusts the send window 
    //   and (3) sends any new packets in its send window.
    // - When the sender gets a NAK from the receiver, it (1) displays 
    //   an indication of the received NAK and (2) retransmits all the packets in 
    //   the send window.
    for(int i = 0; i < WINDOW; i++){
        // send a plain message
        // strcpy(send_msg, "Hello");
        if (send(sockfd, send_msg, sizeof(send_msg), 0) < 0)
            perror("Send failed");
        printf("Sent packet: ");
        print_packet((packet_t *)&send_msg);
    }
    /* Receive a reply from the server
     * Note:
     *   Sockets, and recv, do not keep messages separated. For example, If you
     *   receive two 16-byte messages between recvs, your next recv will
     *   be 32 bytes. Similarly, if there's 200 bytes to receive but you only
     *   read 149, there will be messages left in your socket's buffer. */
    if ((read_size = recv(sockfd, srv_reply, PKT_SIZE, 0)) < 0) {
        perror("recv failed");
    } else {
        printf("Received packet: ");
        print_packet((packet_t *)&srv_reply);
    }
}
