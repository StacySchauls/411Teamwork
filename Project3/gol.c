#include "gol.h"
#include "util.h"
extern int n, G;     //#rows and #col  //# of generatioons
extern int p;   //number of processors and process id

CELL **generateInitialGoL(int rank){
	int j;
	int rNum,i,k; // <-- potential error spot

	CELL **my_grid = (CELL **)malloc((n/p)*sizeof(CELL*));
	for(j = 0; j<(n/(p-1)); j++){
		my_grid[j] = (CELL*)malloc(n*sizeof(CELL));
		my_grid[j]->cur = 0;
		my_grid[j]->old = '\0';
	}
	if(rank == 0){// master process 0
		srand(time(NULL));
		for(i=1;i<p;i++){//sending random number to each process
			rNum = randNum();
			MPI_Send(&rNum,1, MPI_INT, i, 0, MPI_COMM_WORLD);
		}
	}else{ // workers 1 :: p-1
		int generated_num;
		//printf("generateInitialGoL: rank %d\n", rank);
		MPI_Recv(&rNum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		srand(rNum);
		for(i = 0; i<(n/(p-1)); i++){
			//	printf("\n");
			for(k = 0; k<n; k++){
				generated_num = randNum();
				if(generated_num %2 == 0){

					my_grid[i][k].old = 'x';
					my_grid[i][k].cur = 'x';// set both to alive
					//printf("cell at [%d][%d] is %c\n",i,k, my_grid[i][k].cur);
					//printf("%c", my_grid[i][k].cur);
				}else{

					my_grid[i][k].old = '.';
					my_grid[i][k].cur = '.';

					//printf("%c", my_grid[i][k].cur);
				}
			}
		}
	}
	return my_grid;
}

int simulate(CELL **grid, MPI_Comm comm, int rank){

	int i, j = 0;
	Runtime sim_time;
	Runtime gen_time;
	char buf[n], bufBelow[n], bufAbove[n];
	gettimeofday(&sim_time.t1, NULL);
	memset(buf, 0, n);
	memset(bufBelow, 0, n);
	memset(bufAbove, 0, n);
	if(rank == 0){
		for(i = 0; i< G; i++){
		
		//printf("BARRIER1 in simulate: rank %d\n",rank);
		MPI_Barrier(comm);
		//printf("BARRIER2 in simulate: rank %d\n",rank);
		MPI_Barrier(comm);
		//printf("BARRIER3 in simulate: rank %d\n",rank);
		MPI_Barrier(comm);}

	}else{

		while (j < G){
			gettimeofday(&gen_time.t1, NULL);
			for(i = 0; i<n; i++){
				buf[i] = grid[0][i].old;
			}
			//printf("BARRIER1 in simulate: rank %d\n",rank);
			MPI_Barrier(comm);
			if(rank != 1){
				MPI_Send(buf, n, MPI_CHAR, rank-1,0,comm);
			}else{
				MPI_Send(buf, n, MPI_CHAR, p-1,0,comm);
			}

			if(rank == p-1){
				MPI_Recv(bufBelow, n, MPI_CHAR, 1, 0, comm, MPI_STATUS_IGNORE);
			}else{
				MPI_Recv(bufBelow, n, MPI_CHAR, rank+1, 0, comm, MPI_STATUS_IGNORE);
			}
			//printf("BARRIER2 in simulate: rank %d\n",rank);
			MPI_Barrier(comm);

			if(rank == p-1){
				MPI_Send(buf, n, MPI_CHAR, 1,0,comm);
			}else{
				MPI_Send(buf, n, MPI_CHAR, rank +1,0,comm);
			}

			if(rank == 1){
				MPI_Recv(bufAbove, n, MPI_CHAR, p-1,0,comm, MPI_STATUS_IGNORE);
			}else{
				MPI_Recv(bufAbove, n, MPI_CHAR, rank-1,0,comm, MPI_STATUS_IGNORE);
			}

			//printf("BARRIER3 in simulate: rank %d\n",rank);
			MPI_Barrier(comm);

			determineState(grid, rank,bufAbove,0);
			for(i = 1; i<(n/(p-1)); i++){
				determineState(grid,rank,bufBelow,i);
			}
			j++;
		}
	}
	//printf("Rank %d is exiting simulate for row.\n",rank);
	gettimeofday(&sim_time.t2, NULL);
	int sim_avg = timeToMicroSec(&sim_time);
	int * sim_avgs = NULL;
	if (rank == 0){
		sim_avgs = (int*)malloc(sizeof(int) *p);
	}
	else{
		MPI_Gather(&sim_avg, 1, MPI_INT, sim_avgs, 1 , MPI_INT, 0, comm);
	}
	if (rank == 0){
		int sum = 0;
		for (i = 0; i < p; i++){
			sum += sim_avgs[i];
		}
		sim_avg = (int)(sum/(double)p);
	}
	
	return 0;
}




int determineState(CELL **grid, int rank, char buf[], int row){//updates a full row
	//printf("in determine state. Rank %d\n",rank);
	//printf("DETERMINESTATE rank: %d  Row %d\n",rank,row);
	int num_alive = 0, col = 0;
	// count the number of neighbors with old alive
	for (col = 0; col < n; col++) {
		num_alive = 0;		
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
		}else if(row == (n/p)){//bottom row

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

		//printf("NUM ALIVE:%d\n",num_alive);
		if (num_alive > 2 && num_alive < 6) { //we are alive
			grid[row][col].old = grid[row][col].cur;
			grid[row][col].cur = 'x';

		}
		else{
			grid[row][col].old = grid[row][col].cur;
			grid[row][col].cur = '.';
		}
	}
	//printf("Rank %d is exiting DetermineState For row %d.\n",rank,row);
	return 0;
}

int displayGoL(CELL **grid, MPI_Comm comm, int rank){
	int i = 0;
	int j = 0;
	int blocksize = (int)(((n*n)/(p-1)));
	char buf[blocksize];
	if(rank == 0){

		printf("\n\n");
	}
	//printf("The blocksize is: (%d * %d) / %d = %d\n",n,n,p-1,blocksize);
	memset(buf, 0, blocksize);
	//printf("Rank %d at barrier in display \n", rank);
	MPI_Barrier(comm);
	if (rank == 0) {
		//recieve and print from each other process in order
		for (i = 1; i < p-1; i++) {
			//printf("Rank %d is Receiving from %d \n",rank,i);
			MPI_Recv(buf, blocksize, MPI_CHAR, i, 0, comm, MPI_STATUS_IGNORE);
			for (j = 0; j < blocksize; j++){
				if (!(j%n) && j != 0){
					putchar('\n');
				}
				putchar(buf[j]);
			}
			putchar('\n');
		}
	}
	else{
		// send to rank 0 
		for (i = 0; i < n/(p-1); i++){
			for (j = 0; j < n; j++) {
				buf[i*n + j] = grid[i][j].cur;
			}
		}
		MPI_Send(buf, blocksize, MPI_CHAR, 0, 0, comm);
	}
	return 0;
}

int randNum(){
	int i;
	i = (rand() % BIG_PRIME) + 1;
	// printf("Returning: %d\n",i);
	return i;
}
