#include "header.h"
bool sortbysec(const pair<int, int>& a,
    const pair<int, int>& b)
{
    return (a.second < b.second);
}
bool sortbysecBust(const pair<int, int>& a,
    const pair<int, int>& b)
{
    return (a.second < b.second);
}
SchedulingAlgorithm* SchedulingAlgorithm::readFile(string filename)
{
    ifstream f;
    int n;
    f.open(filename, ios::in);
    f >> n;
    if(n == 2)
    {
        RoundRobin * temp = new RoundRobin;
        temp->no = n;
        f >> temp->TimeQuantum;
        f >> temp->noP;
        vector<int> i;
        while(!f.eof())
        {
            f >> n;
            i.push_back(n);
            if(f.get() == '\n' || f.eof())
            {
                temp->Process.push_back(i);
                i.clear();
            }
        }
        f.close();
        for (int i = 0; i < temp->noP; i++)
        {
            temp->WT.push_back(0);
        }
        for (int i = 0; i < temp->noP; i++)
        {
            temp->TT.push_back(0);
        }
        return temp;
    }
    else if(n == 1)
    {
        FCFS* temp = new FCFS;
        temp->no = n;
        f >> temp->noP;
        vector<int> i;
        while(!f.eof())
        {
            f >> n;
            i.push_back(n);
            if(f.get() == '\n' || f.eof())
            {
                temp->Process.push_back(i);
                i.clear();
            }
        }
        f.close();
        for (int i = 0; i < temp->noP; i++)
        {
            temp->WT.push_back(0);
        }
        for (int i = 0; i < temp->noP; i++)
        {
            temp->TT.push_back(0);
        }
        return temp;
    }
    else if (n == 3)
    {
        SJF* temp = new SJF;
        temp->no = n;
        f >> temp->noP;
        vector<int> i;
        while (!f.eof())
        {
            f >> n;
            i.push_back(n);
            if (f.get() == '\n' || f.eof())
            {
                temp->Process.push_back(i);
                i.clear();
            }
        }
        f.close();
        for (int i = 0; i < temp->noP; i++)
        {
            temp->WT.push_back(0);
        }
        for (int i = 0; i < temp->noP; i++)
        {
            temp->TT.push_back(0);
        }
        return temp;
    }
    else if (n == 4)
    {
        SRTN* temp = new SRTN;
        temp->no = n;
        f >> temp->noP;
        vector<int> i;
        while (!f.eof())
        {
            f >> n;
            i.push_back(n);
            if (f.get() == '\n' || f.eof())
            {
                temp->Process.push_back(i);
                i.clear();
            }
        }
        f.close();
        for (int i = 0; i < temp->noP; i++)
        {
            temp->WT.push_back(0);
        }
        for (int i = 0; i < temp->noP; i++)
        {
            temp->TT.push_back(0);
        }
        return temp;
    }
    return NULL;
}
void SchedulingAlgorithm::WriteFile(string filename)
{
    ofstream f;
    f.open(filename, ios::out);
    for (int i = 0; i < this->CPUScheduling.size(); i++)
    {
        f << CPUScheduling[i];
    }
    f << endl;
    for (int i = 0; i < this->Rscheduling.size(); i++)
    {
        f << Rscheduling[i];
    }
    f << endl;
    for (int i = 0; i < this->noP; i++)
    {
        f << TT[i] << " ";
    }
    f << endl;
    for (int i = 0; i < this->noP; i++)
    {
        f << WT[i] << " ";
    }
    f.close();
}
//void SchedulingAlgorithm::print()
//{
//    cout << no << endl << noP << endl;
//    for (int i = 0; i < Process.size(); i++)
//    {
//        for (int k = 0; k < Process[i].size(); k++)
//        {
//            cout << Process[i][k] << " ";
//        }
//        cout << endl;
//    }
//}

RoundRobin::RoundRobin()
{
    TimeQuantum = 0;
}
void RoundRobin::processScheduling()
{
    vector<pair<int, int>> SortArrivalTime;
    for (int i = 0; i < this->noP; i++)
    {
        pair<int, int> k = { i, this->Process[i][0] };
        SortArrivalTime.push_back(k);
    }
    //sort arrival time của mỗi process để xem process nào đến trước
    sort(SortArrivalTime.begin(), SortArrivalTime.end(), sortbysec);
    queue<int> cpu;//chứa các Process chờ sử dụng cpu
    queue<int> R;//chứa các process chờ R
    queue<int> copy_cpu;
    vector<int> nextStateProcess;//chứa các state của process(vd: nextStateProcess[0] = 1, tức là process đầu tiên (thứ 0) đang ở state 1
    //(tức là sẽ sử dụng cpu với giá trị burst time 1: CPU Burst time 1), nếu giá trị là chẵn sẽ là chờ R(vd: 2 => R 2))
    queue<pair<int, int>> orderProcess;//chứa các process đang đợi tới lượt thêm vào queue cpu theo thứ tự arrival time

    // Khởi tạo state của các process đều là 1 (tức là đang chuẩn bị sử dụng cpu với burst time là cpu burst time 1)
    for (int i = 0; i < this->noP; i++)
    {
        nextStateProcess.push_back(1);
    }
    cpu.push(SortArrivalTime[0].first);
    for (int i = 1; i < SortArrivalTime.size(); i++)
    {
        if (SortArrivalTime[i].second == SortArrivalTime[0].second)
        {
            cpu.push(SortArrivalTime[i].first);
        }
        else
        {
            orderProcess.push(SortArrivalTime[i]);
        }
    }
    int i = SortArrivalTime[0].second;//biến thời gian, i sẽ là mỗi giây tính từ thời gian của process đến đầu tiên
    int currentPCpu = 0;// cho biết Process nào đang sử dụng cpu
    int currentPR = 0;// cho biết Process nào đang sử dụng R
    int CountTimeQuantum = TimeQuantum;
    while (!cpu.empty() || !R.empty() || !orderProcess.empty())
    {
        if (cpu.size() > 0)
        {
            currentPCpu = cpu.front();
            CPUScheduling.push_back(char(currentPCpu + 49));
            CountTimeQuantum--;
        }
        else
        {
            CPUScheduling.push_back('_');
            currentPCpu = -1;
        }

        if (R.size() > 0)
        {
            currentPR = R.front();
            Rscheduling.push_back(char(currentPR + 49));
        }
        else
        {
            Rscheduling.push_back('_');
            currentPR = -1;
        }
        copy_cpu = cpu;
        while (!copy_cpu.empty()) {
            if (copy_cpu.front() != currentPCpu)
            {
                WT[copy_cpu.front()]++;
            }
            copy_cpu.pop();
        }
        i++;
        if (orderProcess.size() > 0)
        {
            if (orderProcess.front().second == i)
            {
                cpu.push(orderProcess.front().first);
                orderProcess.pop();
            }
        }
        if (currentPCpu != -1)
        {
            this->Process[currentPCpu][nextStateProcess[currentPCpu]]--;
            if (this->Process[currentPCpu][nextStateProcess[currentPCpu]] == 0)
            {
                if (Process[currentPCpu].size() - 1 > nextStateProcess[currentPCpu])
                {
                    nextStateProcess[currentPCpu]++;
                    R.push(currentPCpu);
                    cpu.pop();
                }
                else {
                    cpu.pop();
                    TT[currentPCpu] = i - this->Process[currentPCpu][0];
                }
                CountTimeQuantum = TimeQuantum;
            }
            else if (CountTimeQuantum == 0)
            {
                cpu.push(cpu.front());
                cpu.pop();
                CountTimeQuantum = TimeQuantum;
            }
        }
        if (currentPR != -1)
        {
            this->Process[currentPR][nextStateProcess[currentPR]]--;
            if (this->Process[currentPR][nextStateProcess[currentPR]] == 0)
            {
                if (Process[currentPR].size() - 1 > nextStateProcess[currentPR])
                {
                    nextStateProcess[currentPR]++;
                    cpu.push(currentPR);
                    R.pop();
                }
                else {
                    R.pop();
                    TT[currentPR] = i - this->Process[currentPR][0];
                }
            }
        }
    }
    for (int i = 0; i < this->WT.size(); i++)
    {
        cout << WT[i] << " ";
    }
    cout << endl;
    for (int i = 0; i < this->TT.size(); i++)
    {
        cout << TT[i] << " ";
    }
    cout << endl;
    for (int i = 0; i < CPUScheduling.size(); i++)
    {
        cout << CPUScheduling[i];
    }
    cout << endl;
    for (int i = 0; i < Rscheduling.size(); i++)
    {
        cout << Rscheduling[i];
    }
}
void FCFS::processScheduling()
{
    vector<pair<int, int>> SortArrivalTime;
    for (int i = 0; i < this->noP; i++)
    {
        pair<int, int> k = { i, this->Process[i][0] };
        SortArrivalTime.push_back(k);
    }
    //sort arrival time của mỗi process để xem process nào đến trước
    sort(SortArrivalTime.begin(), SortArrivalTime.end(), sortbysec);
    queue<int> cpu;
    queue<int> R;
    queue<int> copy_cpu;
    vector<int> nextStateProcess;
    queue<pair<int, int>> orderProcess;
    for (int i = 0; i < this->noP; i++)
    {
        nextStateProcess.push_back(1);
    }
    cpu.push(SortArrivalTime[0].first);
    for (int i = 1; i < SortArrivalTime.size(); i++)
    {
        if (SortArrivalTime[i].second == SortArrivalTime[0].second)
        {
            cpu.push(SortArrivalTime[i].first);
        }
        else
        {
            orderProcess.push(SortArrivalTime[i]);
        }
    }
    int i = SortArrivalTime[0].second;
    int currentPCpu = 0;
    int currentPR = 0;
    while (!cpu.empty() || !R.empty() || !orderProcess.empty())
    {
        if (cpu.size() > 0)
        {
            currentPCpu = cpu.front();
            CPUScheduling.push_back(char(currentPCpu + 49));
        }
        else
        {
            CPUScheduling.push_back('_');
            currentPCpu = -1;
        }
        
        if (R.size() > 0)
        {
            currentPR = R.front();
            Rscheduling.push_back(char(currentPR + 49));
        }
        else
        {
            Rscheduling.push_back('_');
            currentPR = -1;
        }
        copy_cpu = cpu;
        while (!copy_cpu.empty()) {
            if (copy_cpu.front() != currentPCpu)
            {
                WT[copy_cpu.front()]++;
            }
            copy_cpu.pop();
        }
        i++;//i = 6
        if (orderProcess.size() > 0)
        {
            if (orderProcess.front().second == i)
            {
                cpu.push(orderProcess.front().first);
                orderProcess.pop();
            }
        }
        if (currentPCpu != -1)
        {
            this->Process[currentPCpu][nextStateProcess[currentPCpu]]--;
            if (this->Process[currentPCpu][nextStateProcess[currentPCpu]] == 0)
            {
                if (Process[currentPCpu].size() - 1 > nextStateProcess[currentPCpu])
                {
                    nextStateProcess[currentPCpu]++;
                    R.push(currentPCpu);
                    cpu.pop();
                }
                else {
                    cpu.pop();
                    TT[currentPCpu] = i - this->Process[currentPCpu][0];
                }
            }
        }
        if (currentPR != -1)
        {
            this->Process[currentPR][nextStateProcess[currentPR]]--;
            if (this->Process[currentPR][nextStateProcess[currentPR]] == 0)
            {
                if (Process[currentPR].size() - 1 > nextStateProcess[currentPR])
                {
                    nextStateProcess[currentPR]++;
                    cpu.push(currentPR);
                    R.pop();
                }
                else {
                    R.pop();
                    TT[currentPR] = i - this->Process[currentPR][0];
                }
            }
        }
    }
    cout << i << endl;
    for (int i = 0; i < this->WT.size(); i++)
    {
        cout << WT[i] << " ";
    }
    cout << endl;
    for (int i = 0; i < this->TT.size(); i++)
    {
        cout << TT[i] << " ";
    }
    cout << endl;
    for (int i = 0; i < CPUScheduling.size(); i++)
    {
        cout << CPUScheduling[i];
    }
    cout << endl;
    for (int i = 0; i < Rscheduling.size(); i++)
    {
        cout << Rscheduling[i];
    }

}
void SJF::processScheduling()
{
    vector<pair<int, int>> SortArrivalTime;
    vector<pair<int, int>> SortBustTime;
    for (int i = 0; i < this->noP; i++)
    {
        pair<int, int> k = { i, this->Process[i][0] };
        SortArrivalTime.push_back(k);
    }
    //sort arrival time của mỗi process để xem process nào đến trước
    sort(SortArrivalTime.begin(), SortArrivalTime.end(), sortbysec);
    queue<int> cpu;
    queue<int> R;
    queue<int> copy_cpu;
    vector<int> nextStateProcess;
    queue<pair<int, int>> orderProcess;
    for (int i = 0; i < this->noP; i++)
    {
        nextStateProcess.push_back(1);
    }
    SortBustTime.push_back({SortArrivalTime[0].first,this->Process[SortArrivalTime[0].first][1]});
    for (int i = 1; i < SortArrivalTime.size(); i++)
    {
        if (SortArrivalTime[i].second == SortArrivalTime[0].second)
        {
            SortBustTime.push_back({SortArrivalTime[i].first,this->Process[SortArrivalTime[i].first][1] });
        }
        else
        {
            orderProcess.push(SortArrivalTime[i]);
        }
    }
    sort(SortBustTime.begin(), SortBustTime.end(), sortbysecBust);
    for (int i = 0; i < SortBustTime.size(); i++)
    {
        cpu.push(SortBustTime[i].first);
    }
    int i = SortArrivalTime[0].second;
    int currentPCpu = 0;
    int currentPR = 0;
    while (!cpu.empty() || !R.empty() || !orderProcess.empty())
    {
        if (cpu.size() > 0)
        {
            currentPCpu = cpu.front();
            CPUScheduling.push_back(char(currentPCpu + 49));
        }
        else
        {
            CPUScheduling.push_back('_');
            currentPCpu = -1;
        }

        if (R.size() > 0)
        {
            currentPR = R.front();
            Rscheduling.push_back(char(currentPR + 49));
        }
        else
        {
            Rscheduling.push_back('_');
            currentPR = -1;
        }
        copy_cpu = cpu;
        while (!copy_cpu.empty()) {
            if (copy_cpu.front() != currentPCpu)
            {
                WT[copy_cpu.front()]++;
            }
            copy_cpu.pop();
        }
        i++;//i = 6
        if (orderProcess.size() > 0)
        {
            if (orderProcess.front().second == i)
            {
                cpu.push(orderProcess.front().first);
                orderProcess.pop();
            }
        }
        bool check = false;
        if (currentPCpu != -1)
        {
            this->Process[currentPCpu][nextStateProcess[currentPCpu]]--;
            if (this->Process[currentPCpu][nextStateProcess[currentPCpu]] == 0)
            {
                check = true;
                cpu.pop();
                SortBustTime.clear();
                while (!cpu.empty())
                {
                    SortBustTime.push_back({cpu.front(),this->Process[cpu.front()][nextStateProcess[cpu.front()]]});
                    cpu.pop();
                }
                sort(SortBustTime.begin(), SortBustTime.end(), sortbysecBust);
                for (int i = 0; i < SortBustTime.size(); i++)
                {
                    cpu.push(SortBustTime[i].first);
                }
                if (Process[currentPCpu].size() - 1 > nextStateProcess[currentPCpu])
                {
                    nextStateProcess[currentPCpu]++;
                    R.push(currentPCpu);
                }
                else {
                    TT[currentPCpu] = i - this->Process[currentPCpu][0];
                }
            }
        }
        if (currentPR != -1)
        {
            this->Process[currentPR][nextStateProcess[currentPR]]--;
            if (this->Process[currentPR][nextStateProcess[currentPR]] == 0)
            {
                if (Process[currentPR].size() - 1 > nextStateProcess[currentPR])
                {
                    nextStateProcess[currentPR]++;
                    cpu.push(currentPR);
                    R.pop();
                    if (check)
                    {
                        SortBustTime.clear();
                        while (!cpu.empty())
                        {
                            SortBustTime.push_back({ cpu.front(),this->Process[cpu.front()][nextStateProcess[cpu.front()]]});
                            cpu.pop();
                        }
                        sort(SortBustTime.begin(), SortBustTime.end(), sortbysecBust);
                        for (int i = 0; i < SortBustTime.size(); i++)
                        {
                            cpu.push(SortBustTime[i].first);
                        }
                    }
                }
                else {
                    R.pop();
                    TT[currentPR] = i - this->Process[currentPR][0];
                }
            }
        }
    }
    for (int i = 0; i < this->WT.size(); i++)
    {
        cout << WT[i] << " ";
    }
    cout << endl;
    for (int i = 0; i < this->TT.size(); i++)
    {
        cout << TT[i] << " ";
    }
    cout << endl;
    for (int i = 0; i < CPUScheduling.size(); i++)
    {
        cout << CPUScheduling[i];
    }
    cout << endl;
    for (int i = 0; i < Rscheduling.size(); i++)
    {
        cout << Rscheduling[i];
    }
}
void SRTN::processScheduling()
{
    vector<pair<int, int>> SortArrivalTime;
    vector<pair<int, int>> SortBustTime;
    for (int i = 0; i < this->noP; i++)
    {
        pair<int, int> k = { i, this->Process[i][0] };
        SortArrivalTime.push_back(k);
    }
    //sort arrival time của mỗi process để xem process nào đến trước
    sort(SortArrivalTime.begin(), SortArrivalTime.end(), sortbysec);
    queue<int> cpu;
    queue<int> R;
    queue<int> copy_cpu;
    vector<int> nextStateProcess;
    queue<pair<int, int>> orderProcess;
    for (int i = 0; i < this->noP; i++)
    {
        nextStateProcess.push_back(1);
    }
    SortBustTime.push_back({ SortArrivalTime[0].first,this->Process[SortArrivalTime[0].first][1] });
    for (int i = 1; i < SortArrivalTime.size(); i++)
    {
        if (SortArrivalTime[i].second == SortArrivalTime[0].second)
        {
            SortBustTime.push_back({ SortArrivalTime[i].first,this->Process[SortArrivalTime[i].first][1] });
        }
        else
        {
            orderProcess.push(SortArrivalTime[i]);
        }
    }
    sort(SortBustTime.begin(), SortBustTime.end(), sortbysecBust);
    for (int i = 0; i < SortBustTime.size(); i++)
    {
        cpu.push(SortBustTime[i].first);
    }
    int i = SortArrivalTime[0].second;
    int currentPCpu = 0;
    int currentPR = 0;
    while (!cpu.empty() || !R.empty() || !orderProcess.empty())
    {
        if (cpu.size() > 0)
        {
            currentPCpu = cpu.front();
            CPUScheduling.push_back(char(currentPCpu + 49));
        }
        else
        {
            CPUScheduling.push_back('_');
            currentPCpu = -1;
        }

        if (R.size() > 0)
        {
            currentPR = R.front();
            Rscheduling.push_back(char(currentPR + 49));
        }
        else
        {
            Rscheduling.push_back('_');
            currentPR = -1;
        }
        copy_cpu = cpu;
        while (!copy_cpu.empty()) {
            if (copy_cpu.front() != currentPCpu)
            {
                WT[copy_cpu.front()]++;
            }
            copy_cpu.pop();
        }
        i++;//i = 6
        if (orderProcess.size() > 0)
        {
            if (orderProcess.front().second == i)
            {
                cpu.push(orderProcess.front().first);
                orderProcess.pop();
                SortBustTime.clear();
                SortBustTime.clear();
                while (!cpu.empty())
                {
                    SortBustTime.push_back({ cpu.front(),this->Process[cpu.front()][nextStateProcess[cpu.front()]] });
                    cpu.pop();
                }
                sort(SortBustTime.begin(), SortBustTime.end(), sortbysecBust);
                for (int i = 0; i < SortBustTime.size(); i++)
                {
                    cpu.push(SortBustTime[i].first);
                }
            }
        }
        bool check = false;
        if (currentPCpu != -1)
        {
            this->Process[currentPCpu][nextStateProcess[currentPCpu]]--;
            if (this->Process[currentPCpu][nextStateProcess[currentPCpu]] == 0)
            {
                check = true;
                cpu.pop();
                SortBustTime.clear();
                while (!cpu.empty())
                {
                    SortBustTime.push_back({ cpu.front(),this->Process[cpu.front()][nextStateProcess[cpu.front()]] });
                    cpu.pop();
                }
                sort(SortBustTime.begin(), SortBustTime.end(), sortbysecBust);
                for (int i = 0; i < SortBustTime.size(); i++)
                {
                    cpu.push(SortBustTime[i].first);
                }
                if (Process[currentPCpu].size() - 1 > nextStateProcess[currentPCpu])
                {
                    nextStateProcess[currentPCpu]++;
                    R.push(currentPCpu);
                }
                else {
                    TT[currentPCpu] = i - this->Process[currentPCpu][0];
                }
            }
            /*else
            {
                SortBustTime.clear();
                while (!cpu.empty())
                {
                    SortBustTime.push_back({ cpu.front(),this->Process[cpu.front()][nextStateProcess[cpu.front()]] });
                    cpu.pop();
                }
                sort(SortBustTime.begin(), SortBustTime.end(), sortbysecBust);
                for (int i = 0; i < SortBustTime.size(); i++)
                {
                    cpu.push(SortBustTime[i].first);
                }
            }*/
        }
        if (currentPR != -1)
        {
            this->Process[currentPR][nextStateProcess[currentPR]]--;
            if (this->Process[currentPR][nextStateProcess[currentPR]] == 0)
            {
                if (Process[currentPR].size() - 1 > nextStateProcess[currentPR])
                {
                    nextStateProcess[currentPR]++;
                    cpu.push(currentPR);
                    R.pop();
                    if (check)
                    {
                        SortBustTime.clear();
                        while (!cpu.empty())
                        {
                            SortBustTime.push_back({ cpu.front(),this->Process[cpu.front()][nextStateProcess[cpu.front()]] });
                            cpu.pop();
                        }
                        sort(SortBustTime.begin(), SortBustTime.end(), sortbysecBust);
                        for (int i = 0; i < SortBustTime.size(); i++)
                        {
                            cpu.push(SortBustTime[i].first);
                        }
                    }
                }
                else {
                    R.pop();
                    TT[currentPR] = i - this->Process[currentPR][0];
                }
            }
        }
    }
    for (int i = 0; i < this->WT.size(); i++)
    {
        cout << WT[i] << " ";
    }
    cout << endl;
    for (int i = 0; i < this->TT.size(); i++)
    {
        cout << TT[i] << " ";
    }
    cout << endl;
    for (int i = 0; i < CPUScheduling.size(); i++)
    {
        cout << CPUScheduling[i];
    }
    cout << endl;
    for (int i = 0; i < Rscheduling.size(); i++)
    {
        cout << Rscheduling[i];
    }
}

