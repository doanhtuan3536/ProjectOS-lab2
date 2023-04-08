#include "header.h"
using namespace std;
int main()
{
    // // if (argc != 3) 
    // // {
    // //     cout << "Invalid syntax!"                                                                                           << endl;
    // //     cout << "[Execution file] [Input file] [Output file]"          << endl;
    // //     return 0;
    // // }
    string inputfile = "input.txt";
    string outputfile = "output.txt";
    SchedulingAlgorithm* a = SchedulingAlgorithm::readFile(inputfile);
    a->processScheduling();
    a->WriteFile(outputfile);
    delete a;
}