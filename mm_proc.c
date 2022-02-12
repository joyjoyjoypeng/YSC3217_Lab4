#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <string.h>
#include "mm_test.h"


void mm (size_t N, NUMTYPE * A, NUMTYPE * B, NUMTYPE * C) {
  NUMTYPE  *shared_grid = (NUMTYPE *) mmap(NULL , N * sizeof(NUMTYPE), PROT_READ | PROT_WRITE,MAP_SHARED | MAP_ANONYMOUS , -1, 0);
  // Clone the current process
  int child_pid = fork ();
  if (child_pid == -1) return; // Clone failed
  if (child_pid ==0) {
    for (unsigned int x = 0 ; x < N/2 ; x++) {
      for (unsigned int y = 0 ; y < N/2 ; y++) {
        unsigned int tidx = x + y * N ;
        shared_grid[tidx] = 0;
        for (unsigned int d = 0 ; d < N/2 ; d++) {
          shared_grid[tidx] += A[d + y * N] * B[x + d * N] ;
        }
      }
    }
    sleep (10);
    printf("I␣am␣done␣sleeping .\n");
    exit (0);
    } 
  else {
    for (unsigned int x =  N/2; x < N ; x++) {
      for (unsigned int y = N/2 ; y < N ; y++) {
        unsigned int tidx = x + y * N ;
        shared_grid[tidx] = 0;
        for (unsigned int d = N/2 ; d < N ; d++) {
          shared_grid[tidx] += A[d + y * N] * B[x + d * N] ;
        }
      }
    }
    wait(NULL);
    printf("I␣am␣done␣waiting .\n");
  }
  memcpy(C,shared_grid, N*N*sizeof(NUMTYPE));
}