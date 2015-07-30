#ifndef _SPLITTER32_H_
#define _SPLITTER32_H_

#include "systemc.h"


/********************************************************************************/
/* Splitter Template                                                            */
/********************************************************************************/
SC_MODULE (splitter32)
{
    sc_in< sc_uint<32> > in;
    sc_out< sc_uint<16> > out_high;
    sc_out< sc_uint<16> > out_low;

    void process_split(void);

    SC_CTOR (splitter32)
    {
        SC_METHOD(process_split);
        sensitive << in;
        out_high.initialize(0);
        out_low.initialize(0);
    }
};

#endif // _SPLITTER32_H_