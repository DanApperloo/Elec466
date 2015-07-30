#include "systemc.h"
#include "digit.h"
#include "Viewer.h"
#include <iomanip>

using namespace std;


void Viewer::process_viewer(void)
{
    cout << setw(15) << "Time";
    cout << setw(15) << "Out_a";
    cout << setw(15) << "Out_b" << endl;

    while (true) 
    {
        cout << setw(15) << sc_time_stamp();
        cout << setw(15) << out_a.read().to_uint();
        cout << setw(15) << out_b.read().to_uint() << endl;
        wait();
    }
}