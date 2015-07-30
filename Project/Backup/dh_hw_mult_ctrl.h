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

typedef enum _eDH_HW_MULT_EX_SUBSTATE
{
    DH_HW_MULT_EX_IDLE = 0,

    DH_HW_MULT_EX_LD_IN_1,
    DH_HW_MULT_EX_LD_IN_2,
    DH_HW_MULT_EX_LD_IN_3,
    DH_HW_MULT_EX_LD_IN_4,
    DH_HW_MULT_EX_LD_IN_TEMP,

    DH_HW_MULT_EX_MULT_1,
    DH_HW_MULT_EX_MULT_2,
    DH_HW_MULT_EX_MULT_3,
    DH_HW_MULT_EX_MULT_4,

    DH_HW_MULT_EX_T_U,
    DH_HW_MULT_EX_COMPARE_1,
    DH_HW_MULT_EX_ADD_HIGH_1,
    DH_HW_MULT_EX_SET_U,

    DH_HW_MULT_EX_A0_U,
    DH_HW_MULT_EX_COMPARE_2,
    DH_HW_MULT_EX_INCREMENT,
    DH_HW_MULT_EX_SHIFT_T,
    DH_HW_MULT_EX_ADD_TO_A1,

    DH_HW_MULT_EX_FINISH

} eDH_HW_MULT_EX_SUBSTATE;


SC_MODULE (dh_hw_mult_ctrl)
{
    // Input Ports
    sc_in_clk clk;
    sc_in<bool> reset;
    sc_in<bool> hw_mult_enable; 

    // Control Ports
    sc_in< sc_logic > comp_eq, comp_gt, comp_lt;
    sc_out< sc_logic > load_input, save_output;
    sc_out< sc_lv<2> > input_mux_select;
    sc_out< sc_logic > reg_file_in_mux_select;
    sc_out< sc_logic > reg_file_write_enable, reg_file_read_enable;
    sc_out< sc_lv<REG_ADDRESS_SPACE> > reg_file_write_address,
                                       reg_file_read_a_address, reg_file_read_b_address;
    sc_out< sc_lv<2> > adder_mux_select, output_mux_select;
    sc_out< sc_logic > mult_enable, adder_enable;
    sc_out< sc_logic > shifter_enable, shifter_direction;

    // Output Ports
    sc_out<bool> hw_mult_done;

     /* Internal variables and signals */
    sc_signal<eDH_HW_MULT_STATE> current_state;
    sc_signal<eDH_HW_MULT_EX_SUBSTATE> current_substate;


    void process_hw_mult_ctrl();
    void process_ex_subfsm(eDH_HW_MULT_EX_SUBSTATE);

    
    SC_CTOR (dh_hw_mult_ctrl)
    {
        SC_CTHREAD (process_hw_mult_ctrl, clk.pos());

        current_state.write(DH_HW_MULT_WAIT);
        current_substate.write(DH_HW_MULT_EX_IDLE);

        load_input.initialize(SC_LOGIC_0);
        save_output.initialize(SC_LOGIC_0);
        input_mux_select.initialize(0);
        reg_file_in_mux_select.initialize(SC_LOGIC_0);
        reg_file_write_enable.initialize(SC_LOGIC_0);
        reg_file_read_enable.initialize(SC_LOGIC_0);
        reg_file_write_address.initialize(0);
        reg_file_read_a_address.initialize(0);
        reg_file_read_b_address.initialize(0);
        adder_mux_select.initialize(0);
        output_mux_select.initialize(0);
        mult_enable.initialize(SC_LOGIC_0);
        adder_enable.initialize(SC_LOGIC_0);
        shifter_enable.initialize(SC_LOGIC_0);
        shifter_direction.initialize(SC_LOGIC_0);
        hw_mult_done.initialize(false);
    }
};

#endif /* end _DH_HW_MULT_CTRL_H_ */