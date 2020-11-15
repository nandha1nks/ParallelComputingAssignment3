#include<iostream>
#include<mpi.h>
#include<random>

int main(int argc, char* argv[]) {
	int rank, np, N, m;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &np);
	
	if(argc < 3){
		if(rank == 0) {
			std::cout<<"Array size not provided and iteration count not mentioned";
		}
		MPI_Finalize();
		return 0;
	}
	
	N = atoi(argv[1]);
	double a[N];
	m = atoi(argv[2]);
	
	double lbound = 0;
	double ubound = 10;
	std::uniform_real_distribution<double> urd(lbound, ubound);
	std::default_random_engine re;
	for(int i=0;i<N;i++)
		a[i] = urd(re); //rank;
	
	int r1 = 0, r2 = 0;
	while(m--) {
		if(rank == r1){
			r2 = rand() % np;
			MPI_Bcast(&r2, 1, MPI_INT, r1, MPI_COMM_WORLD);
			if(r1 != r2) {
				MPI_Send(a, N, MPI_DOUBLE, r2, 0, MPI_COMM_WORLD);
			}
		} else {
			MPI_Bcast(&r2, 1, MPI_INT, r1, MPI_COMM_WORLD);
			if(r2 == rank) {
				MPI_Recv(a, N, MPI_DOUBLE, r1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			}
		}
		r1 = r2;
	}
	
	if(np>1){
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
	return 0;
}
