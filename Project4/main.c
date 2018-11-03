#include "pa4.h"


extern int rank;
extern int seed, A, p, B, n;
extern int big_prime;

int main(int argc, char *argv[]){
	int i;
	struct timeval start, end;
	long total;
	clock_t p_diff;
	clock_t c_start;
	clock_t c_diff = 0;
	clock_t p_start = clock();
	load_input(argc, argv);// PARALLEL INIT Starts here

	//this is our array of random numbers 
	//n integers ON EACH PROCESS
	//USE N/P FOR SIZE OF ARR. 
		int *arr = (int*)malloc((n/p)*sizeof(int));
		memset(arr, 0, n/p * sizeof(int));//memset(arr, 0, n/p * sizeof(int); 
		int *serialarr = (int*)malloc((n)*sizeof(int));
	
	if(rank == 0){//only RANK 0 runs the serial code. 
		serial_baseline(serialarr);
	}
	//putchar(10);
	//serial_matrix(n,A,B,P, arr);

	//Each RANK generates a random array
	gettimeofday(&start, NULL);
	gen_random(arr);
	gettimeofday(&end, NULL);
	//total = (end.tv_usec - start.tv_usec)/1000;
	long t = (end.tv_sec*1e6 + end.tv_usec) - (start.tv_sec*1e6 + start.tv_usec);
	long int times[p];
	memset(times,0,p);

	MPI_Gather(&t,1,MPI_LONG,times,1,MPI_LONG,0,MPI_COMM_WORLD);
	MPI_Barrier(MPI_COMM_WORLD);
	//get first 1
/*
	if(rank == 0){
		long int serialTime = times[0];
		long int time2, time4, time8,time16,time32,time64;
		long int i,sum;
		for(i = 1; i <=2 ; i++){
			sum += times[i];
		}
		time2 = sum;

		sum = 0;

		for(i = 3; i <=6; i++){
			sum += times[i]; 
		}
		time4 = sum/4;
		sum = 0;

		for(i =7; i <=14; i++){
			sum+=times[i];
		}
		time8 = sum/8;
		sum = 0;

		for(i = 15; i <=31; i++){
			sum+=times[i];
		}
		time16 = sum/16;
		sum = 0;
		for(i = 32; i <=63; i++){
			sum+=times[i];
		}
		time32 = sum/32;
		sum = 0;
		for(i = 64; i <=127; i++){
			sum+=times[i];
		}
		time64 = sum/64;
		sum = 0;
	
	printf("1:%ld\n2:%ld\n4:%ld\n8:%ld\n16:%ld\n32%ld\n64:%ld\n",serialTime,time2,time4,time8,time16,time32,time64);
	}
*/	
	if(p == 1)
		printf("%ld\n",t);
	//printf("\np is 2: \n");
	if(p == 2)
		printf("%ld\n",t);
	//printf("\np is 4: \n");
	if(p == 4)
		printf("%ld\n",t);
	//printf("\np is 8: \n");
	if(p == 8)
		printf("%ld\n",t);
	//printf("\np is 16: \n");
	if(p == 16)
		printf("%ld\n",t);
	//printf("\np is 32: \n");
	if(p == 32)
		printf("%ld\n",t);
	//printf("\np is 64: \n");
	if(p == 64)
		printf("%ld\n",t);
	

	/*
	for(i = 0; i < n-1; i++){
		//	printf("rank: %d, arr[%d] : %d\n\n", rank,i, arr[i]);
	}*/
	//we dont need gather. remove it and such
	//FIX THIS. DONT NEED AN IF ELSE. CREATE A EW OUTPUT ARRAY OF SIZE N:w
	
		//MPI_Gather(arr, n/p, MPI_INT, arr, n/p, MPI_INT, 0, MPI_COMM_WORLD);
/*
	if (rank == 0){
		for (i = 0; i < n; i++){
			printf("Random[%d] = %d\n", i, *(arr + i));
		}
	}
*/
	MPI_Finalize();
	return 0;
}
