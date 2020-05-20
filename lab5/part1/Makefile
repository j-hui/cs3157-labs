CC=gcc
CFLAGS=-g -Wall
LDLIBS=
LDFLAGS=

default: mdb-lookup-server-nc-1 mdb-lookup-server-nc-2

mdb-lookup-server-nc-1: mdb-lookup-server-nc-1.o
mdb-lookup-server-nc-1.o: mdb-lookup-server-nc-1.c

mdb-lookup-server-nc-2: mdb-lookup-server-nc-2.o mdb-lookup-server-nc-1
	@cp $@.c $@
	@chmod +x $@
	@echo "$(CC)" "$(LDFLAGS)" "$^" "$(LDLIBS)" "-o" "$@"

mdb-lookup-server-nc-2.o: mdb-lookup-server-nc-2.c
	@$(CC) $(CFLAGS) -c -o $@ mdb-lookup-server-nc-1.c
	@echo "$(CC)" "$(CFLAGS)" "-c" "-o" "$@" "$^"

clean:
	rm -rf mdb-lookup-server-nc-1 mdb-lookup-server-nc-2 *.o mypipe-*

.PHONY: clean default
