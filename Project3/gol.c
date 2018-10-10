#include "gol.h"
extern int n, G;     //#rows and #col  //# of generatioons
extern int p, rank;   //number of processors and process id
extern CELL **grid, **nextGrid;

CELL **generateInitialGoL(){
	int j;
	int rNum,i,k; // <-- potential error spot
	CELL **my_grid = (CELL **)malloc((n/p)*sizeof(CELL*));
	for(j = 0; j<(n/p); j++){
		my_grid[j] = (CELL*)malloc(n*sizeof(CELL));
		grid[j]->cur = 0;
		grid[j]->old = '\0';
	}


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
	return my_grid;
}

int simulate(CELL **grid, MPI_Comm comm){

	int i, j = 0;
	char buf[n],bufBelow[n],bufAbove[n];
	memset(buf, 0, n);
	memset(bufBelow, 0, n);
	memset(bufAbove, 0, n);
	while(j < G){
		for(i = 0; i<n; i++){
			buf[i] = grid[0][i].old;
		}	
		MPI_Barrier(comm);
		if(rank != 1){
			MPI_Send(buf, n, MPI_CHAR, rank-1,0,comm);
		}if else{
			MPI_Send(buf, n, MPI_CHAR, p-1,0,comm);
		}

		if(rank == p-1){
			MPI_Recv(bufBelow, n, MPI_CHAR, 1,0,comm);
		}else{
			MPI_Recv(bufBelow, n, MPI_CHAR, rank+1,0,comm);
		}
		MPI_Barrie(comm);

		if(rank == p-1){
			MPI_Send(buf, n, MPI_CHAR, 1,0,comm);
		}if else{
			MPI_Send(buf, n, MPI_CHAR, rank +1,0,comm);
		}

		if(rank == 1){
			MPI_Recv(bufAbove, n, MPI_CHAR, p-1,0,comm);
		}else{
			MPI_Recv(bufAbove, n, MPI_CHAR, rank-1,0,comm);
		}

		MPI_Barrier(comm);

		determineState(grid,bufAbove,0);
		for(i = i; i<(n/p)-1; i++){
			determineState(grid,bufBelow,i);
		}
		j++;
	}
}	
return 0;

}

int determineState(CELL **grid, char buf[], int row){//updates a full row
	int num_alive = 0, col = 0;
	// count the number of neighbors with old alive

	for (col = 0; col < n; col++) {
		//set num_alive for each col value
		//assuming in our grid
		if(row == 0){
			//top row
			//check Norths
			if(buf[(col==0)?n-1:(col-1)] == 'x'){
				num_alive++;
			}
			if(buf[col] == 'x'){
				num_alive++;
			}
			if(buf[(col+1)%n] == 'x'){
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
			if(buf[(col==0)?n-1:(col-1)] == 'x'){
				num_alive++;
			}
			if(buf[col] == 'x'){
				num_alive++;
			}
			if(buf[(col+1)%n] == 'x'){
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
			grid[row][col].old = grid[row][col].cur;
			grid[row][col].cur = ALIVE;

		}
		else{
			grid[row][col].old = grid[row][col].cur;
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
