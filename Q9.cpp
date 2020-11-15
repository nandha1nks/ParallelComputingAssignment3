#include <iostream>
#include <omp.h>
#include <vector>
#include <random>

int main(int argc, char* argv[]){
	
	if(argc<4) {
		std::cout<<"Matrix sizes not provided\n";
		return 0;
	}
	int m,n,o;
	m= atoi(argv[1]);
	n=atoi(argv[2]);
	o = atoi(argv[3]);
	double a[m][n], b[n][o], c[m][o];
	double lbound = 0;
	double ubound = 10;
	std::uniform_real_distribution<double> urd(lbound, ubound);
	std::default_random_engine re;
	for (int i = 0; i < m; i++)
		for(int j=0;j<n;j++)
			a[i][j] = urd(re);
	for (int i = 0; i<n; i++)
		for(int j=0;j<o;j++)
			b[i][j] = urd(re);
	for (int i = 0; i < m; i++)
		for(int j=0;j<o;j++)
			c[i][j] = 0;
	
	#pragma omp parallel for collapse(2)
	for(int i=0;i<m;i++)
		for(int j=0;j<o;j++)
			for(int k=0;k<n;k++)
				c[i][j] += a[i][k]*b[k][j];

	for (int i = 0; i < m; i++){
		for(int j=0;j<o;j++)
			std::cout<<c[i][j]<<" ";
		std::cout<<"\n";
	}

	return 0;
}
