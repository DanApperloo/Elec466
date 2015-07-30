#ifndef _REGISTER_H_
#define _REGISTER_H_

#include "systemc.h"


/********************************************************************************/
/* Register Template                                                            */
/********************************************************************************/
template <int S>
SC_MODULE (reg)
{
    sc_in_clk clk;

    sc_in< sc_uint<S> > in;
    sc_in< sc_logic > load;
    sc_out< sc_uint<S> > out;


    void process_reg(void)
    {
        while (true)
        {
            if (SC_LOGIC_1 == load.read())
            {
                out.write(in.read());
            }
            wait();
        }
    }

    SC_CTOR (reg)
    {
        SC_CTHREAD(process_reg, clk.pos());
        out.initialize(0);
    }
};

#endif // _REGISTER_H_