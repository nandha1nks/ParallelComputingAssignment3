#include<iostream>
#include<mpi.h>
#include<random>

int main(int argc, char* argv[]){
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
	
	N = atoi(argv[1]);
	double a[N];
	double lbound = 0;
	double ubound = 10;
	std::uniform_real_distribution<double> urd(lbound, ubound);
	std::default_random_engine re;
	for(int i=0;i<N;i++) {
		a[i] = urd(re); //rank;
	}
	if(np == 1) {
		std::cout<<"No process to share data\n";
	}
	else if(rank == 0 && np > rank + 1) {
		int r = 0;
		std::cout<<"Data before transaction:\n";
		std::cout<<"data in process "<<rank<<"\n";
		for(int i=0;i<N;i++)
			std::cout<<a[i]<<" "; 
		std::cout<<"\n";
		MPI_Send(&r, 1, MPI_INT, rank+1, 0, MPI_COMM_WORLD);
	} else {
		int r = 0;
		MPI_Recv(&r, 1, MPI_INT, rank-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		std::cout<<"data in process "<<rank<<"\n";
		for(int i=0;i<N;i++)
			std::cout<<a[i]<<" "; 
		std::cout<<"\n";
		if(rank+1<np)
			MPI_Send(&r, 1, MPI_INT, (rank+1)%np, 0, MPI_COMM_WORLD);
	}
	
	if(np>1){
		MPI_Sendrecv_replace(a, N, MPI_DOUBLE, (rank+1)%np, 0, (rank+np-1)%np, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		if(rank == 0)
			std::cout<<"Data after transaction:\n";
		int r = 0;
		if(rank!=0)
			MPI_Recv(&r, 1, MPI_INT, (rank+np-1)%np, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		std::cout<<"data in process "<<rank<<"\n";
		for(int i=0;i<N;i++)
			std::cout<<a[i]<<" "; 
		std::cout<<"\n";
		if(np > rank + 1)
			MPI_Send(&r, 1, MPI_INT, rank+1, 0, MPI_COMM_WORLD);
	}
	
	MPI_Finalize();
}
