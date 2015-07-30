#include "systemc.h"
#include "digit.h"

#ifndef _DIGIT_ADAPTER_H_
#define _DIGIT_ADAPTER_H_

SC_MODULE (digit_adapter_in)
{
    sc_in< NN_DIGIT > in_data;
    sc_out< sc_uint<NN_DIGIT_BITS> > out_data;

    
    void process_in_conversion()
    {
        out_data.write((sc_uint<NN_DIGIT_BITS>) in_data.read());
    };

    
    SC_CTOR (digit_adapter_in)
    {
        SC_METHOD (process_in_conversion);
        sensitive << in_data;
        out_data.initialize(0);
    }
};


SC_MODULE (digit_adapter_out)
{
    sc_in< sc_uint<NN_DIGIT_BITS> > in_data;
    sc_out< NN_DIGIT > out_data;

    
    void process_out_conversion()
    {
        out_data.write(in_data.read().to_uint());
    };

    
    SC_CTOR (digit_adapter_out)
    {
        SC_METHOD (process_out_conversion);
        sensitive << in_data;
        out_data.initialize(0);
    }
};

#endif /* end _DIGIT_ADAPTER_H_ */