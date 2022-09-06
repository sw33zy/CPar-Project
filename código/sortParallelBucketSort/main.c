#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <omp.h>
#include "papi.h"

#include "bucketSortParSort.h"

static int verify_command_line (int argc, char *argv[], int *m_size, int *version);
static void print_usage (char *msg);


// PAPI events to monitor
#define NUM_EVENTS 4
int Events[NUM_EVENTS] = { PAPI_TOT_CYC, PAPI_TOT_INS, PAPI_L1_DCM, PAPI_L2_DCM };
// PAPI counters' values
long long values[NUM_EVENTS], min_values[NUM_EVENTS];
int retval, EventSet=PAPI_NULL;

// number of times the function is executed and measured
#define NUM_RUNS 10

int main (int argc, char *argv[]) {
    long long start_usec, end_usec, elapsed_usec, min_usec=0L;
    int num_hwcntrs = 0;
    int run,i;
    int dim;	// Array size 
    int n_buckets;	// Requested number of buckets
    
    if (!verify_command_line (argc, argv, &dim, &n_buckets)) {
        return 0;
    }

    //fprintf(stdout, "\n Number of threads: %d",omp_get_max_threads());
    fprintf (stdout, "\nSetting up PAPI...");
    // Initialize PAPI
    retval = PAPI_library_init(PAPI_VER_CURRENT);
    if (retval != PAPI_VER_CURRENT) {
        fprintf(stderr,"PAPI library init error!\n");
        return 0;
    }
    
    /* create event set */
    if (PAPI_create_eventset(&EventSet) != PAPI_OK) {
        fprintf(stderr,"PAPI create event set error\n");
        return 0;
    }
    
    
    /* Get the number of hardware counters available */
    if ((num_hwcntrs = PAPI_num_hwctrs()) <= PAPI_OK)  {
        fprintf (stderr, "PAPI error getting number of available hardware counters!\n");
        return 0;
    }
    fprintf(stdout, "done!\nThis system has %d available counters.\n\n", num_hwcntrs);
    
    // We will be using at most NUM_EVENTS counters
    if (num_hwcntrs >= NUM_EVENTS) {
        num_hwcntrs = NUM_EVENTS;
    } else {
        fprintf (stderr, "Error: there aren't enough counters to monitor %d events!\n", NUM_EVENTS);
        return 0;
    }
    
    if (PAPI_add_events(EventSet,Events,NUM_EVENTS) != PAPI_OK)  {
        fprintf(stderr,"PAPI library add events error!\n");
        return 0;
    }
    
    
    for (run=0 ; run < NUM_RUNS ; run++) {
        
        // use PAPI timer (usecs) - note that this is wall clock time
        // for process time running in user mode -> PAPI_get_virt_usec()
        // real and virtual clock cycles can also be read using the equivalent
        // PAPI_get[real|virt]_cyc()
        start_usec = PAPI_get_real_usec();
        
        /* Start counting events */
        if (PAPI_start(EventSet) != PAPI_OK) {
            fprintf (stderr, "PAPI error starting counters!\n");
            return 0;
        }
        
        sequentialBucketSort(dim,n_buckets);
        
        /* Stop counting events */
        if (PAPI_stop(EventSet,values) != PAPI_OK) {
            fprintf (stderr, "PAPI error stoping counters!\n");
            return 0;
        }
        
        end_usec = PAPI_get_real_usec();
        fprintf (stdout, "done!\n");
        
        elapsed_usec = end_usec - start_usec;
        
        if ((run==0) || (elapsed_usec < min_usec)) {
            min_usec = elapsed_usec;
            for (i=0 ; i< NUM_EVENTS ; i++) min_values[i] = values [i];
        }
        
    } // end runs
    fprintf (stdout,"\nWall clock time: %lld usecs\n", min_usec);
    
    // output PAPI counters' values
    for (i=0 ; i< NUM_EVENTS ; i++) {
        char EventCodeStr[PAPI_MAX_STR_LEN];
        
        if (PAPI_event_code_to_name(Events[i], EventCodeStr) == PAPI_OK) {
            fprintf (stdout, "%s = %lld\n", EventCodeStr, min_values[i]);
        } else {
            fprintf (stdout, "PAPI UNKNOWN EVENT = %lld\n", min_values[i]);
        }
    }
    
    fprintf (stdout,"\nThat's all, folks\n");
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

/*
int alloc_matrix (float **m, int N) {
    
    *m = (float *) malloc (N*sizeof(float));
    if (!(*m)) {
        print_usage ((char *)"Could not allocate memory for matrix!");
        return 0;
    }
    return 1;
}

float my_rand (void) {
    double d;
    
    d = drand48 ();
    d *= 1.E10;
    d -= 0.5E10;
    return ((float)d);
}

int ini_matrix (float **m, int N) {
    int i;
    float *ptr;
    
    if (!alloc_matrix (m, N)) return 0;
    for (i=0 , ptr = (*m) ; i<N ; i++ , ptr++) {
        *ptr = my_rand();
    }
    return 1;
} 

int ini_zero_matrix (float **m, int N) {
    if (!alloc_matrix (m, N)) return 0;
    bzero ((*m), N*sizeof(float));
    return 1;
} 

int free_matrix (float **m) {
    free (*m);
    *m = NULL;
    return 1;
}*/



