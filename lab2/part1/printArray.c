#include <stdio.h>

void printIntArray(int *begin, int *end) {
	printf("[");

	int *p = begin;
	while(p < end) {
		printf("%d", *p);
		if(++p < end) {printf(", ");}
	}

	printf("]\n");
}
