#include <iostream>
#include <math.h>
#include <string>
#include <iomanip>
#include <string>
#include <fstream>
#include <vector>
#include <queue>
#include <utility>
#include <algorithm>
using namespace std;
bool sortbysec(const pair<int, int>& a,
    const pair<int, int>& b);
bool sortbysecBust(const pair<int, int>& a,
    const pair<int, int>& b);
class SchedulingAlgorithm
{
    public:
    int no;
    int noP;
    vector<vector<int>> Process;
    vector<int> WT;
    vector<int> TT;
    vector<char> CPUScheduling;//process scheduling trên CPU
    vector<char> Rscheduling;//process scheduling trên R
    static SchedulingAlgorithm* readFile(string filename);
    virtual void processScheduling() = 0;// thực hiện scheduling
    void WriteFile(string filename);
};
class RoundRobin : public SchedulingAlgorithm
{
    public:
    int TimeQuantum;
    RoundRobin();
    void processScheduling();
};
class FCFS : public SchedulingAlgorithm
{
public:
    void processScheduling();
};

class SJF : public SchedulingAlgorithm
{
public:
    void processScheduling();
};

class SRTN : public SchedulingAlgorithm
{
public:
    void processScheduling();
};