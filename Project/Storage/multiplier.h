#ifndef _MULTIPLIER_H_
#define _MULTIPLIER_H_

#include "systemc.h"


/********************************************************************************/
/* Multiplier Template                                                          */
/********************************************************************************/
template <int S>
SC_MODULE (multiplier)
{
    sc_in< sc_uint<S> > in_a;
    sc_in< sc_uint<S> > in_b;
    sc_out< sc_uint<2*S> > out;


    void process_mult(void)
    {
        sc_uint<S> a_sample = in_a.read();
        sc_uint<S> b_sample = in_b.read();
        out.write(a_sample * a_sample);
    }

    SC_CTOR (multiplier)
    {
        SC_METHOD(process_mult);
        sensitive << in_a << in_b;
        out.initialize(0);
    }
};

#endif // _MULTIPLIER_H_