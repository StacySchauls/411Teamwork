#include "gol.h"
#include "util.h"

extern int n, G;     //#rows and #col  //# of generatioons
extern int p;   //number of processors and process id

int main(int argc, char **argv){
	int rank;
	CELL **local_grid;
	if(argc < 3){
		printf("Not enough arguments.\n");
		exit(-1);
	}
	n = atoi(argv[1]);
	G = atoi(argv[2]);
	printf("# of Generations: %d\n",G);
	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);

	if( (local_grid = generateInitialGoL(rank) )){
		//printf("init success\n");
		displayGoL(local_grid, MPI_COMM_WORLD,rank);
		simulate(local_grid, MPI_COMM_WORLD, rank);
	}else {
		printf("init failed\n");
		return -1;
	}
	displayGoL(local_grid, MPI_COMM_WORLD,rank);
	MPI_Finalize();

	return 0;
}
