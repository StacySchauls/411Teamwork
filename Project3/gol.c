#include "gol.h"
#include "util.h"
extern int n, G;     //#rows and #col  //# of generatioons
extern int p;   //number of processors and process id

CELL **generateInitialGoL(int rank){
	int j;
	int rNum,i,k; // <-- potential error spot

	CELL **my_grid = (CELL **)malloc((n/p)*sizeof(CELL*));
	for(j = 0; j<(n/p); j++){
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
		rNum = randNum();//This one is for rank 0 to use
	}
	else { // workers 1 :: p-1
		//just recieve
		MPI_Recv(&rNum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}
	srand(rNum);//every process does this
	for(i = 0; i<(n/p); i++){
		for(k = 0; k<n; k++){
			if(randNum() %2 == 0){
				my_grid[i][k].old = 'x';
				my_grid[i][k].cur = 'x';// set both to alive
			}
			else {
				my_grid[i][k].old = '.';
				my_grid[i][k].cur = '.';
			}
		}
	}
	return my_grid;
}

int simulate(CELL **grid, MPI_Comm comm, int rank){
  struct timeval tv1,tv2,tv3,tv4;
 double  total, total2;
total = 0;
	int i, j = 0;
	Runtime sim_time;
	Runtime gen_time;
	//Runtime blah_time;
  
	//int blah_time_time =NULL;
	char buf[n], bufBelow[n], bufAbove[n];
	gettimeofday(&sim_time.t1, NULL);
	memset(buf, 0, n);
	memset(bufBelow, 0, n);
	memset(bufAbove, 0, n);

	while (j < G){
		gettimeofday(&gen_time.t1, NULL);
		for(i = 0; i<n; i++){
			buf[i] = grid[0][i].old;
		}
gettimeofday(&tv1,NULL);
		if(rank != 0){
			MPI_Send(buf, n, MPI_CHAR, rank-1,0,comm);
		}else{
			MPI_Send(buf, n, MPI_CHAR, p-1,0,comm);
		}

		if(rank == p-1){
			MPI_Recv(bufBelow, n, MPI_CHAR, 0, 0, comm, MPI_STATUS_IGNORE);
		}else{
			MPI_Recv(bufBelow, n, MPI_CHAR, rank+1, 0, comm, MPI_STATUS_IGNORE);
		}

		if(rank == p-1){
			MPI_Send(buf, n, MPI_CHAR, 0,0,comm);
		}else{
			MPI_Send(buf, n, MPI_CHAR, rank+1,0,comm);
		}

		if(rank == 0){
			MPI_Recv(bufAbove, n, MPI_CHAR, p-1,0,comm, MPI_STATUS_IGNORE);
		}else{
			MPI_Recv(bufAbove, n, MPI_CHAR, rank-1,0,comm, MPI_STATUS_IGNORE);
		}
  gettimeofday(&tv2, NULL);
		determineState(grid, rank,bufAbove,0);
		for(i = 0; i<(n/(p) - 1); i++){
			determineState(grid,rank,bufBelow,i);
		}
		j++;
		gettimeofday(&gen_time.t2, NULL);
total = total + (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 + (double) (tv2.tv_sec - tv1.tv_sec);
}
	gettimeofday(&sim_time.t2, NULL);
	int sim_avg = timeToMicroSec(&sim_time);
	int * sim_avgs = NULL;
	if (rank == 0){
		sim_avgs = (int*)malloc(sizeof(int) *p);
	}
gettimeofday(&tv3, NULL);
	MPI_Gather(&sim_avg, 1, MPI_INT, sim_avgs, 1 , MPI_INT, 0, comm);
gettimeofday(&tv4, NULL);
total = total + (double) (tv4.tv_usec - tv3.tv_usec) / 1000000 + (double) (tv4.tv_sec - tv3.tv_sec);
  if (rank == 0){
		
		int sum = 0;
		int *start;
		start = sim_avgs;
		for (i = 0; i < p; i++){
			sum += *start;
			start++;
		}
		sim_avg = (int)(sum/(double)p);
		printf("%d, %d, %d, %d\n",sim_avg,n,G,p);
    printf("Total com time waas: %f\n",total);
}


	return 0;
}




int determineState(CELL **grid, int rank, char buf[], int row){//updates a full row
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

		if (num_alive > 2 && num_alive < 6) { //we are alive
			grid[row][col].old = grid[row][col].cur;
			grid[row][col].cur = 'x';

		}
		else{
			grid[row][col].old = grid[row][col].cur;
			grid[row][col].cur = '.';
		}
	}
	return 0;
}

int displayGoL(CELL **grid, MPI_Comm comm, int rank){
	int i = 0;
	int j = 0;
	int blocksize = (int)(((n*n)/(p-1)));
	char *board = (char *)calloc(n*n, sizeof(char));

	char buf[blocksize*p];
	memset(buf, 0, blocksize*p);
	for (i = 0; i < n/p; i++){
		for (j = 0; j < n; j++) {
			buf[i*n + j] = grid[i][j].cur;
		}
	}
	MPI_Gather(board, blocksize, MPI_CHAR, board, blocksize, MPI_CHAR, 0, comm);
	if (rank == 0){
		for (i = 0; i < p; i++) {
			for (j = 0; j < blocksize; j++){
				if (!(j%n) && j != 0){
					putchar('\n');
				}
				putchar(*(board + i*n + j));
			}
			putchar('\n');
		}
	}
	return 0;
}

int randNum(){
	int i;
	i = (rand() % BIG_PRIME) + 1;
	return i;
}
