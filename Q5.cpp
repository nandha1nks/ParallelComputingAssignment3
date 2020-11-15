#include <iostream>
#include <thread>
#include <vector>
#include <random>

const int nthreads = 4;
double sum[nthreads] = { 0 };

int min(int a, int b){
	if(a<b)
		return a;
	return b;
}

void compute_sum(int tid, int &n, std::vector<double>& d)
{
  int st = ((n+nthreads-1) / nthreads) * tid;
  int en = min(((n+nthreads-1) / nthreads) * (tid + 1), n);

  for (int i = st; i < en; i++) {
    sum[tid] += d[i];
  }
}

int main(int argc, char ** argv)
{
	std::vector<std::thread> threads;
	std::vector<double> data;
	
	if(argc < 2){
		std::cout<<"Array size not provided";
		return 0;
	}
	int n;
	n = atoi(argv[1]);

	double lbound = 0;
	double ubound = 10;
	std::uniform_real_distribution<double> urd(lbound, ubound);
	std::default_random_engine re;
	for (int i = 0; i < n; i++)
		data.push_back(urd(re)); //i);
	for (int i = 0; i < nthreads; i++) 
		threads.push_back(std::thread(compute_sum, i,std::ref(n), std::ref(data)));
	for (auto &th : threads) 
		th.join();
	double tsum = 0;
	for (int i = 0; i < nthreads; i++)
		tsum += sum[i];

	std::cout << "Array sum is: " << tsum << std::endl;

	return 0;
}

