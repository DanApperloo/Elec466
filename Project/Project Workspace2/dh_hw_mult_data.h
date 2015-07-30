#include "systemc.h"
#include "hw_const.h"
#include "hw_components.h"

#ifndef _DH_HW_MULT_DATA_H_
#define _DH_HW_MULT_DATA_H_


SC_MODULE (dh_hw_mult_data)
{
    // Input Ports
    sc_in_clk Clk;
    sc_in< bool > reset;
    sc_in< sc_uint<32> > in_b;
    sc_in< sc_uint<32> > in_c;

    // Control Ports
    sc_in< bool > load_input, save_output;
    sc_in< sc_uint<2> > input_mux_select;
    sc_in< bool > reg_file_in_mux_select;
    sc_in< bool > write_enable, read_enable;
    sc_in< sc_uint<REG_ADDRESS_SPACE> > write_select,
                                      read_a_select, read_b_select;
    sc_in< sc_uint<2> > adder_mux_select, output_mux_select;
    sc_in< bool > mult_enable, adder_enable;
    sc_in< bool > shifter_enable, shifter_direction;
    sc_out< bool > comp_eq, comp_gt, comp_lt;

    // Output Ports
    sc_out< sc_uint<32> > a0;
    sc_out< sc_uint<32> > a1;

    // Components
    hw_const<NN_DIGIT_BITS> high_1_const, low_1_const, zero_const;
    register_wt_half_index b_reg, c_reg;
    mux4 input_mux, adder_mux, output_mux;
    mux2 reg_file_in_mux;
    register_file reg_file;
    multiplier mult;
    adder add;
    shifter16 shifter;
    comparator comparison;
    register_32 a1_reg, a0_reg;


    // Wiring Signals
    sc_signal< sc_uint<NN_DIGIT_BITS> > high_1_value, low_1_value, zero_value;
    sc_signal< sc_uint<NN_DIGIT_BITS> > b_high, b_low, c_high, c_low;
    sc_signal< sc_uint<NN_DIGIT_BITS> > input_reg_value, data_path_calculated_value, regfile_input;
    sc_signal< sc_uint<NN_DIGIT_BITS> > out_a, out_b;
    sc_signal< sc_uint<NN_DIGIT_BITS> > adder_mux_output, adder_output, shifter_output, mult_output;


    SC_CTOR (dh_hw_mult_data) : 
        high_1_const("high_1_const", 0x00010000), low_1_const("low_1_const", 0x00000001), zero_const("zero_const", 0x00000000),
        b_reg("b_reg"), c_reg("c_reg"), input_mux("input_mux"), adder_mux("adder_mux"), output_mux("output_mux"), 
        reg_file_in_mux("reg_file_in_mux"), reg_file("reg_file"), mult("mult"), add("add"), shifter("shifter"), 
        comparison("comparison"), a1_reg("a1_reg"), a0_reg("a0_reg")
    {
        // Constant Connections
        high_1_const.output(high_1_value);
        low_1_const.output(low_1_value);
        zero_const.output(zero_value);

        // Init Load
        b_reg.in(in_b);
        b_reg.out_high(b_high);
        b_reg.out_low(b_low);
        b_reg.clk(Clk);
        b_reg.load(load_input);

        c_reg.in(in_c);
        c_reg.out_high(c_high);
        c_reg.out_low(c_low);
        c_reg.clk(Clk);
        c_reg.load(load_input);

        // FSM Input Mux
        input_mux.in_a(b_high);
        input_mux.in_b(b_low);
        input_mux.in_c(c_high);
        input_mux.in_d(c_low);
        input_mux.out(input_reg_value);
        input_mux.select(input_mux_select);

        // Register File Input Mux
        reg_file_in_mux.in_a(input_reg_value);
        reg_file_in_mux.in_b(data_path_calculated_value);
        reg_file_in_mux.out(regfile_input);
        reg_file_in_mux.select(reg_file_in_mux_select);

        reg_file.clk(Clk);
        reg_file.in_a(regfile_input);
        reg_file.write_enable(write_enable);
        reg_file.write_select(write_select);
        reg_file.read_a_select(read_a_select);
        reg_file.read_b_select(read_b_select);
        reg_file.read_enable(read_enable);
        reg_file.out_a(out_a);
        reg_file.out_b(out_b);

        // Multiplier
        mult.in_a(out_a);
        mult.in_b(out_b);
        mult.out(mult_output);
        mult.enable(mult_enable);

        // Adder Input Mux
        adder_mux.in_a(out_b);
        adder_mux.in_b(high_1_value);
        adder_mux.in_c(low_1_value);
        adder_mux.in_d(zero_value);
        adder_mux.out(adder_mux_output);
        adder_mux.select(adder_mux_select);

        // Adder
        add.in_a(out_a);
        add.in_b(adder_mux_output);
        add.out(adder_output);
        add.enable(adder_enable);

        // Shifter
        shifter.in(out_a);
        shifter.out(shifter_output);
        shifter.direction(shifter_direction);
        shifter.enable(shifter_enable);

        // Comparator
        comparison.in_a(out_a);
        comparison.in_b(out_b);
        comparison.eq(comp_eq);
        comparison.lt(comp_lt);
        comparison.gt(comp_gt);

        // Output Mux
        output_mux.in_a(mult_output);
        output_mux.in_b(adder_output);
        output_mux.in_c(shifter_output);
        output_mux.in_d(zero_value);
        output_mux.out(data_path_calculated_value);
        output_mux.select(output_mux_select);

        // Final Phase
        a0_reg.in(out_a);
        a0_reg.out(a0);
        a0_reg.clk(Clk);
        a0_reg.load(save_output);
        
        a1_reg.in(out_b);
        a1_reg.out(a1);
        a1_reg.clk(Clk);
        a1_reg.load(save_output);
    }
};

#endif /* end _DH_HW_MULT_DATA_H_ */