#include<iostream>
#include<mpi.h>
#include<random>

int min(int a, int b) {
	if(a<b)
		return a;
	return b;
}

int main(int argc, char* argv[]){
	int rank, np, N, arrSize;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &np);
	
	if(argc < 2){
		if(rank == 0) {
			std::cout<<"Array size not provided";
		}
		MPI_Finalize();
		return 0;
	}
	
	N = atoi(argv[1]);
	int n = (N+np-1)/np;
	arrSize = min(n, N-rank*n);
	int a[arrSize];
	
	for(int i=0;i<arrSize;i++)
		a[i] = rank*n + i;
	
	int sum = 0;
	for(int i=0;i<arrSize;i++)
		sum += a[i];
	
	if(rank == 0){
		int lSum;
		for(int i=1;i<np;i++){
			MPI_Recv(&lSum, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			sum += lSum;
		}
		std::cout<<"Sum is "<<sum<<"\n";
	} else {
		MPI_Send(&sum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
	}
	MPI_Finalize();
	return 0;
}
