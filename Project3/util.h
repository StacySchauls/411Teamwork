#ifndef UTIL_H
#define UTIL_H

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <mpi.h>
#include <assert.h>
#include <sys/time.h>

#define BUFSIZE 1048576



typedef struct runtime{
	struct timeval t1;
	struct timeval t2;
} Runtime;

int *generateInput(int n);
int timeToMicroSec(Runtime *r);
int sendMBytes(int m, Runtime *s);
int recvMBytes(int m, Runtime *r);
void printCSV(int rank, int m, int time);

#endif
