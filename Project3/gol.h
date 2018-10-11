#ifndef GOL_H
#define GOL_H


#include <stddef.h>
#include <getopt.h>
#include <mpi.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#define DEAD '.'
#define ALIVE 'x'
#define BIG_PRIME 93377

// GLOBALS
int n, G;     //#rows and #col  //# of generatioons
int p, rank;   //number of processors and process id

//struct with 2 chars
typedef struct Cell{
  char old;
  char cur;
} CELL;



//function prototypes
CELL **generateInitialGoL();
int simulate(CELL **grid, MPI_Comm comm, int rank);
int determineState(CELL **grid,char buf[],int row);
int displayGoL(CELL **grid, MPI_Comm comm,int rank);
int randNum();

#endif //!GOL_H
