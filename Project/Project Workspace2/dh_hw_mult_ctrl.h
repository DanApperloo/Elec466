#include "systemc.h"
#include "common.h"

#ifndef _DH_HW_MULT_CTRL_H_
#define _DH_HW_MULT_CTRL_H_


typedef enum _eDH_HW_MULT_STATE
{
    DH_HW_MULT_WAIT = 0,
    DH_HW_MULT_EXECUTE,
    DH_HW_MULT_OUTPUT,
    DH_HW_MULT_FINISH

} eDH_HW_MULT_STATE;



SC_MODULE (dh_hw_mult_ctrl)
{
    // Input Ports
    sc_in_clk clk;
    sc_in<bool> reset;
    sc_in<bool> hw_mult_enable; 

    // Control Ports
    sc_in< bool > comp_eq, comp_gt, comp_lt;
    sc_out< bool > load_input, save_output;
    sc_out< sc_uint<2> > input_mux_select;
    sc_out< bool > reg_file_in_mux_select;
    sc_out< bool > write_enable, read_enable;
    sc_out< sc_uint<REG_ADDRESS_SPACE> > write_select,
                                       read_a_select, read_b_select;
    sc_out< sc_uint<2> > adder_mux_select, output_mux_select;
    sc_out< bool > mult_enable, adder_enable;
    sc_out< bool > shifter_enable, shifter_direction;

    // Output Ports
    sc_out<bool> hw_mult_done;

     /* Internal variables and signals */
    sc_signal<eDH_HW_MULT_STATE> current_state;


    void process_hw_mult_ctrl();
    void process_ex_subfsm(void);

    
    SC_CTOR (dh_hw_mult_ctrl)
    {
        SC_CTHREAD (process_hw_mult_ctrl, clk.pos());

        current_state.write(DH_HW_MULT_WAIT);

        load_input.initialize(false);
        save_output.initialize(false);
        input_mux_select.initialize(0);
        reg_file_in_mux_select.initialize(false);
        write_enable.initialize(false);
        read_enable.initialize(false);
        write_select.initialize(0);
        read_a_select.initialize(0);
        read_b_select.initialize(0);
        adder_mux_select.initialize(0);
        output_mux_select.initialize(0);
        mult_enable.initialize(false);
        adder_enable.initialize(false);
        shifter_enable.initialize(false);
        shifter_direction.initialize(false);
        hw_mult_done.initialize(false);
    }
};

#endif /* end _DH_HW_MULT_CTRL_H_ */