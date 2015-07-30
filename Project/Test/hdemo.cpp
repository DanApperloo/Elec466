#include "systemc.h"
#include "digit.h"
#include "Driver.h"
#include "hw_components.h"
#include "hw_const.h"
#include "Viewer.h"


int sc_main(int argc , char *argv[])
{
    sc_signal< sc_uint<NN_DIGIT_BITS> > in_a, in_b;
    sc_signal< sc_uint<3> > write_select;
    sc_signal< bool > write_enable;
    sc_signal< sc_uint<3> > read_a_select, read_b_select;
    sc_signal< bool > read_enable, input_load;
    sc_signal< sc_uint<2> > input_mux_select;
    sc_signal< bool > reg_file_in_mux_select;
    sc_signal< sc_uint<NN_DIGIT_BITS> > out_a;
    sc_signal< sc_uint<NN_DIGIT_BITS> > out_b;
    sc_signal< sc_uint<NN_DIGIT_BITS> > b_high, b_low, c_high, c_low;
    sc_signal< sc_uint<NN_DIGIT_BITS> > input_reg_value, data_path_calculated_value,
                                        regfile_input;
    sc_signal< sc_uint<2> > adder_mux_select, output_mux_select;
    sc_signal< bool > mult_enable, adder_enable;
    sc_signal< bool > shifter_enable, shifter_direction;
    sc_signal< bool > comp_eq, comp_gt, comp_lt;
    sc_signal< sc_uint<NN_DIGIT_BITS> > high_1_value, low_1_value, zero_value;
    sc_signal< sc_uint<NN_DIGIT_BITS> > adder_mux_output, adder_output, shifter_output, mult_output;
    sc_clock Clk("Clock", 10, SC_NS);


    Driver driver("driver");

    hw_const<NN_DIGIT_BITS> high_1_const("high_1_const", 0x00010000),
                            low_1_const("low_1_const",   0x00000001),
                            zero_const("zero_const",     0x00000000);
    register_wt_half_index b_reg("b_reg"), c_reg("c_reg");
    mux4 input_mux("input_mux"), adder_mux("adder_mux"), output_mux("output_mux");
    mux2 reg_file_in_mux("reg_file_in_mux");
    register_file reg_file("reg_file");
    multiplier mult("mult");
    adder add("add");
    shifter16 shifter("shifter");
    comparator comparison("comparison");

    Viewer viewer("viewer");


    // Constant Connections
    high_1_const.output(high_1_value);
    low_1_const.output(low_1_value);
    zero_const.output(zero_value);

    // Driver
    driver.CLK(Clk);
    driver.in_a(in_a);
    driver.in_b(in_b);
    driver.write_enable(write_enable);
    driver.write_select(write_select);
    driver.read_a_select(read_a_select);
    driver.read_b_select(read_b_select);
    driver.read_enable(read_enable);
    driver.input_load(input_load);
    driver.input_mux_select(input_mux_select);
    driver.reg_file_in_mux_select(reg_file_in_mux_select);
    driver.adder_mux_select(adder_mux_select);
    driver.output_mux_select(output_mux_select);
    driver.mult_enable(mult_enable);
    driver.adder_enable(adder_enable);
    driver.shifter_enable(shifter_enable);
    driver.shifter_direction(shifter_direction);
    driver.comp_eq(comp_eq);
    driver.comp_gt(comp_gt);
    driver.comp_lt(comp_lt);

    // Init Load
    b_reg.in(in_a);
    b_reg.out_high(b_high);
    b_reg.out_low(b_low);
    b_reg.clk(Clk);
    b_reg.load(input_load);

    c_reg.in(in_b);
    c_reg.out_high(c_high);
    c_reg.out_low(c_low);
    c_reg.clk(Clk);
    c_reg.load(input_load);

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

    // Viewer
    viewer.out_a(out_a);
    viewer.out_b(out_b);
    
    sc_start();
    
    return(0);
}