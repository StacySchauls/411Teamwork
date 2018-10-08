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
	grid = (char **)malloc(n * sizeof(char*));
	for(i = 0; i < n; i++){
		grid[i] = (char*)(malloc(n*sizeof(char)));
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



int generateInitialGoL(){
	int j;
	CELL **my_grid = (CELL **)malloc((n/p)*sizeof(CELL*));
	for(j = 0; j<(n/p); j++){
		my_grid[j] = (CELL*)malloc(n*sizeof(CELL));
	}
	int rNum,i,k;
	if(rank == 0){
		srand(time(NULL));
		for(i=0;i<p;i++){
			rNum = randNum();
			MPI_Send(&rNum,1, MPI_INT, i, 0, MPI_COMM_WORLD);
		}

	}else{
		int generated_num;
		MPI_Recv(&rNum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		srand(rNum);
		for(i = 0; i<(n/p); i++){
			printf("\n");
			for(k = 0; k<n; k++){
				generated_num = randNum();
				if(generated_num %2 == 0){

					my_grid[i][k].old = my_grid[i][k].cur = ALIVE;
					printf("cell at [%d][%d} is %c\n",i,k, my_grid[i][k].cur);
					//printf("%c"),my_grid[i][k].cur;
				}else{

					my_grid[i][k].old = my_grid[i][k].cur = DEAD;

					printf(""),my_grid[i][k].cur;
				}
			}
		}
	}
}

int simulate(){

	return 0;
}

int determineState(int row, int col){

	return 0;
}
int randNum(){
	int i;
	i = (rand() % BIG_PRIME) + 1;
	// printf("Returning: %d\n",i);
	return i;
}
