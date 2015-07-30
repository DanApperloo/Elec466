#ifndef _COMPARATOR_H_
#define _COMPARATOR_H_

#include "systemc.h"


/********************************************************************************/
/* Comparator Template                                                          */
/********************************************************************************/
template <int S>
SC_MODULE (comparator)
{
    sc_in< sc_uint<S> > in_a;
    sc_in< sc_uint<S> > in_b;
    sc_out< sc_logic > eq;
    sc_out< sc_logic > gt;
    sc_out< sc_logic > lt;


    void process_compare(void)
    {
        // Sample the input
        sc_uint<S> a_sample = in_a.read();
        sc_uint<S> b_sample = in_b.read();
    
        // Default to SC_LOGIC_0
        eq.write(SC_LOGIC_0);
        lt.write(SC_LOGIC_0);
        gt.write(SC_LOGIC_0);
    
        if (a_sample == b_sample)
        {
            eq.write(SC_LOGIC_1);
        }
        else if (a_sample > b_sample)
        {
            gt.write(SC_LOGIC_1);
        }
        else if (a_sample < b_sample)
        {
            lt.write(SC_LOGIC_1);
        }
    }

    SC_CTOR (comparator)
    {
        SC_METHOD(process_compare);
        sensitive << in_a << in_b;
        eq.initialize(SC_LOGIC_0);
        gt.initialize(SC_LOGIC_0);
        lt.initialize(SC_LOGIC_0);
    }
};

#endif // _COMPARATOR_H_