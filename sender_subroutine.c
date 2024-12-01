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
    int pack_num = 0;     // Packet sequence number
    int base = 0;         // Go-Back-N ARQ base sequence
    int next_seq_num = 0; // Next sequence number
    char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char srv_reply[150];            // Buffer for receiver replies
    packet_t *window[WINDOW] = {0}; // Store packets in the send window
    printf("---------Beginning subroutine---------\n");

    // Input BER (bit error rate)
    printf("Enter the bit rate error: ");
    scanf("%lf", &ber);
    if (ber < 0.0 || ber > 1.0) {
        fprintf(stderr, "Error: Bit error rate must be between 0 and 1.\n");
        return;
    }

    while (base < 13) { // Loop until all packets are acknowledged
        // Send packets in the current window
        while (next_seq_num < base + WINDOW && next_seq_num < 13) {
            packet_t *packet = malloc(sizeof(packet_t));
            if (!packet) {
                perror("Memory allocation failed");
                return;
            }

            // Build the packet
            char send_msg[3] = {alphabet[next_seq_num * 2],
                                alphabet[next_seq_num * 2 + 1], '\0'};
            build_packet(packet, PKT_TYPE_DATA, send_msg, next_seq_num);

            // Generate CRC
            short int crc = calculate_CCITT16((unsigned char *)packet,
                                              PKT_SIZE - 2, GENERATE_CRC);
            packet->crc_sum[0] = (crc >> 8) & 0xFF; // High byte
            packet->crc_sum[1] = crc & 0xFF;        // Low byte

            // Apply BER to the packet
            printf("Applying BER=%.3f to packet %d\n", ber, next_seq_num);
            introduce_bit_error((char *)packet, PKT_SIZE, ber);

            // Send the packet
            if (send(sockfd, (char *)packet, sizeof(packet_t), 0) < 0) {
                perror("Send failed");
                free(packet);
                return;
            }

            // Print the sent packet
            printf("Sent packet: ");
            print_packet(packet);

            // Store the packet in the send window for potential retransmission
            window[next_seq_num % WINDOW] = packet;
            next_seq_num++;
        }
        printf("Sliding window: base=%d, next_seq_num=%d\n", base,
               next_seq_num);

        // Wait for acknowledgment
        if (recv(sockfd, srv_reply, PKT_SIZE, 0) < 0) {
            perror("Recv failed");
            return;
        } else {
            packet_t *response = (packet_t *)srv_reply;

            // Process ACK
            if (response->type == PKT_TYPE_ACK) {
            printf("Received ACK for packet %d\n", response->sequence_number);
            
            if (response->sequence_number >= base) {
                // Slide the window forward
                while (base <= response->sequence_number) {
                    // Do NOT free packets until they are completely out of the window
                    // Only set them to NULL for better clarity
                    if (window[base % WINDOW]) {
                        free(window[base % WINDOW]);
                        window[base % WINDOW] = NULL;
                    }
                    base++;
                }
            }
        }

            // Process NAK
            else if (response->type == PKT_TYPE_NAK) {
                printf("Received NAK for packet %d, retransmitting window...\n",
                    response->sequence_number);
                // Start retransmitting from the base
                for (int i = base; i < next_seq_num; i++) {
                    if (window[i % WINDOW]) {
                        // Resend the packet
                        if (send(sockfd, (char *)window[i % WINDOW], sizeof(packet_t), 0) < 0) {
                            perror("Resend failed");
                            return;
                        }
                        // Log the resent packet
                        printf("Retransmitted packet: ");
                        print_packet(window[i % WINDOW]);
                    }
                }
            }

        }
    }

    // Cleanup: free any remaining allocated packets
    for (int i = 0; i < WINDOW; i++) {
        if (window[i]) {
            free(window[i]);
        }
    }
    printf("All packets sent and acknowledged successfully.\n");
}
