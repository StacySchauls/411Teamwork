#include "pa4.h"


int *serial_baseline(int n, int A, int B, int P, int output[]){
	int seed = 909;
	int i = 0;
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
	int tA = A, tB = B;
	int matrix[2][2] = { {A, 0},
						 {B, 1} };
	printf("matrix[0] : %d \n", seed);
	for(i = 1; i<n; i++){
		//calculate our matrix to the i-th power
		output[i] = (tA* (output[i-1] ) +  1 * tB )% P;
		printf("matrix[%d] : %d \n", i, output[i]);
		tA = tA*A;
		tB = tB*A + B;
	}

	return output;

}
