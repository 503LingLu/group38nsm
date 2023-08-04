//#include "SM3.h"
#include "optimize.h"

int main()
{
    string str = "202100150110";

    LARGE_INTEGER BeginTime;
    LARGE_INTEGER EndTime;
    LARGE_INTEGER Frequency;
    QueryPerformanceFrequency(&Frequency);
    QueryPerformanceCounter(&BeginTime);
    cout << "Input message is: " << str << endl;
    cout << endl;
    string paddingValue = padding(str);
    string result = iteration(paddingValue);
    QueryPerformanceCounter(&EndTime);
    cout << "Hash value: " << endl;
    for (int i = 0; i < 8; i++)
    {
        cout << result.substr(i * 8, 8) << "  ";
    }
    cout << endl;
    double time = (double)(EndTime.QuadPart - BeginTime.QuadPart) / Frequency.QuadPart;
    printf("Elapsed time: %f seconds\n", time);
}