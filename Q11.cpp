#include <chrono>
#include <iostream>
#include <thread>
#include <vector>
#include <random>
#include <mutex>
#include<mpi.h>
 
const int nthreads = 4;
int barrier_blocked[nthreads] = {0};
std::mutex m;

void iittp_barrier(int tid, int rank, int np) {
	m.lock();
	barrier_blocked[tid] = 1;
	m.unlock();
	
	int c = 0;
	for(int i=0;i<nthreads;i++){
		m.lock();
		if(barrier_blocked[i] == 0) {
			c++;
		}
		m.unlock();
	}
	
	if(c == 0) {
		if(np > 1) {
			int r;
			if(rank == 0){
				MPI_Recv(&r, 1, MPI_INT, rank+1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			} else if(rank == np-1) {
				MPI_Send(&r, 1, MPI_INT, rank-1, 0, MPI_COMM_WORLD);
			} else {
				MPI_Recv(&r, 1, MPI_INT, rank+1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				MPI_Send(&r, 1, MPI_INT, rank-1, 0, MPI_COMM_WORLD);
			}
			MPI_Bcast(&r, 1, MPI_INT, 0, MPI_COMM_WORLD);
		}
		for(int i=0;i<nthreads;i++){
			m.lock();
			barrier_blocked[i] = 0;
			m.unlock();
		}
	}
	
	while(1) {
		int t;
		m.lock();
		t = barrier_blocked[tid];
		m.unlock();
		if(t==0){
			break;
		}
	}
	return;
}

void run_thread(int tid, int &rank, int &np)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(rank*nthreads*1000));
	std::this_thread::sleep_for(std::chrono::milliseconds(tid*1000));
	std::cout<<rank<<" "<<tid<<"\n";
	iittp_barrier(tid, rank, np);
	std::this_thread::sleep_for(std::chrono::milliseconds(tid*1000));
	std::cout<<rank<<" "<<tid<<"\n";
}

int main(int argc, char ** argv)
{
	int rank, np;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &np);
	
	
	std::vector<std::thread> threads;
	for (int i = 0; i < nthreads; i++) 
		threads.push_back(std::thread(run_thread, i, std::ref(rank), std::ref(np)));
	for (auto &th : threads) 
		th.join();
		
	MPI_Finalize();

	return 0;
}

