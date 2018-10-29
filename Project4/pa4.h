#include <string.h>
#include <stdlib.h>
#include <stdio.h>


int *serial_baseline(int n, int A, int B, int P, int output[]);
int *serial_matrix(int n, int A, int B, int P, int output[]);
int parallel_prefix(int A[], int n, char op);
void load_input(int argc, char *argv[]);
int *serial_matrix1(int N, int Mo[2][2]);
int *gen_random(void);
