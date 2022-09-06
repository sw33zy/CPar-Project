#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <omp.h>
#include "papi.h"

#include "bucketSortPar.h"

static int verify_command_line (int argc, char *argv[], int *m_size, int *version);
static void print_usage (char *msg);



int main (int argc, char *argv[]) {
    int dim;	
    int n_buckets;
    
    if (!verify_command_line (argc, argv, &dim, &n_buckets)) {
        return 0;
    }
        
    bucketSortPar(dim, n_buckets);
        
     
    return 0;
}

int verify_command_line (int argc, char *argv[], int *dim, int *buckets) {
    int val;
    
    if (argc!=3) {
        print_usage ((char *)"Exactly 2 arguments are required!");
        return 0;
    }
    
    val = atoi (argv[1]);
    
    if (val <= 0) {
        print_usage ((char *)"The array size is the number of elements inside it and must be a positive integer!");
        return 0;
    }
    else {
        *dim = val;
    }
    
    val = atoi (argv[2]);
    
    if (val <= 0){
        print_usage ((char *)"The number of buckets must be a positive integer!");
        return 0;
    }
    else {
        *buckets = val;
    }
    
    return 1;
}

void print_usage (char *msg) {
    fprintf (stderr, "Command Line Error! %s\n", msg);
    fprintf (stderr, "Usage:\tbucketSortSeq <array size> <nr of buckets>\n\n");
}





