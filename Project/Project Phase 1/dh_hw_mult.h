#include "systemc.h"
#include "digit.h"

#ifndef _DH_HW_MULT_H_
#define _DH_HW_MULT_H_ 1

typedef enum _eDH_HW_MULT_STATE
{
    DH_HW_MULT_WAIT = 0,
    DH_HW_MULT_EXECUTE,
    DH_HW_MULT_OUTPUT,
    DH_HW_MULT_FINISH

} eDH_HW_MULT_STATE;


SC_MODULE (dh_hw_mult)
{
    sc_in<bool> hw_mult_enable; 
    sc_in<NN_DIGIT> in_data_1;
    sc_in<NN_DIGIT> in_data_2;
    sc_out<NN_DIGIT> out_data_low;
    sc_out<NN_DIGIT> out_data_high;
    sc_out<bool> hw_mult_done;

    sc_in_clk clk;
    sc_in<bool> reset;

    /* Internal variables and signals */
    sc_signal<eDH_HW_MULT_STATE> current_state;

    void process_hw_mult();
    
    SC_CTOR (dh_hw_mult)
    {
        SC_CTHREAD (process_hw_mult, clk.pos());

        current_state.write(DH_HW_MULT_WAIT);
        hw_mult_done.initialize(false);
    }
    
};

#endif /* end _DH_HW_MULT_H_ */