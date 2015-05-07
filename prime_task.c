//------------------------------------------------------------------------- 
// Copyright (c) 2015 Parker Harris Emerson
// Makes use of supportingsoftware for CS415/515 Parallel
// Programming, copyright (c) Portland State University
//------------------------------------------------------------------------- 

// A sequential prime-finding algorithm.
//
// Usage: ./prime_omp <N>
// 
//
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>

int main(int argc, char **argv) {
  int N;
  int num_threads;

  /* check command line first */
  if (argc < 3){
    printf ("Usage: ./prime <N> <num_threads>\n");
    exit(0);
  }
  if ((N=atoi(argv[1])) < 2) {
    printf ("N must be greater than 1.\n");
    exit(0);
  }
  if ((num_threads = atoi(argv[2])) < 1) {
    printf ("num_threads must be greater than 0.\n");
    exit(0);
  }

  omp_set_num_threads(num_threads);

#ifdef DEBUG
  printf("Finding primes in range 1..%d\n", N);
#endif


  int *array = (int *) malloc(sizeof(int) * (N+1));
  
  #pragma omp parallel for
  for (int i = 2; i <= N; i++)
    array[i] = 1;

  int limit = (int) sqrt((double) N);

  #pragma omp parallel for
  for (int i = 2; i <= limit; i++) {
    if (array[i] == 1) {
      #pragma omp task
      for (int j = i+i; j <= N; j += i) 
        array[j] = 0;
    }
  }

  int cnt = 0;
  # pragma omp parallel for reduction(+:cnt)
  for (int i = 2; i <= N; i++) {
    if (array[i] == 1)
      cnt++;
  }

  printf("Total %d primes found\n", cnt);
}

