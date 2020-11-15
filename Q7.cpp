#include<random>
#include<iostream>
#include<mpi.h>
#include<omp.h>

int min(int a, int b) {
	if(a<b)
		return a;
	return b;
}

int main(int argc, char* argv[]) {
	int rank, np, N;
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
	
	N = (atoi(argv[1])+np-1)/np;
	double a[N], sum = 0;
	int arrSize = atoi(argv[1]);
	//Initialize array in rank 0 and send to all processes
	if(rank == 0) {
		double data[arrSize];
		double lbound = 0;
		double ubound = 10;
		std::uniform_real_distribution<double> urd(lbound, ubound);
		std::default_random_engine re;
		for(int i=0;i<arrSize;i++)
			data[i] = urd(re);
		for(int i=0;i<N;i++)
			a[i] = data[i];
		for(int i=1;i<np;i++){
			MPI_Send(&data[i*N], min(N, arrSize-i*N), MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
		}
	} else {
		MPI_Recv(a, N, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}
	
	
	#pragma omp parallel for reduction (+:sum)
	for(int i=0;i<N;i++)
		sum += a[i];
		
	//Rank 0 getting all sums from other
	if(rank == 0) {
		double oSum;
		for(int i=1;i<np;i++){
			MPI_Recv(&oSum, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			sum += oSum;	
		}
		std::cout<<"Array sum is "<<sum<<std::endl;
	} else {
		MPI_Send(&sum, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
	}
	
	MPI_Finalize();
	return 0;
}
