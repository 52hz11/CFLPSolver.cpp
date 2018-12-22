#include <iostream>
#include <cstdio>
#include <cstdlib>
#include "CFLPSolver.h"
using namespace std;
int main(){
	FILE *fp = fopen("greedy_output", "w");
	for(int fileNum = 1; fileNum <=71; fileNum++){
		char fileName[50];
		sprintf(fileName,"Instances/p%d", fileNum);
		CFLPSolver s(fileName);
		int cost = s.getFinalCost();
		long duration = s.getDuration();
		fprintf(fp, "%d %ld\n", cost, duration);
	}
}