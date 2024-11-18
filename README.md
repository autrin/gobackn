# CPRE 4890 Go-Back-N Template

## Getting Started

### Compiling and Running

A makefile is included which compiles both the `sender` and `receiver` binaries. You may then run the receiver (server) and sender (client). The below example uses the port 5000, though you may use any un-privileged port.

```shell
make
./receiver 5000
./sender 127.0.0.1 5000 0.01
```

If successful, you should see the sender transmit a mostly-garbage packet. The receiver will respond with a NAK as the sent packet's checksum is invalid.

### Developing Your Solution

It's recommended you first read over each of the files provided, especially the header files `introduceerror.h`, `ccitt16.h`, and `utilities.h`.

Once ready, you may begin developing your solution in `sender_subroutine.c`.

1. ccitt16.h: a header file for calcuating CCITT16 checksums.
2. ccitt16.o: the object file for ccitt16.o, already compiled for the co2061 machines.
3. introduceerror.c/.h:
