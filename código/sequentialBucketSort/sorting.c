#include <omp.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "sorting.h"

void swap(int *xp, int *yp)
{
	int temp = *xp;
	*xp = *yp;
	*yp = temp;
}

void bubbleSort(int arr[], int n)
{
	int i, j;
	for (i = 0; i < n - 1; i++)

		for (j = 0; j < n - i - 1; j++)
			if (arr[j] > arr[j + 1])
				swap(&arr[j], &arr[j + 1]);
}

void swap2(int *a, int *b)
{
	int tmp = *a;
	*a = *b;
	*b = tmp;
}

void heapify(int arr[], int n, int i)
{
	int max = i; 
	int leftChild = 2 * i + 1;
	int rightChild = 2 * i + 2;

	if (leftChild < n && arr[leftChild] > arr[max])
		max = leftChild;

	if (rightChild < n && arr[rightChild] > arr[max])
		max = rightChild;

	if (max != i)
	{
		swap2(&arr[i], &arr[max]);
		heapify(arr, n, max);
	}
}

void heapSort(int arr[], int n)
{
	int i;

	for (i = n / 2 - 1; i >= 0; i--)
		heapify(arr, n, i);

	for (i = n - 1; i >= 0; i--)
	{
		swap(&arr[0], &arr[i]); 

		heapify(arr, i, 0); 
	}
}

void insertionSort(int arr[], int n)
{
	int i, key, j;
	for (i = 1; i < n; i++)
	{
		key = arr[i];
		j = i - 1;

		while (j >= 0 && arr[j] > key)
		{
			arr[j + 1] = arr[j];
			j = j - 1;
		}
		arr[j + 1] = key;
	}
}

void selectionSort(int array[], int n)
{
	int i, j, min_element;
	for (i = 0; i < n - 1; i++)
	{
		min_element = i;
		for (j = i + 1; j < n; j++)
			if (array[j] < array[min_element])
				min_element = j;
		swap2(&array[min_element], &array[i]);
	}
}
