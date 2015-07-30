#include "systemc.h"
#include "digit.h"
#include "Driver.h"
#include "hw_components.h"
#include "Viewer.h"


int sc_main(int argc , char *argv[])
{
    sc_signal< sc_uint<NN_DIGIT_BITS> > in_a, in_b;
    sc_signal< bool > input_load;
    sc_signal< sc_uint<NN_DIGIT_BITS> > b_high, b_low, c_high, c_low;
    sc_clock Clk("Clock", 10, SC_NS);

    
    Driver driver("driver");

    register_wt_half_index b_reg("b_reg"), c_reg("c_reg");

    Viewer viewer("viewer");

    driver.CLK(Clk);
    driver.in_a(in_a);
    driver.in_b(in_b);
    driver.input_load(input_load);

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


    viewer.b_high(b_high);
    viewer.b_low(b_low);
    viewer.c_high(c_high);
    viewer.c_low(c_low);
    
    sc_start();
    
    return(0);
}