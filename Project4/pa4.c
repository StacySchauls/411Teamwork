#include "pa4.h"


int *serial_baseline(int n, int A, int B, int P, int output[]){
	int seed = 909;
	int i = 0;
	output[0] = seed;
	printf("serial[0] : %d \n", seed);
	for(i = 1; i<n; i++){
		output[i] = (A* (output[i-1] ) + B )% P;
		printf("serial[%d] : %d \n", i, output[i]);
	}

	return output;
}


int *serial_matrix(int n, int A, int B, int P, int output[]){
	int seed = 909;
	int i = 0;
	long long int tA = A, tB = B;
	output[0] = seed;
	printf("matrix[0] : %d \n", seed);
	for(i = 1; i<n; i++){
		//calculate our matrix to the i-th power
		output[i] = (tA* seed + tB )% P;
		printf("[%lld, 0]\n[%lld, 1]\n\n", tA,tB);
		printf("matrix[%d] : %d \n", i, output[i]);
		tA = tA*A + 0;
		tB = tB*A + B;
	}

	return output;

}
