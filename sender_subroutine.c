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
    char send_msg[6];
    char srv_reply[150];

    printf("---------Beginning subroutine---------\n");

    // send a plain message
    strcpy(send_msg, "Hello");
    if (send(sockfd, send_msg, sizeof(send_msg), 0) < 0)
        perror("Send failed");
    printf("Sent packet: ");
    print_packet((packet_t *)&send_msg);

    /* Receive a reply from the server
     * Note:
     *   Sockets, and recv, do not keep messages separated. For example, If you
     *   receive two 16-byte messages between receives, your next receive will
     *   be 32 bytes. Similarly, if there's 200 bytes to receive but you only
     *   read 149, there will be messages left in your socket's buffer. */
    if ((read_size = recv(sockfd, srv_reply, 6, 0)) < 0) {
        perror("recv failed");
    } else {
        printf("Received packet: ");
        print_packet((packet_t *)&srv_reply);
    }
}
