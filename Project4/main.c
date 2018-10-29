#include "pa4.h"


extern int rank;
extern int seed, A, p, B, n;
extern int big_prime;
int main(int argc, char *argv[]){
	int arr[n];
	memset(arr, 0, n);
	//serial_baseline(n,A,B, arr);
	putchar(10);
	//serial_matrix(n,A,B,P, arr);
	load_input(argc, argv);
	gen_random();
	MPI_Finalize();
	return 0;
}
