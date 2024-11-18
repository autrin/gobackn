all: sender receiver

clean:
	rm receiver_subroutine.o utilities.o sender_subroutine.o introduceerror.o sender receiver

receiver_subroutine.o: receiver_subroutine.c
	cc -c receiver_subroutine.c -o receiver_subroutine.o

utilities.o: utilities.c
	cc -c utilities.c -o utilities.o

receiver: receiver.o receiver_subroutine.o utilities.o ccitt16.o
	cc -o receiver receiver.o receiver_subroutine.o utilities.o ccitt16.o

sender_subroutine.o: sender_subroutine.c
	cc -c sender_subroutine.c -o sender_subroutine.o

introduceerror.o:
	cc -c introduceerror.c -o introduceerror.o

sender: sender.o sender_subroutine.o utilities.o ccitt16.o introduceerror.o
	cc -o sender sender.o sender_subroutine.o utilities.o ccitt16.o introduceerror.o