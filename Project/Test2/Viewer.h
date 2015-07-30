#ifndef _VIEWER_H_
#define _VIEWER_H_

#include "systemc.h"
#include "digit.h"


SC_MODULE (Viewer)
{
    sc_in< sc_uint<32> > b_high, c_high;
    sc_in< sc_uint<32> > b_low,  c_low;
    sc_in< sc_uint<32> > high_1_value, low_1_value, zero_value;

    void process_viewer(void);


    SC_CTOR (Viewer)
    {
        SC_THREAD(process_viewer);
        sensitive << b_high << c_high << b_low << c_low 
            << high_1_value << low_1_value << zero_value;
    }
};

#endif // _VIEWER_H_