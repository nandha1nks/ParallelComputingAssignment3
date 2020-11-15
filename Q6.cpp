#include <iostream>
#include <omp.h>
#include <vector>
#include <random>

int main(int argc, char* argv[]){
	
	int n;
	if(argc < 2){
		std::cout<<"Array size not provided";
		return 0;
	}
	
	n = atoi(argv[1]);
	std::vector <double> data;
	double lbound = 0;
	double ubound = 10;
	std::uniform_real_distribution<double> urd(lbound, ubound);
	std::default_random_engine re;
	for (int i = 0; i < n; i++)
		data.push_back(urd(re)); //i);
	
	double sum = 0;
	#pragma omp parallel for reduction (+:sum)
	for(int i=0;i<n;i++)
		sum += data[i];

	std::cout<<"Array sum is "<<sum<<"\n";
	return 0;
}
