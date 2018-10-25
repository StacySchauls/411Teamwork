#include "pa4.h"


int *serial_baseline(int n, int A, int B, int P, int output[]){
	int seed = 909;
	int i;
	memset(output, 0, n);
	printf("[0] : %d \n", seed);
	for(i = 1; i<n; i++){
		output[i] = (A* (output[i-1] ) + B )% P;
		printf("[%d] : %d \n", i, output[i]);
	}

	return output;
}
