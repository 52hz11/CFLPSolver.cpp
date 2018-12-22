#include "CFLPSolver.h"
#include <cstring>
#include <cmath>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#define CURRENT_TO_NEW 0
#define NEW_TO_CURRENT 1
using namespace std;
CFLPSolver::CFLPSolver(char *fileName)
{
    int start = clock();
    strcpy(this->fileName, fileName);
    FILE *fp = fopen(fileName, "r+");
    if (!fp)
    {
        printf("Cannot open the file!\n");
        exit(0);
    }
    fscanf(fp, "%d%d", &facilityNum, &customerNum);
    //开空间
    capacity = new int[facilityNum];
    openCost = new int[facilityNum];
    demand = new int[customerNum];
    allocateCost = new int *[facilityNum];
    open = new int[facilityNum];
    allocate = new int[customerNum];
    capacityLeft = new int[facilityNum];
    for(int i = 0; i < facilityNum; i++)
    {
        allocateCost[i] = new int[customerNum];
    }
    //读入
    for(int i = 0; i < facilityNum; i++)
    {
        fscanf(fp, "%d%d", &capacity[i], &openCost[i]);
    }
    for(int i = 0; i < customerNum; i++)
    {
        float tmp;
        fscanf(fp, "%f", &tmp);
        demand[i] = tmp;
    }
    for(int i = 0; i < facilityNum; i++)
    {
        for(int j = 0; j < customerNum; j++)
        {
            float tmp;
            fscanf(fp, "%f", &tmp);
            allocateCost[i][j] = tmp;
        }
    }
    fclose(fp);
    //printInput();
    //初始化
    finalCost = 0;
    for(int i = 0; i < facilityNum; i++)
    {
        open[i] = 0;
        capacityLeft[i] = capacity[i];
    }
    beginGreedy();
    //beginSA();
    outputToFile();
    int end = clock();
    duration = end - start;
}

CFLPSolver::~CFLPSolver()
{
    delete []capacity;
    delete []openCost;
    delete []demand;
    for(int i = 0 ; i < facilityNum; i++)
    {
        delete []allocateCost[i];
    }
    delete []allocateCost;
    delete []open;
    delete []allocate;
}

void CFLPSolver::beginGreedy()
{
    //对每个顾客执行贪心策略
    for(int i = 0; i < customerNum; i++)
    {
        int select = -1;
        int curCost = 99999999;
        //选取分配费用最小的工厂
        for(int j = 0; j < facilityNum; j++)
        {
            if(demand[i] <= capacityLeft[j])
            {
                //int newCost = (1 - open[j]) * openCost[j] + allocateCost[j][i];
                int newCost = allocateCost[j][i];
                if(newCost < curCost)
                {
                    select = j;
                    curCost = newCost;
                }
            }
        }
        //没有工厂可以选取了，说明这个样例的工厂的容量比较紧张，不能用简单的贪心策略做，不过在我测试的71个样例中都没有发现问题
        if(select == -1)
        {
            cout << "greedy failed" << endl;
        }
        //选取工厂，更新工厂的剩余容量，开启被选取的工厂，加上分配的费用
        else
        {
            allocate[i] = select;
            capacityLeft[select] -= demand[i];
            open[select] = 1;
            finalCost += curCost;
        }
    }
    //加上开工厂的费用
    for(int i = 0; i < facilityNum; i++)
    {
        if(open[i])
        {
            finalCost += openCost[i];
        }
    }
}

void CFLPSolver::printInput()
{
    printf("facilityNum: %d, customerNum: %d\n", facilityNum, customerNum);
    for(int i = 0; i < facilityNum; i++)
    {
        printf("facility %d: capacity:%d, openCost:%d\n", i, capacity[i], openCost[i]);
    }
    printf("demand:");
    for(int i = 0; i < customerNum; i++)
    {
        printf("%d, ", demand[i]);
    }
    printf("\nallocate cost: \n");
    for(int i = 0; i < facilityNum; i++)
    {
        for(int j = 0; j < customerNum; j++)
        {
            printf("%d ", allocateCost[i][j]);
        }
        printf("\n");
    }
}

void CFLPSolver::outputToFile()
{
    char outputFile[50];
    string inputFile(fileName);
    inputFile = inputFile.substr(10);
    //sprintf(outputFile, "greedy/%s_result", inputFile.c_str());
    sprintf(outputFile, "SA/%s_result", inputFile.c_str());
    FILE *fp = fopen(outputFile, "w");
    if (!fp)
    {
        printf("Cannot open the file!\n");
        exit(0);
    }
    fprintf(fp, "%d\n", finalCost);
    for(int i = 0; i < facilityNum; i++)
    {
        fprintf(fp, "%d ", open[i]);
    }
    fprintf(fp, "\n");
    for(int i = 0; i < customerNum; i++)
    {
        fprintf(fp, "%d ", allocate[i]);
    }
    fprintf(fp, "\n");
    fclose(fp);
    cout<<"finish :"<<inputFile.c_str()<<endl;
}

void CFLPSolver::beginSA()
{
    //初始化解和分配空间
    initSA();
    //初温
    int T = 290000;
    //外循环结束条件
    while(T > 0.000000001)
    {
        //内循环
        for(int i = 0; i < 1500; i++)
        {
            //把当前解复制一份到新解的空间，便于生成新解
            copySolution(CURRENT_TO_NEW);
            //找到新解
            getNewSolution();
            //找到更好的解直接采纳
            if(newCost < finalCost)
            {
                copySolution(NEW_TO_CURRENT);
            }
            //找到更差的解按概率采纳
            else
            {
                double delta = newCost - finalCost;
                double probility = exp(-delta / T);
                if(probility > (rand() % 1000) / 1000.0)
                {
                    copySolution(NEW_TO_CURRENT);
                }
            }
        }
        T *= 0.98;
    }
    delete[] newOpen;
    delete[] newAllocate;
    delete[] newCapacityLeft;
}
void CFLPSolver::initSA()
{
    srand((unsigned int)time(NULL));
    //给特有的变量分配空间
    newOpen = new int[facilityNum];
    newAllocate = new int[customerNum];
    newCapacityLeft = new int[facilityNum];
    //初始化出一个解，采取直接暴力分配的策略，容量够就直接分配
    for(int i = 0; i < customerNum; i++)
    {
        int select = -1;
        for(int j = 0; j < facilityNum; j++)
        {
            if(demand[i] <= capacityLeft[j])
            {
                select = j;
                break;
            }
        }
        if(select == -1)
        {
            cout << "init failed" << endl;
        }
        else
        {
            allocate[i] = select;
            capacityLeft[select] -= demand[i];
            open[select] = 1;
            finalCost += allocateCost[select][i];
        }
    }
    for(int i = 0; i < facilityNum; i++)
    {
        if(open[i])
        {
            finalCost += openCost[i];
        }
    }
}
//复制解的辅助函数
void CFLPSolver::copySolution(int flag)
{
    if(flag == CURRENT_TO_NEW)
    {
        for(int i = 0; i < facilityNum; i++)
        {
            newOpen[i] = open[i];
            newCapacityLeft[i] = capacityLeft[i];
        }
        for(int i = 0; i < customerNum; i++)
        {
            newAllocate[i] = allocate[i];
        }
        newCost = finalCost;
    }
    else if(flag == NEW_TO_CURRENT)
    {
        for(int i = 0; i < facilityNum; i++)
        {
            open[i] = newOpen[i];
            capacityLeft[i] = newCapacityLeft[i];
        }
        for(int i = 0; i < customerNum; i++)
        {
            allocate[i] = newAllocate[i];
        }
        finalCost = newCost;
    }
}
//生成一个新解
void CFLPSolver::getNewSolution()
{
    int method = rand() % 2;
    //交换两个顾客所属工厂
    if(method == 0)
    {
        //25->1 30->2,
        int select_1 = rand() % customerNum;
        int select_2 = rand() % customerNum;
        //如果两个顾客已经在同一个工厂，或者是交换后会超出容量，重新选取顾客再来进行交换
        while(newAllocate[select_1] == newAllocate[select_2] ||
                (newCapacityLeft[newAllocate[select_2]] + demand[select_2]) < demand[select_1] ||
                (newCapacityLeft[newAllocate[select_1]] + demand[select_1]) < demand[select_2])
        {
            select_1 = rand() % customerNum;
            select_2 = rand() % customerNum;
        }
        //交换两个顾客选取的工厂
        int tmp = newAllocate[select_1];
        newAllocate[select_1] = newAllocate[select_2];
        newAllocate[select_2] = tmp;
    }
    //随机选一个顾客，改变其工厂
    else if(method == 1){
    	int select = rand() % customerNum;
    	vector<int> validFacility;
    	for(int i = 0; i < facilityNum; i++){
    		if(i != newAllocate[select] && newCapacityLeft[i] >= demand[select]){
    			validFacility.push_back(i);
    		}
    	}
    	if(validFacility.empty()){
    		cout<<"error"<<endl;
    		return;
    	}
    	newAllocate[select] = validFacility[rand() % validFacility.size()];
    }
    //更新其他信息
    reEvaluate();
}
//用顾客分配的工厂来重新生成其他信息
void CFLPSolver::reEvaluate(){
	for(int i = 0; i < facilityNum; i++){
		newOpen[i] = 0;
		newCapacityLeft[i] = capacity[i];
	}
	newCost = 0;
	for(int i = 0; i < customerNum; i++){
		newOpen[newAllocate[i]] = 1;
		newCost += allocateCost[newAllocate[i]][i];
		newCapacityLeft[newAllocate[i]] -= demand[i];
	}
	for(int i = 0; i < facilityNum; i++){
		if(newOpen[i]){
			newCost += openCost[i];
		}
	}
}

int CFLPSolver::getFinalCost(){
    return finalCost;
}

long CFLPSolver::getDuration(){
    return duration;
}