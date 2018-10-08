#include "gol.h"
extern int n, G;     //#rows and #col  //# of generatioons
extern int p, rank;   //number of processors and process id
extern CELL **grid, **nextGrid;

int main(int argc, char **argv){
	int i;
	if(argc < 3){
		printf("Not enough arguments.\n");
		exit(-1);
	}
	n = atoi(argv[1]);
	G = atoi(argv[2]);
	grid = (CELL **)malloc(n * sizeof(CELL*));
	for(i = 0; i < n; i++){
		grid[i] = (CELL*)(malloc(n*sizeof(CELL)));
		grid[i]->cur = 0;
		grid[i]->old = '\0';
	}
	// printf("We have %d rows/col and %d generations\n",n,G);

	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);

	if(!generateInitialGoL()){
		printf("init success\n");
		simulate();
	}else
		printf("init failed\n");
	MPI_Finalize();

	return 0;
}
