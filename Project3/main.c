#include "gol.h"
extern int n, G;     //#rows and #col  //# of generatioons
extern int p, rank;   //number of processors and process id

int main(int argc, char **argv){
	CELL **local_grid;
	if(argc < 3){
		printf("Not enough arguments.\n");
		exit(-1);
	}
	n = atoi(argv[1]);
	G = atoi(argv[2]);

	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);

	if( (local_grid = generateInitialGoL() )){
		//printf("init success\n");
		simulate(local_grid, MPI_COMM_WORLD);
		displayGoL(local_grid, MPI_COMM_WORLD);
	}else {
		printf("init failed\n");
	}
	MPI_Finalize();

	return 0;
}
