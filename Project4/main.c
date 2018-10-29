#include "pa4.h"
#include "util.h"


extern int rank;
extern int seed, A, p, B, n;
extern int big_prime;
int main(int argc, char *argv[]){
	int arr[n];
	clock_t p_diff;
	clock_t c_start;
	clock_t c_diff = 0;
	clock_t p_start = clock();
	load_input(argc, argv);
	memset(arr, 0, n);
	if(rank == 0){
		c_start = clock();
		serial_baseline(arr);
		c_diff = clock() - c_start;
	}
	putchar(10);
	//serial_matrix(n,A,B,P, arr);
	gen_random();
	p_diff = clock()-p_start - c_diff;
	p_diff = p_diff * 1000000 / CLOCKS_PER_SEC;
	c_diff = c_diff * 1000000 / CLOCKS_PER_SEC;
	printf("the timeing s: parallel= %d, cereal = %d\n",(int) p_diff/1000000,(int) c_diff/1000000);
	MPI_Finalize();
	return 0;
}
