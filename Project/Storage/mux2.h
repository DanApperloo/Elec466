#ifndef _MUX2_H_
#define _MUX2_H_

#include "systemc.h"


/********************************************************************************/
/* Mux2 Template                                                                */
/********************************************************************************/
template <int S>
SC_MODULE (mux2)
{
    sc_in< sc_uint<S> > in_a;
    sc_in< sc_uint<S> > in_b;
    sc_in< sc_logic > select;
    sc_out< sc_uint<S> > out;


    void process_mux(void)
    {
        if (SC_LOGIC_1 == select.read())
        {
            out.write(in_b.read());
        }
        else
        {
            out.write(in_a.read());
        }
    }

    SC_CTOR (mux2)
    {
        SC_METHOD(process_mux);
        sensitive << in_a << in_b << select;
        out.initialize(0);
    }
};

#endif // _MUX2_H_