#include <omp.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "sorting.h"

struct bucket {
	int n_elem;
	int index; 
	int start;
};

int cmpfunc (const void * a, const void * b)
{
 return ( *(int*)a - *(int*)b );
}

void sequentialBucketSort(int dim, int n_buckets) {

	int *A, *B, *temp;
	int i, w, j, limit;
	struct bucket* buckets; 
	double t1; 
	float total; 

	limit = 100000;
	w = (int)limit/n_buckets;
	A = (int *) malloc(sizeof(int)*dim);
	B = (int *) malloc(sizeof(int)*dim);
	
	buckets = (struct bucket *) calloc(n_buckets, sizeof(struct bucket));

	for(i=0;i<dim;i++) {
		A[i] = random() % limit;
	}
	

	t1 = omp_get_wtime();
	
	for (i=0; i<dim;i++){
		j = A[i]/w;
		if (j>n_buckets-1)
				j = n_buckets-1;
		buckets[j].n_elem++;
	}
	
	for (i=1; i<n_buckets;i++){
		buckets[i].index = buckets[i-1].index + buckets[i-1].n_elem;
		buckets[i].start = buckets[i-1].start + buckets[i-1].n_elem;
	}
	int b_index;
	for (i=0; i<dim;i++){
		j = A[i]/w;
		if (j > n_buckets -1)
				j = n_buckets-1;
		b_index = buckets[j].index++;
		B[b_index] = A[i];
	}	
	
	for(i=0; i<n_buckets; i++){
		qsort(B+buckets[i].start, buckets[i].n_elem, sizeof(int), cmpfunc);
		//bubbleSort(B + buckets[i].start, buckets[i].n_elem);
		//selectionSort(B + buckets[i].start, buckets[i].n_elem);
		//insertionSort(B + buckets[i].start, buckets[i].n_elem);
		//heapSort(B + buckets[i].start, buckets[i].n_elem);

	
	}
	temp = A;
	A = B;
	B = temp;
	total = omp_get_wtime() - t1;

	printf("Given size %d elements took %f seconds to be sorted\n", dim,total);

	int sorted = 1;
	for(i=0;i<dim-1;i++) {
		if (A[i] > A[i+1])
			sorted = 0;
  	}
  	if (!sorted)
		printf("The data is not sorted!!!\n");
}

