### To design and develop a sender function (sender_subroutine.c) to implement the Go-Back-N ARQ protocol. 
##### Note that this Go-Back-N ARQ protocol is a revised version, and it uses both ACK and NAK packets to simplify the implementation. So it does not implement the timeout mechanism.

#### Packet Type:
#### - 1: Data Packet (sent from sender to receiver)
#### - 2: Acknowledgement Packet (ACK) (sent from receiver to sender)
#### - 3: Negative Acknowledgment Packet (NAK) (sent from receiver to sender)
#### Packet Number:
#### - Starts from 0 and increments sequentially to 12
#### Data:
#### - Two alphabet characters (sent from sender to receiver)
#### - No data is sent from receiver to sender
#### CRC:
#### - CRC generated for this entire packet, including Packet Type, Packet Number, and Data fields

#### CRC generation and error detection is provided in ccitt16.o.
### Sender:
#### - Displays the built packets (print_packet()).
#### - Applys the “IntroduceError.c” routine to the entire packet. Passes the BER value to the program as an argument in the command line.
#### - Implements the Go-Back-N ARQ protocol with a send window of size N = 3:
#### - The sender initially sends all packets within its send window. Any time a packet is sent, the packet is printed accordingly.
#### - When the sender gets an ACK from the receiver, it (1) displays an indication of the received ACK, (2) adjusts the send window and (3) sends any new packets in its send window.
#### - When the sender gets a NAK from the receiver, it (1) displays an indication of the received NAK and (2) retransmits all the packets in the send window.

### Receiver: 
#### Accept data packets from the sender.
#### Run the CRC:
#### - If the packet is received error free and in sequence, it displays the packet content and sequence number, then sends back an ACK.
#### - If the packet is received error free but out of sequence, it does not display the packet content but displays the sequence number, then sends back an ACK.
#### - If the packet is received in error, it does not display the packet content but displays the sequence number if possible, then sends back a NAK.
#### - ACK and NAK packets will never be corrupted.

### Compiling and Running

A makefile is included which compiles both the `sender` and `receiver` binaries. You may then run the receiver and sender. The below example uses the port 5000, though you may use any un-privileged port.

```shell
make
./receiver 5000
./sender 127.0.0.1 5000 0.01
```

## Helpful Notes

- `sender.o` and `receiver.o` already set up the TCP connection between the sender and receiver.
