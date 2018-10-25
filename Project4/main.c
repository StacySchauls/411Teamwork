#include "pa4.h"


#define n 10
#define B 15
#define A 23
#define P 27211
int main(int argc, char *argv[]){
	int arr[n];
	memset(arr, 0, n);
	serial_baseline(n,A,B,P, arr);
	putchar(10);
	serial_matrix(n,A,B,P, arr);
	return 0;
}
