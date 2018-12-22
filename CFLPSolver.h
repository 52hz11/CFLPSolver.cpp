#ifndef _GREEDY_
#define _GREEDY_
#include <cstdio>
#include <cstdlib>
#include <iostream>
using namespace std;
class CFLPSolver{
public:
	CFLPSolver(char* fileName);
	int getFinalCost();
	long getDuration();
	~CFLPSolver();
private:
	void printInput();
	void outputToFile();
	void beginGreedy();
	void beginSA();
	void initSA();
	void copySolution(int flag);
	void getNewSolution();
	void reEvaluate();
	char fileName[50];
	long duration;
	//input
	int facilityNum;
	int customerNum;
	int* capacity;
	int* openCost;
	int* demand;
	int** allocateCost;
	//answer
	long finalCost;
	int* open;
	int* allocate;
	int* capacityLeft;
	//SA特有
	long newCost;
	int* newOpen;
	int* newAllocate;
	int* newCapacityLeft;
};
#endif