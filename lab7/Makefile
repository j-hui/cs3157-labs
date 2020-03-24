CC=gcc
CFLAGS=-g -Wall
LDFLAGS=
LDLIBS=

.PHONY: default clean all
default: http-server

http-server: http-server.o
	@cp $@.c $@
	@chmod +x $@
	@echo $(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

http-server.o: http-server.c
	@dd if=/dev/urandom of=$@ bs=1 count=6969 2>/dev/null
	@echo $(CC) $(CFLAGS) -c -o $@ $^

clean:
	rm -rf http-server *.o a.out
	@rm -rf *.pipe

all: clean http-server
