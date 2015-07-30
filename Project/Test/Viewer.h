#ifndef _VIEWER_H_
#define _VIEWER_H_

#include "systemc.h"
#include "digit.h"


SC_MODULE (Viewer)
{
    sc_in< sc_uint<32> > out_a, out_b;

    void process_viewer(void);


    SC_CTOR (Viewer)
    {
        SC_THREAD(process_viewer);
        sensitive << out_a << out_b;
    }
};

#endif // _VIEWER_H_