#ifndef _ADDER_H_
#define _ADDER_H_

#include "systemc.h"


/********************************************************************************/
/* Adder Template                                                               */
/********************************************************************************/
template <int S>
SC_MODULE (adder)
{
    sc_in< sc_uint<S> > in_a;
    sc_in< sc_uint<S> > in_b;
    sc_out< sc_uint<S> > out;

    void process_add(void)
    {
        sc_uint<S> a_sample = in_a.read();
        sc_uint<S> b_sample = in_b.read();
        out.write(a_sample + a_sample);
    }

    SC_CTOR (adder)
    {
        SC_METHOD(process_add);
        sensitive << in_a << in_b;
        out.initialize(0);
    }
};

#endif // _ADDER_H_