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
    sc_in< sc_lv<32> > in_data_1;
    sc_in< sc_lv<32> > in_data_2;

    // Control Ports
    sc_in< sc_logic > load_input, save_output;
    sc_in< sc_lv<2> > input_mux_select;
    sc_in< sc_logic > reg_file_in_mux_select;
    sc_in< sc_logic > reg_file_write_enable, reg_file_read_enable;
    sc_in< sc_lv<REG_ADDRESS_SPACE> > reg_file_write_address,
                                      reg_file_read_a_address, reg_file_read_b_address;
    sc_in< sc_lv<2> > adder_mux_select, output_mux_select;
    sc_in< sc_logic > mult_enable, adder_enable;
    sc_in< sc_logic > shifter_enable, shifter_direction;
    sc_out< sc_logic > comp_eq, comp_gt, comp_lt;

    // Output Ports
    sc_out< sc_lv<32> > out_data_low;
    sc_out< sc_lv<32> > out_data_high;

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
    register_32 out_low_reg, out_high_reg;

    // Wiring Signals
    sc_signal< sc_lv<NN_DIGIT_BITS> > high_1_value, low_1_value, zero_value;
    sc_signal< sc_lv<NN_DIGIT_BITS> > b_high, b_low, c_high, c_low;
    sc_signal< sc_lv<NN_DIGIT_BITS> > input_reg_value, data_path_calculated_value, regfile_input;
    sc_signal< sc_lv<NN_DIGIT_BITS> > regfile_output_a, regfile_output_b;
    sc_signal< sc_lv<NN_DIGIT_BITS> > adder_mux_output, adder_output, shifter_output, mult_output;


    SC_CTOR (dh_hw_mult_data) : 
        high_1_const("high_1_const", 0x00010000), low_1_const("low_1_const", 0x00000001), zero_const("zero_const", 0x00000000),
        b_reg("b_reg"), c_reg("c_reg"), input_mux("input_mux"), reg_file_in_mux("reg_file_in_mux"),
        reg_file("reg_file"), mult("mult"), add("add"), shifter("shifter"), output_mux("output_mux"),
        adder_mux("adder_mux"), comparison("comparison"), out_low_reg("out_low_reg"), out_high_reg("out_high_reg")
    {
        // Constant Connections
        high_1_const.output(high_1_value);
        low_1_const.output(low_1_value);
        zero_const.output(zero_value);

        // Init Load
        b_reg.in(in_data_1);
        b_reg.out_high(b_high);
        b_reg.out_low(b_low);
        b_reg.clk(clk);
        b_reg.load(load_input);

        c_reg.in(in_data_2);
        c_reg.out_high(c_high);
        c_reg.out_low(c_low);
        c_reg.clk(clk);
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

        // Register File
        reg_file.in_a(regfile_input);
        reg_file.out_a(regfile_output_a);
        reg_file.out_b(regfile_output_b);
        reg_file.write_enable(reg_file_write_enable);
        reg_file.write_select(reg_file_write_address);
        reg_file.read_enable(reg_file_read_enable);
        reg_file.read_a_select(reg_file_read_a_address);
        reg_file.read_b_select(reg_file_read_b_address);
        reg_file.clk(clk);

        // Multiplier
        mult.in_a(regfile_output_a);
        mult.in_b(regfile_output_b);
        mult.out(mult_output);
        mult.enable(mult_enable);

        // Adder Input Mux
        adder_mux.in_a(regfile_output_b);
        adder_mux.in_b(high_1_value);
        adder_mux.in_c(low_1_value);
        adder_mux.in_d(zero_value);
        adder_mux.out(adder_mux_output);
        adder_mux.select(adder_mux_select);

        // Adder
        add.in_a(regfile_output_a);
        add.in_b(adder_mux_output);
        add.out(adder_output);
        add.enable(adder_enable);

        // Shifter
        shifter.in(regfile_output_a);
        shifter.out(shifter_output);
        shifter.direction(shifter_direction);
        shifter.enable(shifter_enable);

        // Comparator
        comparison.in_a(regfile_output_a);
        comparison.in_b(regfile_output_b);
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
        out_high_reg.in(regfile_output_a);
        out_high_reg.out(out_data_high);
        out_high_reg.clk(clk);
        out_high_reg.load(save_output);
        
        out_low_reg.in(regfile_output_b);
        out_low_reg.out(out_data_low);
        out_low_reg.clk(clk);
        out_low_reg.load(save_output);
    }
};

#endif /* end _DH_HW_MULT_DATA_H_ */