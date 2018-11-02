#include "pa4.h"
#include "util.h"


extern int rank;
extern int seed, A, p, B, n;
extern int big_prime;

int main(int argc, char *argv[]){
	int i;
	clock_t p_diff;
	clock_t c_start;
	clock_t c_diff = 0;
	clock_t p_start = clock();
	load_input(argc, argv);// PARALLEL INIT Starts here

	//this is our array of random numbers 
	//n integers ON EACH PROCESS
	//USE N/P FOR SIZE OF ARR. 
	int *arr = (int*)malloc(n*sizeof(int));
	memset(arr, 0, n);//memset(arr, 0, n/p * sizeof(int); 
	if(rank == 0){//only RANK 0 runs the serial code. 
		c_start = clock();
		serial_baseline(arr);
		c_diff = clock() - c_start;
	}
	//putchar(10);
	//serial_matrix(n,A,B,P, arr);

	//Each RANK generates a random array
	gen_random(arr);
	for(i = 0; i < n-1; i++){
		//	printf("rank: %d, arr[%d] : %d\n\n", rank,i, arr[i]);
	}
	//we dont need gather. remove it and such
	//FIX THIS. DONT NEED AN IF ELSE. CREATE A EW OUTPUT ARRAY OF SIZE N:w
	if (rank == 0){
		MPI_Gather(arr, n/p, MPI_INT, arr, n/p, MPI_INT, 0, MPI_COMM_WORLD);
	}
	else{
		MPI_Gather(arr, n/p, MPI_INT, NULL, n/p, MPI_INT, 0, MPI_COMM_WORLD);
	}

	if (rank == 0){
		for (i = 0; i < n; i++){
			printf("Random[%d] = %d\n", i, *(arr + i));
		}
	}
	MPI_Finalize();
	p_diff = clock()-p_start - c_diff;
	p_diff = p_diff * 1000000 / CLOCKS_PER_SEC;
	c_diff = c_diff * 1000000 / CLOCKS_PER_SEC;
	printf("the timeing s: parallel= %ld, cereal = %ld\n",(long int) p_diff/1000000,(long int) c_diff/1000000);
	return 0;
}
