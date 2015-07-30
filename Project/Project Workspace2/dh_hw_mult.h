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
    sc_signal< bool > load_input, save_output;
    sc_signal< sc_uint<2> > input_mux_select;
    sc_signal< bool > reg_file_in_mux_select;
    sc_signal< bool > write_enable, read_enable;
    sc_signal< sc_uint<REG_ADDRESS_SPACE> > write_select, 
                                          read_a_select, read_b_select;
    sc_signal< sc_uint<2> > adder_mux_select, output_mux_select;
    sc_signal< bool > mult_enable, adder_enable;
    sc_signal< bool > shifter_enable, shifter_direction;
    sc_signal< bool > comp_eq, comp_gt, comp_lt;

    sc_signal< sc_uint<NN_DIGIT_BITS> > connector_a, connector_b, connector_c, connector_d;


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
        control.load_input(load_input);
        control.save_output(save_output);
        control.input_mux_select(input_mux_select);
        control.reg_file_in_mux_select(reg_file_in_mux_select);
        control.write_enable(write_enable);
        control.read_enable(read_enable);
        control.write_select(write_select);
        control.read_a_select(read_a_select);
        control.read_b_select(read_b_select);
        control.adder_mux_select(adder_mux_select);
        control.output_mux_select(output_mux_select);
        control.mult_enable(mult_enable);
        control.adder_enable(adder_enable);
        control.shifter_enable(shifter_enable);
        control.shifter_direction(shifter_direction);
        control.comp_eq(comp_eq);
        control.comp_gt(comp_gt);
        control.comp_lt(comp_lt);

        datapath.load_input(load_input);
        datapath.save_output(save_output);
        datapath.input_mux_select(input_mux_select);
        datapath.reg_file_in_mux_select(reg_file_in_mux_select);
        datapath.write_enable(write_enable);
        datapath.read_enable(read_enable);
        datapath.write_select(write_select);
        datapath.read_a_select(read_a_select);
        datapath.read_b_select(read_b_select);
        datapath.adder_mux_select(adder_mux_select);
        datapath.output_mux_select(output_mux_select);
        datapath.mult_enable(mult_enable);
        datapath.adder_enable(adder_enable);
        datapath.shifter_enable(shifter_enable);
        datapath.shifter_direction(shifter_direction);
        datapath.comp_eq(comp_eq);
        datapath.comp_gt(comp_gt);
        datapath.comp_lt(comp_lt);

        // Data Path Connections
        digit_adapt_in_a.in_data(in_data_1);
        digit_adapt_in_a.out_data(connector_a);
        digit_adapt_in_b.in_data(in_data_2);
        digit_adapt_in_b.out_data(connector_b);
        digit_adapt_out_h.in_data(connector_c);
        digit_adapt_out_h.out_data(out_data_high);
        digit_adapt_out_l.in_data(connector_d);
        digit_adapt_out_l.out_data(out_data_low);

        datapath.in_b(connector_a);
        datapath.in_c(connector_b);
        datapath.a1(connector_c);
        datapath.a0(connector_d);

        datapath.Clk(clk);
        datapath.reset(reset);
    }
};

#endif /* end _DH_HW_MULT_CTRL_H_ */