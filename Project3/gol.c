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



int generateInitialGoL(){
	int j;
	CELL **my_grid = (CELL **)malloc((n/p)*sizeof(CELL*));
	for(j = 0; j<(n/p); j++){
		my_grid[j] = (CELL*)malloc(n*sizeof(CELL));
		grid[j]->cur = 0;
		grid[j]->old = '\0';
	}
	int rNum,i,k;
	if(rank == 0){// master process 0
		srand(time(NULL));
		for(i=0;i<p;i++){//sending random number to each process
			rNum = randNum();
			MPI_Send(&rNum,1, MPI_INT, i, 0, MPI_COMM_WORLD);
		}

	}else{ // workers 1 :: p-1
		int generated_num;
		MPI_Recv(&rNum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		srand(rNum);
		for(i = 0; i<(n/p); i++){
			printf("\n");
			for(k = 0; k<n; k++){
				generated_num = randNum();
				if(generated_num %2 == 0){

					my_grid[i][k].old = my_grid[i][k].cur = ALIVE;// set both to alive
					//printf("cell at [%d][%d] is %c\n",i,k, my_grid[i][k].cur);
					printf("%c", my_grid[i][k].cur);
				}else{

					my_grid[i][k].old = my_grid[i][k].cur = DEAD;

					printf("%c", my_grid[i][k].cur);
				}
			}
		}
	}
	return 0;
}

int simulate(){
	return 0;
}

int determineState(CELL **grid, char buf[], int row){//updates a full row
	int num_alive = 0, col = 0;
	char val;
	// count the number of neighbors with old alive

	//two communication steps
	if (row == 0 || row == (n/p - 1)) { //use buffer

	}

	for (col = 0; col < n; col++) {
		//set num_alive for each col value


		//assuming in our grid
		if(row == 0){
			//top row
			//check Norths
			if(buf[(col==0)?n-1:(col-1)].old == 'x'){
				num_alive++;
			}
			if(buf[col].old == 'x'){
				num_alive++;
			}
			if(buf[(col+1)%n].old == 'x'){
				num_alive++;
			}

			//check souths
			if(grid[row+1[(col==0)?n-1:(col-1)].old == 'x'){
				num_alive++;
			}
			if(grid[row+1][col].old == 'x'){
				num_alive++;
			}
			if(grid[row+1][(col+1)%n].old == 'x'){
				num_alive++;
			}

			//current row
			if(grid[row][(col==0)?n-1:(col-1)].old == 'x'){
				num_alive++;
			}
			if(grid[row][(col+1)%n].old == 'x'){
				num_alive++;
			}
		}else if(row == (n/p)-1){//bottom row

			//check Norths
			if(grid[row-1][(col==0)?n-1:(col-1)].old == 'x'){
				num_alive++;
			}
			if(grid[row-1][col].old == 'x'){
				num_alive++;
			}
			if(grid[row-1][(col+1)%n].old == 'x'){
				num_alive++;
			}

			//check souths
			if(buf[(col==0)?n-1:(col-1)].old == 'x'){
				num_alive++;
			}
			if(buf[col].old == 'x'){
				num_alive++;
			}
			if(buf[(col+1)%n].old == 'x'){
				num_alive++;
			}

			//current row
			if(grid[row][(col==0)?n-1:(col-1)].old == 'x'){
				num_alive++;
			}
			if(grid[row][(col+1)%n].old == 'x'){
				num_alive++;
			}

		}else{

			if(grid[row-1][(col==0)?n-1:(col-1)].old == 'x'){
				num_alive++;
			}
			if(grid[row-1][col].old == 'x'){
				num_alive++;
			}
			if(grid[row-1][(col+1)%n].old == 'x'){
				num_alive++;
			}

			//check souths
			if(grid[row+1][(col==0)?n-1:(col-1)].old == 'x'){
				num_alive++;
			}
			if(grid[row+1][col].old == 'x'){
				num_alive++;
			}
			if(grid[row+1][(col+1)%n].old == 'x'){
				num_alive++;
			}

			//current row
			if(grid[row][(col==0)?n-1:(col-1)].old == 'x'){
				num_alive++;
			}
			if(grid[row][(col+1)%n].old == 'x'){
				num_alive++;
			}

		}





		if (num_alive > 2 && num_alive < 6) { //we are alive
			grid[row][col].cur = ALIVE;
		}
		else{
			grid[row][col].cur = DEAD;
		}
	}
	return 0;
}
int randNum(){
	int i;
	i = (rand() % BIG_PRIME) + 1;
	// printf("Returning: %d\n",i);
	return i;
}
