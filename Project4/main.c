#include "pa4.h"


extern int rank;
extern int seed, A, p, B, n;
extern int big_prime;
int main(int argc, char *argv[]){
	int arr[n];
	load_input(argc, argv);
	memset(arr, 0, n);
	if(rank == 0){

		serial_baseline(arr);
	}
	putchar(10);
	//serial_matrix(n,A,B,P, arr);
	gen_random();
	MPI_Finalize();
	return 0;
}
