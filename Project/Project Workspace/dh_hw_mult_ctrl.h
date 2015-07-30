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
    DH_HW_MULT_EX_START = 0,
    DH_HW_MULT_EX_PHASE_1,
    DH_HW_MULT_EX_PHASE_2,
    DH_HW_MULT_EX_PHASE_3,
    DH_HW_MULT_EX_FINISH

} eDH_HW_MULT_EX_SUBSTATE;


SC_MODULE (dh_hw_mult_ctrl)
{
    sc_in_clk clk;
    sc_in<bool> reset;
    sc_in<bool> hw_mult_enable; 

    sc_in< bool > phase_one_eq, phase_one_gt, phase_one_lt;
    sc_in< bool > phase_two_eq, phase_two_gt, phase_two_lt;

    sc_out< bool > load_input;
    sc_out< bool > save_output;
    sc_out< bool > phase_one_select;
    sc_out< bool > phase_two_select;
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
        current_substate.write(DH_HW_MULT_EX_START);

        load_input.initialize(false);
        save_output.initialize(false);
        phase_one_select.initialize(false);
        phase_two_select.initialize(false);
        hw_mult_done.initialize(false);
    }
};

#endif /* end _DH_HW_MULT_CTRL_H_ */