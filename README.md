# CPRE 4890 Go-Back-N Template

## Getting Started

### Compiling and Running

A makefile is included which compiles both the `sender` and `receiver` binaries. You may then run the receiver and sender. The below example uses the port 5000, though you may use any un-privileged port.

```shell
make
./receiver 5000
./sender 127.0.0.1 5000 0.01
```

If successful, you should see the sender transmit a mostly-garbage packet. The receiver will respond with a NAK as the sent packet's checksum is invalid.

### Developing Your Solution

It's recommended you first read over each of the files provided, especially the header files `introduceerror.h`, `ccitt16.h`, and `utilities.h`.

Once ready, you may begin developing your solution in `sender_subroutine.c`.

## Helpful Notes

- When receiving a `NAK`, don't think too much into the received packet printed out by `receiver_subroutine.c`. After all, it _is_ a corrupted packet.
- `sender.o` and `receiver.o` already set up the connection between the sender and receiver.
