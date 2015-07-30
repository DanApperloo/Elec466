#include "systemc.h"
#include "digit.h"
#include "Viewer.h"
#include <iomanip>

using namespace std;


void Viewer::process_viewer(void)
{
    cout << setw(15) << "Const";
    cout << setw(15) << "High1: " << high_1_value.read().to_uint();
    cout << setw(15) << "Low 1: " << low_1_value.read().to_uint();
    cout << setw(15) << "Zero : " << zero_value.read().to_uint() << endl;

    cout << setw(15) << "Time";
    cout << setw(15) << "b_high";
    cout << setw(15) << "b_low";
    cout << setw(15) << "c_high";
    cout << setw(15) << "c_low" << endl;

    while (true) 
    {
        cout << setw(15) << sc_time_stamp();
        cout << setw(15) << b_high.read().to_uint();
        cout << setw(15) << b_low.read().to_uint();
        cout << setw(15) << c_high.read().to_uint();
        cout << setw(15) << c_low.read().to_uint() << endl;
        wait();
    }
}