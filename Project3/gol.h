#ifndef GOL_H
#define GOL_H


#include <stddef.h>
#include <getopt.h>
#include <mpi.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include <getopt.h>
#include <stdio.h>

#define DEAD '.'
#define ALIVE 'x'
#define BIG_PRIME 93377

//struct with 2 chars
typedef struct Cell{
  char old;
  char cur;
} CELL;



//function prototypes
int generateInitialGoL();
int simulate();
int displayGoL();
int randNum();
int simulate();
int determineState();
//global variables
int n, G;     //#rows and #col  //# of generatioons
int p, rank;   //number of processors and process id
CELL **grid, **nextGrid;

#endif //!GOL_H
