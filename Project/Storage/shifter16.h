#include "systemc.h"

#ifndef _SHIFTER16_H_
#define _SHIFTER16_H_


/********************************************************************************/
/* Shifter Template                                                             */
/********************************************************************************/
SC_MODULE(shifter16)
{
    sc_in< sc_uint<32> > in;
    sc_out< sc_uint<32> > out;

    void process_shift(void);

    SC_CTOR (shifter16)
    {
        SC_METHOD(process_shift);
        sensitive << in;
        out.initialize(0);
    }
};


#endif // _SHIFTER16_H_