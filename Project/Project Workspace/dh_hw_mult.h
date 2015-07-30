#include "systemc.h"
#include "digit.h"
#include "digit_adapter.h"
#include "dh_hw_mult_ctrl.h"
#include "dh_hw_mult_data.h"

#ifndef _DH_HW_MULT_H_
#define _DH_HW_MULT_H_


SC_MODULE (dh_hw_mult)
{
    // Input Variables
    sc_in_clk clk;
    sc_in<bool> hw_mult_enable; 
    sc_in<NN_DIGIT> in_data_1;
    sc_in<NN_DIGIT> in_data_2;
    sc_in<bool> reset;

    // Output Variables
    sc_out<NN_DIGIT> out_data_low;
    sc_out<NN_DIGIT> out_data_high;
    sc_out<bool> hw_mult_done;

    // Internal Signals
    sc_signal< bool > load_input;
    sc_signal< bool > save_output;
    sc_signal< bool > phase_one_select;
    sc_signal< bool > phase_two_select;

    sc_signal< bool > phase_one_eq, phase_one_gt, phase_one_lt;
    sc_signal< bool > phase_two_eq, phase_two_gt, phase_two_lt;

    sc_signal< sc_uint<32> > connector_a, connector_b, connector_c, connector_d;


    // Components
    digit_adapter_in digit_adapt_in_a, digit_adapt_in_b;
    digit_adapter_out digit_adapt_out_h, digit_adapt_out_l;
    
    dh_hw_mult_ctrl control;
    dh_hw_mult_data datapath;


    SC_CTOR (dh_hw_mult):
        digit_adapt_in_a("digit_adapt_in_a"), digit_adapt_in_b("digit_adapt_in_b"),
        digit_adapt_out_h("digit_adapt_out_h"), digit_adapt_out_l("digit_adapt_out_l"),
        control("Control"), datapath("Datapath")
    {
        // Control Path Connections
        control.hw_mult_enable(hw_mult_enable);
        control.hw_mult_done(hw_mult_done);

        control.clk(clk);
        control.reset(reset);

        // Internal Connections
        control.phase_one_select(phase_one_select);
        control.phase_one_eq(phase_one_eq);
        control.phase_one_gt(phase_one_gt);
        control.phase_one_lt(phase_one_lt);
        control.phase_two_select(phase_two_select);
        control.phase_two_eq(phase_two_eq);
        control.phase_two_gt(phase_two_gt);
        control.phase_two_lt(phase_two_lt);
        control.load_input(load_input);
        control.save_output(save_output);

        datapath.phase_one_select(phase_one_select);
        datapath.phase_one_eq(phase_one_eq);
        datapath.phase_one_gt(phase_one_gt);
        datapath.phase_one_lt(phase_one_lt);
        datapath.phase_two_select(phase_two_select);
        datapath.phase_two_eq(phase_two_eq);
        datapath.phase_two_gt(phase_two_gt);
        datapath.phase_two_lt(phase_two_lt);
        datapath.load_input(load_input);
        datapath.save_output(save_output);

        // Data Path Connections
        digit_adapt_in_a.in_data(in_data_1);
        digit_adapt_in_a.out_data(connector_a);
        digit_adapt_in_b.in_data(in_data_2);
        digit_adapt_in_b.out_data(connector_b);
        digit_adapt_out_h.in_data(connector_c);
        digit_adapt_out_h.out_data(out_data_high);
        digit_adapt_out_l.in_data(connector_d);
        digit_adapt_out_l.out_data(out_data_low);

        datapath.in_data_1(connector_a);
        datapath.in_data_2(connector_b);
        datapath.out_data_high(connector_c);
        datapath.out_data_low(connector_d);

        datapath.clk(clk);
        datapath.reset(reset);
    }
};

#endif /* end _DH_HW_MULT_CTRL_H_ */