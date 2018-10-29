#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <mpi.h>
int rank;
int seed, A, p, B, n;
int big_prime;

int *serial_baseline(int n, int A, int B, int P, int output[]);
int *serial_matrix(int n, int A, int B, int P, int output[]);
void *serial_matrix1(int N, int Mo[2][2]);
int parallel_prefix(int A[], int n, char op);
void load_input(int argc, char *argv[]);
int *gen_random(void);
void x_circle(int d[2][2], int m[2][2]);
