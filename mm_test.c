#include <mm_test.h>

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h> 
#include <sys/time.h>

void print_matrix (int N, NUMTYPE *A) {
  
  for (int y = 0 ; y < N ; y++) {
    for (int x = 0 ; x < N ; x++) {      
      int i = x + y * N;
      if (x) {
	fprintf(stdout,  " " NUMFORMAT, A[i]);
      } else {
	fprintf(stdout,  NUMFORMAT, A[i]);
      }
    }
    fprintf(stdout, "\n");
  }

}

int compare_matrices (int N,NUMTYPE *A, NUMTYPE *B) {
  int count = 0;
  for (int y = 0 ; y < N ; y++) {
    for (int x = 0 ; x < N ; x++) {      
      int i = x + y * N;   
      double diff = abs(A[i] - B[i]) ;
      if (diff > EPSILON) {
	count++;
	fprintf (stderr,"Error res " NUMFORMAT " != expected " NUMFORMAT " diff = %f\n", A[i] , B[i], diff);
      }      
    }
  }
  return count;

}


int main (int argc, char** argv) {

  assert(argc == 2);
  
  char * input_file = argv[1];
  FILE * fdesc = fopen(input_file,"r");
  assert(fdesc);
  
  long int N;
  assert(fscanf(fdesc,"%ld", &N) == 1);  
  
  // 1 - Parse input
  fprintf(stderr, "Please wait... reading %ld inputs.\n", N * N * 3);

  NUMTYPE *A    = (NUMTYPE*) malloc(N * N * sizeof (NUMTYPE));
  NUMTYPE *B    = (NUMTYPE*) malloc(N * N * sizeof (NUMTYPE));
  NUMTYPE *C    = (NUMTYPE*) malloc(N * N * sizeof (NUMTYPE));
  NUMTYPE *G    = (NUMTYPE*) malloc(N * N * sizeof (NUMTYPE));

  assert(A);
  assert(B);
  assert(C);
  assert(G);
  
  for (int i = 0 ; i < N * N ; i++) {
    assert(fscanf(fdesc,NUMFORMAT, &A[i]) == 1);    
  }
  for (int i = 0 ; i < N * N ; i++) {
    assert(fscanf(fdesc,NUMFORMAT, &B[i]) == 1);        
  }
  for (int i = 0 ; i < N * N ; i++) {
    assert(fscanf(fdesc,NUMFORMAT, &G[i]) == 1);    
  }

  fprintf(stderr, "Start computation.\n");
  static struct timeval t1, t2 ; 
  gettimeofday(&t1, NULL);

  mm (N,A,B,C);
  
  gettimeofday(&t2, NULL);
  
  double duration =((long long int)(t2.tv_sec - t1.tv_sec) * 1000000u + t2.tv_usec - t1.tv_usec) / 1.e6;
  fprintf(stderr, "Computation finished, duration = %f sec.\n", duration);

  int errors = compare_matrices (N,C,G);
  fprintf(stderr, "Total errors: %d.\n", errors);

  fprintf(stdout, "%ld\n", N);
  print_matrix(N,A);
  print_matrix(N,B);
  print_matrix(N,C);

  free(A);
  free(B);
  free(C);
  free(G);

  
}
