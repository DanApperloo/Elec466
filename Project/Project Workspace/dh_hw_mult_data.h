#include "systemc.h"
#include "hw_const.h"
#include "hw_components.h"

#ifndef _DH_HW_MULT_DATA_H_
#define _DH_HW_MULT_DATA_H_


SC_MODULE (dh_hw_mult_data)
{
    // Input Ports
    sc_in_clk clk;
    sc_in< bool > reset;
    sc_in< bool > load_input;
    sc_in< bool > save_output;
    sc_in< sc_uint<32> > in_data_1;
    sc_in< sc_uint<32> > in_data_2;
    sc_in< bool > phase_one_select;
    sc_in< bool > phase_two_select;

    // Output Ports
    sc_out< bool > phase_one_eq, phase_one_gt, phase_one_lt;
    sc_out< bool > phase_two_eq, phase_two_gt, phase_two_lt;
    sc_out< sc_uint<32> > out_data_low;
    sc_out< sc_uint<32> > out_data_high;

    // Components
    hw_const<32> p1_const, p2_const;
    control_const true_const, false_const;
    register_wt_half_index reg_b, reg_c;
    multiplier multa, multb, multc, multd;

    adder p1_adder_a, p1_adder_b;
    shifter16 p1_shifter;
    comparator p1_comp;

    mux2 mux_a;
    adder p2_adder_a, p2_adder_b;
    comparator p2_comp;

    mux2 mux_b;
    adder p3_adder;
    shifter16 p3_shifter;
    register_32 reg_high, reg_low;

    // Wiring Signals
    sc_signal< sc_uint<32> > const_a, const_b;
    sc_signal< bool > const_true, const_false;
    sc_signal< sc_uint<32> > b_high, b_low, c_high, c_low;
    sc_signal< sc_uint<32> > a1_mult_out, u_mult_out, t_mult_out, a0_mult_out;
    sc_signal< sc_uint<32> > p1_adder_a_out, p1_adder_b_out, shifter_out;
    sc_signal< sc_uint<32> > mux1_output, p2_adder_output;
    sc_signal< sc_uint<32> > mux2_output, p3_shifter_output;
    sc_signal< sc_uint<32> > to_reg_high, to_reg_low;


    SC_CTOR (dh_hw_mult_data) : 
        p1_const("p1_const", 0x00010000), p2_const("p2_const", 0x00000001),
        true_const("true_const", true), false_const("false_const", false),
        reg_b("reg_b"), reg_c("reg_c"),
        multa("multa"), multb("multb"), multc("multc"), multd("multd"),
        p1_adder_a("p1_adder_a"), p1_adder_b("p1_adder_b") ,p1_shifter("p1_shifter"),
        p1_comp("p1_comp"), mux_a("mux_a"), p2_adder_a("p2_adder_a"), p2_adder_b("p2_adder_b"), 
        p2_comp("p2_comp"), mux_b("mux_b"), p3_adder("p3_adder"), p3_shifter("p3_shifter"),
        reg_high("reg_high"), reg_low("reg_low")
    {
        // Const Wires
        true_const.output(const_true);
        false_const.output(const_false);

        // Init Load
        reg_b.in(in_data_1);
        reg_b.out_high(b_high);
        reg_b.out_low(b_low);
        reg_b.clk(clk);
        reg_b.load(load_input);

        reg_c.in(in_data_2);
        reg_c.out_high(c_high);
        reg_c.out_low(c_low);
        reg_c.clk(clk);
        reg_c.load(load_input);

        // Multipliers
        multa.in_a(b_high);
        multa.in_b(c_high);
        multa.out(a1_mult_out);
        multa.enable(const_true);

        multb.in_a(b_high);
        multb.in_b(c_low);
        multb.out(u_mult_out);
        multb.enable(const_true);

        multc.in_a(b_low);
        multc.in_b(c_high);
        multc.out(t_mult_out);
        multc.enable(const_true);

        multd.in_a(b_low);
        multd.in_b(c_low);
        multd.out(a0_mult_out);
        multd.enable(const_true);

        // Phase 1 Load
        p1_const.output(const_a);
        p1_adder_a.in_a(a1_mult_out);
        p1_adder_a.in_b(const_a);
        p1_adder_a.out(p1_adder_a_out);
        p1_adder_a.enable(const_true);

        p1_adder_b.in_a(u_mult_out);
        p1_adder_b.in_b(t_mult_out);
        p1_adder_b.out(p1_adder_b_out);
        p1_adder_b.enable(const_true);

        p1_comp.in_a(p1_adder_b_out);
        p1_comp.in_b(u_mult_out);
        p1_comp.eq(phase_one_eq);
        p1_comp.gt(phase_one_gt);
        p1_comp.lt(phase_one_lt);

        p1_shifter.in(p1_adder_b_out);
        p1_shifter.out(shifter_out);
        p1_shifter.direction(const_false);
        p1_shifter.enable(const_true);

        // Phase 2 Load
        mux_a.in_a(p1_adder_a_out);
        mux_a.in_b(a1_mult_out);
        mux_a.select(phase_one_select);
        mux_a.out(mux1_output);

        p2_const.output(const_b);
        p2_adder_a.in_a(mux1_output);
        p2_adder_a.in_b(const_b);
        p2_adder_a.out(p2_adder_output);
        p2_adder_a.enable(const_true);

        p2_adder_b.in_a(shifter_out);
        p2_adder_b.in_b(a0_mult_out);
        p2_adder_b.out(to_reg_low);
        p2_adder_b.enable(const_true);

        p2_comp.in_a(to_reg_low);
        p2_comp.in_b(shifter_out);
        p2_comp.eq(phase_two_eq);
        p2_comp.gt(phase_two_gt);
        p2_comp.lt(phase_two_lt);

        // Final Phase
        mux_b.in_a(p2_adder_output);       reg_high.in(to_reg_high);
        mux_b.in_b(mux1_output);           reg_high.out(out_data_high);
        mux_b.select(phase_two_select);    reg_high.clk(clk);
        mux_b.out(mux2_output);            reg_high.load(save_output);

        p3_adder.in_a(mux2_output);        reg_low.in(to_reg_low);
        p3_adder.in_b(p3_shifter_output);  reg_low.out(out_data_low);
        p3_adder.out(to_reg_high);         reg_low.clk(clk);
        p3_adder.enable(const_true);       reg_low.load(save_output);

        p3_shifter.in(p1_adder_b_out);
        p3_shifter.out(p3_shifter_output);
        p3_shifter.direction(const_true);
        p3_shifter.enable(const_true);
    }
};

#endif /* end _DH_HW_MULT_DATA_H_ */