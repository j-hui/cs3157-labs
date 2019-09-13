#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "sortArray.h"
#include "printArray.h"

void randoArray(int *a, size_t size) {
	srandom(time(NULL));
	uint u;
	for(u = 0; u < size; u++)
		a[u] = random() % 100;
}

int main() {
	int aSize;
	scanf("%d", &aSize); // Assumed to be positive integer

	int *a1 = malloc(aSize * sizeof(int));
	randoArray(a1, aSize); // Randomly initialised

	int *a2 = malloc(aSize * sizeof(int));
	memcpy(a2, a1, aSize * sizeof(int));
	sort_integer_array(a2, a2 + aSize, 1); // Ascending sort
	
	int *a3 = malloc(aSize * sizeof(int));
	memcpy(a3, a1, aSize * sizeof(int));
	sort_integer_array(a3, a3 + aSize, 0); // Descending sort

	printIntArray(a1, a1 + aSize);
	printIntArray(a2, a2 + aSize);
	printIntArray(a3, a3 + aSize);

	free(a1);
	free(a2);
	free(a3);
	
	return 0;
}
