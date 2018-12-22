# 用贪心算法和模拟退货法解决CFLP问题

首先问题描述如下：

给定n个工厂和m个顾客，开工厂需要一定的费用，一个工厂有一定的容量限制，每个顾客也有一定的需求，而每个顾客要选取某个工厂也需要一定的分配费用，现在要求找出一个分配方案，把顾客分配给不同的工厂，然后在可以满足所有顾客需求的前提下让所有的花费（开工厂的花费和分配的花费）最小。

这显然是一个NP-hard问题，因为情况数非常的多，而且也没有什么固定的好的策略，所以现在我们就用贪心算法和模拟退火法两种方法来解决这个问题。
## 一、贪心算法
这个问题如果用贪心算法做的话思路应该是很简单的，就直接一个顾客一个顾客这样来选择工厂，然后在每次的选取过程中保证这个顾客的花费最小即可，但是这样做的话就会带来一个问题，就是开工厂的费用问题，如果把开工厂的费用都归到一个顾客的花费上的话，那么这个顾客要选取一个新工厂的花费就会很高，那么如果每个顾客采取这个策略就会都不愿意开新工厂，那么实际上就会去选取那些已经被其他顾客开的工厂。但是实际上开工厂的钱应该是要平摊到选取了这个工厂的所有顾客头上的，因此这样的策略显然不是太好。所以为了优化这个问题，我们可以在贪心的过程中直接不考虑开工厂的价钱（虽然这样做其实也没有符合很精确的开工厂的钱的平摊，但是因为在遍历过程中我们没办法确认后面的顾客会怎么选取工厂，所以只能退而求其次），然后在最后再看哪些工厂被选取了，然后再加上开这部分工厂的价钱即可，所以贪心部分的代码如下：
```c++
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
```
而对样例的结果测试如下（这里只列出最终的cost和运行时间，具体的结果见文末的github连接）：
| instance | cost  | time(ms) |
|----------|-------|----------|
| p1       | 9440  | 9        |
| p2       | 8126  | 7        |
| p3       | 10126 | 10       |
| p4       | 12126 | 6        |
| p5       | 9375  | 15       |
| p6       | 8061  | 0        |
| p7       | 10061 | 16       |
| p8       | 12061 | 0        |
| p9       | 9040  | 15       |
| p10      | 7726  | 16       |
| p11      | 9726  | 0        |
| p12      | 11726 | 21       |
| p13      | 12032 | 14       |
| p14      | 9180  | 3        |
| p15      | 13180 | 16       |
| p16      | 17180 | 15       |
| p17      | 12032 | 16       |
| p18      | 9180  | 16       |
| p19      | 13180 | 26       |
| p20      | 17180 | 12       |
| p21      | 12032 | 0        |
| p22      | 9180  | 16       |
| p23      | 13180 | 15       |
| p24      | 17180 | 22       |
| p25      | 19197 | 47       |
| p26      | 16131 | 47       |
| p27      | 21531 | 47       |
| p28      | 26931 | 69       |
| p29      | 19305 | 48       |
| p30      | 16239 | 62       |
| p31      | 21639 | 39       |
| p32      | 27039 | 67       |
| p33      | 19055 | 62       |
| p34      | 15989 | 42       |
| p35      | 21389 | 62       |
| p36      | 26789 | 47       |
| p37      | 19055 | 57       |
| p38      | 15989 | 50       |
| p39      | 21389 | 63       |
| p40      | 26789 | 62       |
| p41      | 7226  | 16       |
| p42      | 9957  | 31       |
| p43      | 12448 | 31       |
| p44      | 7585  | 16       |
| p45      | 9848  | 16       |
| p46      | 12639 | 42       |
| p47      | 6634  | 16       |
| p48      | 9044  | 31       |
| p49      | 12420 | 32       |
| p50      | 10062 | 15       |
| p51      | 11351 | 16       |
| p52      | 10364 | 15       |
| p53      | 12470 | 32       |
| p54      | 10351 | 0        |
| p55      | 11970 | 24       |
| p56      | 23882 | 52       |
| p57      | 32882 | 79       |
| p58      | 53882 | 62       |
| p59      | 39121 | 62       |
| p60      | 23882 | 84       |
| p61      | 32882 | 71       |
| p62      | 53882 | 74       |
| p63      | 39121 | 69       |
| p64      | 23882 | 72       |
| p65      | 32882 | 69       |
| p66      | 53882 | 68       |
| p67      | 39671 | 73       |
| p68      | 23882 | 115      |
| p69      | 32882 | 114      |
| p70      | 53882 | 80       |
| p71      | 39121 | 73       |
可以看到效果其实一般般，第一个样例我所查到的最优解应该是在八千多，但是用贪心却达到了9400，但是这个算法的运行效率是很高的，如果对于一些实时系统应该还是可以得到应用的，如果用其他的算法的话都比较难达到这个效率，复杂度为O(mn)

## 二、模拟退火法
所以为了达到更好的解，我们可以考虑使用模拟退火法，模拟退火的思路其实很简单，就是先生成一个解，然后用这个解去不断生成更好的解，然后不断逼近最优解，而这种思路其实和局部搜索法是很类似的，就是不断去搜索更好的解，然后只采纳更好的解，这种方法也被很形象的成为是爬山法，但是局部搜索的方法会很容易陷入局部最优解，而为了改善这个问题，模拟退火法在局部搜索法的基础上加上了退火的策略，就是在找到更好的解的时候直接采纳，找到更差的解的时候按概率采纳，而整个接收更差的解的概率和温度有关系，温度越高接收差解的概率越高，温度越低接收差解的概率越低，而只要我们不断的降温，最后就差不多是局部搜索策略了。模拟退火法的优越性体现在其在陷入局部最优解时是有一定概率去接收更差的解的，这样就有机会去跳出局部最优解，然后继续去逼近整体最优解。

回到我们的问题，我们的解的表示方法很简单，就是每个顾客分别选取了哪个工厂，其他的信息，比如哪个工厂开了，以及最终的花费都可以用这个方法计算出来，而要产生新解，我采取了两种办法，一种是找到可以交换工厂的两个顾客，然后交换他们所选取的工厂，第二就是随机选取一个顾客，然后改变他的工厂。所以具体的相关代码实现如下：
```c++
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
```

而对样例的结果测试如下（这里只列出最终的cost和运行时间，具体的结果见文末的github连接）：
| instance | cost  | time(ms) |
|----------|-------|----------|
| p1       | 9003  | 16713    |
| p2       | 7942  | 20581    |
| p3       | 9588  | 23464    |
| p4       | 11647 | 29037    |
| p5       | 9170  | 26883    |
| p6       | 7996  | 26107    |
| p7       | 9903  | 26515    |
| p8       | 11685 | 26817    |
| p9       | 8582  | 33358    |
| p10      | 7939  | 32361    |
| p11      | 9632  | 32596    |
| p12      | 10469 | 32784    |
| p13      | 8971  | 39347    |
| p14      | 7741  | 37907    |
| p15      | 9377  | 40642    |
| p16      | 11422 | 39898    |
| p17      | 8895  | 40509    |
| p18      | 7516  | 40054    |
| p19      | 9963  | 39870    |
| p20      | 12184 | 41256    |
| p21      | 8913  | 40438    |
| p22      | 8277  | 33460    |
| p23      | 9333  | 23349    |
| p24      | 11259 | 23013    |
| p25      | 13919 | 24538    |
| p26      | 13107 | 24577    |
| p27      | 14619 | 24388    |
| p28      | 17524 | 24679    |
| p29      | 15740 | 29986    |
| p30      | 12812 | 30608    |
| p31      | 16229 | 44399    |
| p32      | 18305 | 43838    |
| p33      | 14362 | 43200    |
| p34      | 13546 | 41222    |
| p35      | 14597 | 44140    |
| p36      | 17333 | 44326    |
| p37      | 12747 | 45003    |
| p38      | 11662 | 42988    |
| p39      | 14682 | 43784    |
| p40      | 16900 | 43997    |
| p41      | 7108  | 31173    |
| p42      | 7324  | 42936    |
| p43      | 6773  | 42007    |
| p44      | 7516  | 30162    |
| p45      | 7573  | 42037    |
| p46      | 7085  | 42379    |
| p47      | 6347  | 29808    |
| p48      | 6234  | 35578    |
| p49      | 6837  | 24464    |
| p50      | 9349  | 18092    |
| p51      | 8828  | 23690    |
| p52      | 9686  | 17729    |
| p53      | 9692  | 22414    |
| p54      | 9658  | 15597    |
| p55      | 10053 | 26636    |
| p56      | 24988 | 24469    |
| p57      | 32157 | 23464    |
| p58      | 48956 | 23326    |
| p59      | 37202 | 23121    |
| p60      | 23493 | 23386    |
| p61      | 30637 | 23928    |
| p62      | 47824 | 24076    |
| p63      | 33046 | 23258    |
| p64      | 25025 | 23661    |
| p65      | 32541 | 23937    |
| p66      | 49256 | 23989    |
| p67      | 38329 | 23407    |
| p68      | 24241 | 22882    |
| p69      | 31896 | 23381    |
| p70      | 51170 | 23529    |
| p71      | 35199 | 22955    |

对比两种方式的结果后我们可以发现模拟退火的结果比用贪心算法的结果基本都是要好的，而且和网上找到的最优解的误差也可以控制在10%以内，不过美中不足的是样例的运行时间确实有点长，不过其实要解决这一点也比较简单，只要降低初温和减少内循环次数即可，运行时间就可以成倍的加快，不过为了尽可能找到更好的解，我还是没有去这样做，如果有实际使用的需求，就可以通过对解的要求程度和可以接受的程序运行程度之间做一个平衡，这个要根据具体需求而定。不过需要注意的是模拟退火算法是一个随机的算法，所以每次的结果实际上都会不一样，比如第一个样例在我某一次运行时求到的cost是9004，但是你多运行几次实际上最好的时候是可以跑到8700左右的，所以模拟退火算法的结果始终是存在一个浮动的。