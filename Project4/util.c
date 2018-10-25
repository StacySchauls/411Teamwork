#include "util.h"


int *generateInput(int n){
	int *seq = (int*)calloc(n, sizeof(int));
	int i = 0;
	time_t t;
	srand((unsigned) time(&t));

	for (i = 0; i < n; i++){
		*(seq + i) = rand() % 100;
	}
	return seq;
}
int timeToMicroSec(Runtime *r){
	return (r->t2.tv_sec-r->t1.tv_sec) * 1000000 + (r->t2.tv_usec-r->t1.tv_usec);
}
int sendMBytes(int m, Runtime *s){
	char *x = (char*)malloc(m * sizeof(char));//buffer for sending
	int dest = 0;
	gettimeofday(&s->t1, NULL);
	MPI_Send(x, m, MPI_CHAR,dest,0,MPI_COMM_WORLD);
	gettimeofday(&s->t2, NULL);
	int tSend = timeToMicroSec(s);

	//printf("Rank=%d: sent message size %d to rank %d; Send time %d milliseconds\n", rank, m, dest, tSend);
	free(x);
	return tSend;
}

int recvMBytes(int m, Runtime *r){
	char *y = (char*)calloc(m, sizeof(char));
	MPI_Status status;
	gettimeofday(&r->t1,NULL);
	MPI_Recv(y, m, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD,&status);
	gettimeofday(&r->t2, NULL);
	int tRecv = timeToMicroSec(r);
	//printf("Rank=%d: recieved message size %d from rank %d; Recv time %d milliseconds\n", rank, m, status.MPI_SOURCE, tRecv);
	free(y);
	return tRecv;
}

void printCSV(int rank, int m, int time){
	printf("%d,%d,%d\n", rank, m, time);
}
