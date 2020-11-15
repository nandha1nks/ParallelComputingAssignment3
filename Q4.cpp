#include<iostream>
#include<mpi.h>
#include<random>

int min(int a, int b) {
	if(a<b)
		return a;
	return b;
}

int main(int argc, char* argv[]){
	int rank, np, N, arrSize, b;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &np);
	
	if(argc < 3){
		if(rank == 0) {
			std::cout<<"Array size not provided or the parameter to decide bock or cyclic is not provided";
		}
		MPI_Finalize();
		return 0;
	}
	
	N = atoi(argv[1]);
	b = atoi(argv[2]);
	int n = (N+np-1)/np;
	int a[n];
	
	if(b==0) {
		arrSize = min(n, N-rank*n);
		for(int i=0;i<arrSize;i++)
			a[i] = rank*n + i;
	} else if(b==1) {
		arrSize = 0;
		for(int i=rank;i<N;i+=np)
			a[arrSize++] = i;
	} else {
		int blockSize = -1*min(-2, -1*n);
		arrSize = 0;
		for(int i=blockSize*rank;i<N;i+=np*blockSize)
			for(int j=i;j<N&&j<(rank+1)*blockSize;j++)
				a[arrSize++] = j;
	}
	
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
