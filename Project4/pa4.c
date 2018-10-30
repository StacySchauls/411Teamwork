#include "pa4.h"

extern int rank;
extern int seed, A, p, B, n;
extern int big_prime;


int *serial_baseline(int output[]){
	//int seed = 909;
//	printf("A B : %d %d \n",A, B);
	int i = 0;
	output[0] = seed;
//	printf("serial[0] : %d \n", seed);
	for(i = 1; i<n; i++){
		output[i] = (A* (output[i-1] ) + B )% big_prime;
		printf("serial[%d] : %d \n", i, output[i]);
	}

	return output;
}




void serial_matrix1(int output[], int N, int Mo[2][2]){
	int i = 0;
	long long tA = Mo[0][0], tB = Mo[1][0];
	
	printf("\n\n- - - - - - - - - - - - - - - - - - - - - - - - -\n");
	printf("Starting serial_matrix1, rank = %d\n", rank);
	printf("Mo: { {%d, %d}, {%d, %d} }\n",Mo[0][0], Mo[0][1], Mo[1][0], Mo[1][1]);
	//printf("N is %d\n", N);
	//printf("matrix[0] : %d \n", seed);
	for(i = 0; i<N; i++){
		//printf("i is : %d\n", i);
		//calculate our matrix to the i-th power
		output[i] = (tA* seed + tB )% big_prime;
	//	printf("[%d, 0]\n[%d, 1] rank: %d \n\n", tA,tB,rank);
		printf("matrix[%d] : %lld rank %d \n\n", i,(long long) output[i],rank);
		tA = tA*A + 0;
		tB = tB*A + B;
	}
	printf("Exiting serial_matrix1, rank = %d\n", rank);
	printf("- - - - - - - - - - - - - - - - - - - - - - - - -\n");
}
/*int *serial_matrix(int n, int A, int B, int P, int output[]){
	int seed = 909;
	int i = 0;
	long long int tA = A, tB = B;
	output[0] = seed;
	printf("matrix[0] : %d \n", seed);
	for(i = 1; i<n; i++){
		//calculate our matrix to the i-th power
		output[i] = (tA* seed + tB )% p;
		printf("[%lld, 0]\n[%lld, 1]\n\n", tA,tB);
		printf("matrix[%d] : %d \n", i, output[i]);
		tA = tA*A + 0;
		tB = tB*A + B;
	}

	return output;

}*/


//g gp does not change outside of the file
void parallel_prefix(int Mo[2][2], int * Ml){
	int l[2][2];
	int g[2][2];
	int *gp = (int *)malloc(sizeof(int[2][2]));
	int *gt = (int *)malloc(sizeof(int[2][2]));
	int v = 1, t = 1, mate = 0;
	printf("\n\n- - - - - - - - - - - - - - - - - - - - - - - - -\n");
	printf("Starting parallel_prefix, rank = %d\n", rank);
	
	double var = log( (double) p) / log(2);
	memcpy(l, Ml+((n/p) -1), sizeof(l));
	memcpy(g, Ml+((n/p) -1), sizeof(g));
	for(t = 0; t <var - 1 ; t++){
		mate = rank ^ v;
		v = v <<  1;
		memcpy(gt,g, sizeof(g));
		MPI_Send(gt, 4, MPI_INT, mate, 0, MPI_COMM_WORLD);//MIGHT BREAK
		MPI_Recv(gp, 4, MPI_INT, mate, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

		//g += gp;
		x_circle(g, gp);
		if(mate<rank){
			// l+=gp;
			x_circle(l, gp);
		}
	}
	printf("l: { {%d, %d}, {%d, %d} }\n",l[0][0], l[0][1], l[1][0], l[1][1]);
	memcpy(Mo, l, sizeof(l));
	free(gp);
	printf("Exiting parallel_prefix, rank = %d\n", rank);
	printf("- - - - - - - - - - - - - - - - - - - - - - - - -\n");
}

void load_input(int argc, char *argv[]){
	if(argc < 4){
		printf("Usage: \n Must have 4 arguments: n, Seed, A, B,<BIG_PRIME> ");
		exit(-1);
	}
	n = 0;
	int *arr = NULL;
	arr = (int *) calloc(n , sizeof(int));
	arr[0] = seed;
	printf(" %s %s %s\n",  argv[1], argv[2], argv[3]);
	n = atoi(argv[1]);
	seed = atoi(argv[2]);
	A = atoi(argv[3]);
	B = atoi(argv[4]);
	
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if(argc == 5){
		big_prime = atoi(argv[5]);
	}else{
		big_prime = 997;
	}

}

void gen_random(int array[]){
	//step 1
	int M[2][2] = { { A, 0}, {B, 1}};
	int Mp[2][2] = { { 1, 0}, {0, 1}};
	int Ml[2][2];
	int Mo[2][2];
	int i;

	memset(array, 0 , sizeof((n/p) * sizeof(int)));
	//step 2: populate local xl
	int *xl = (int*) malloc((n/p) * sizeof(int[2][2]));
	for(i = 0; i < n/p; i++){
		//printf("in here okay %d \n", i);
		memcpy(xl + (i* sizeof(int[2][2])), M, sizeof(M));
	}
	//printf("yay we are out\n");
	if(rank == 0)
		memcpy(xl,Mp, sizeof(Mp));

	//step 3 calculate Mlocal

	memcpy(Ml, Mp, sizeof(Mp));
	for(i = 0; i < (n/p) -1; i++){
		//multiply matricies
		x_circle(Ml, xl + (i* sizeof(int[2][2])));
		memcpy(xl + (i* sizeof(int[2][2])), Ml, sizeof(Ml));
		
		printf("Ml: { {%d, %d}, {%d, %d} }\n",Ml[0][0], Ml[0][1], Ml[1][0], Ml[1][1]);
	}
	//step 4
	parallel_prefix(Mo, xl);
	
	//step 5
	serial_matrix1(array, n/p, Mo);
}


void x_circle(int d[2][2], int *m){
	int t[2][2];
	memcpy(t, d, sizeof(int[2][2]));
	
	//printf("m: { {%d, %d}, {%d, %d} }\n",*m, *(m+1), *(m+2), *(m+3));

	d[0][0] = t[0][0] * *m     + t[0][1] * *(m+2);
	d[0][1] = t[0][0] * *(m+1) + t[0][1] * *(m+3);
	d[1][0] = t[1][0] * *m     + t[1][1] * *(m+2);
	d[1][1] = t[1][0] * *(m+1) + t[1][1] * *(m+3);
	printf("d: { {%d, %d}, {%d, %d} }\n",d[0][0], d[0][1], d[1][0], d[1][1]);
	printf("t: { {%d, %d}, {%d, %d} }\n",t[0][0], t[0][1], t[1][0], t[1][1]);
}
