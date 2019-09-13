#include "sortArray.h"
#include "printArray.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#define pair_bool(a, b) ((a) << 1 | (b))

int ascfunc(const void *a, const void *b) {
	   return(*(int*)a - *(int*)b);
}

int dscfunc(const void *a, const void *b) {
	   return(*(int*)b - *(int*)a);
}

struct array {
	int *arr;
	size_t size;
	int asc;
};

void *msort(void *arg)
{
	struct array *a;
	int *tmp, i, i1, i2;
	pthread_t tid1, tid2;
	
	a = arg;

	if (a->size <= 1)
		return NULL;

	struct array a1 = {a->arr, a->size / 2, a->asc};
	struct array a2 = {a->arr + a1.size, a->size - a1.size, a->asc};

	pthread_create(&tid1, NULL, msort, &a1);
	pthread_create(&tid2, NULL, msort, &a2);

	tmp = malloc(sizeof(*tmp) * a->size);

	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);

	for (i = 0, i1 = 0, i2 = 0; i < a->size; i++)
		switch pair_bool(i1 < a1.size, i2 < a2.size) {
		case pair_bool(1, 1):
			if ((a1.arr[i1] < a2.arr[i2]) == a->asc)
				tmp[i] = a1.arr[i1++];
			else
				tmp[i] = a2.arr[i2++];
			break;
		case pair_bool(1, 0):
			tmp[i] = a1.arr[i1++];
			break;
		case pair_bool(0, 1):
			tmp[i] = a2.arr[i2++];
			break;
		case pair_bool(0, 0):
			goto complete;
		}
complete:
	memcpy(a->arr, tmp, a->size * sizeof(*a->arr));
	free(tmp);

	return NULL;
}

void sort_integer_array(int *begin, int *end, int ascending) {
	struct array a = {begin, end - begin, ascending};

	msort(&a);
}
