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

    /* TODO: Replace this function with your solution according to the lab
     * manual. */
    int read_size;
    // char send_msg[6];
    char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char send_msg[3]; // To store two characters and a null terminator
    char srv_reply[150];
    int pack_num = 0;
    double ber; // Bit Rate Error

    printf("---------Beginning subroutine---------\n");
    while(pack_num < 13){ // 13 packets to send
        packet_t *packet = malloc(sizeof(packet_t));
        if(!packet){
            perror("Memmory allocation failed");
            return;
        }
        // Extract 2 characters for the current packet
        send_msg[0] = alphabet[pack_num * 2];
        send_msg[1] = alphabet[pack_num * 2 + 1];
        send_msg[2] = '\0';
        // Build packet
        build_packet(packet, PKT_TYPE_DATA, send_msg, pack_num);
        printf("Enter the bit rate error: ");
        scanf("%lf", &ber);
        // introduce error based on the bit rate error
        introduce_bit_error(send_msg, sizeof(send_msg)/sizeof(send_msg[1]), ber);
        // Notice that if the data is delivered corrupt, it needs to be redelivered

        // send a plain message
        strcpy(send_msg, "Hello");
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
