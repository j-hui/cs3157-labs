CC = gcc
INCLUDES =
LDLIBS =
CFLAGS = -Wall -g $(INCLUDES) -pthread
LDFLAGS = $(INCLUDES) -pthread

HEADERS = printArray.h sortArray.h
DEPS = isort.o printArray.o sortArray.o

isort: $(DEPS)

isort.o: isort.c $(HEADERS)

printArray.o: printArray.c printArray.h

sortArray.o: sortArray.c sortArray.h

.PHONY: all
all: clean isort

.PHONY: clean
clean:
	rm -f *.o isort a.out
