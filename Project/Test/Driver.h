#ifndef _DRIVER_H_
#define _DRIVER_H_

#include "systemc.h"
#include "digit.h"

// Control Definitions
typedef enum _INPUT_MUX_SELECT
{
    B_HIGH = 0,
    B_LOW = 1, 
    C_HIGH = 2,
    C_LOW = 3

} INPUT_MUX_SELECT;

// enum _REG_FILE_IN_MUX_SELECT
#define INPUT_LINE      false
#define DATA_PATH_LINE  true

typedef enum _REG_FILE_ADDRESS
{
    A1_REG = 0,
    A0_REG = 1,
    U_REG = 2,
    T_REG = 3,
    TEMP1_REG = 4,
    TEMP2_REG = 5

} REG_FILE_ADDRESS;

typedef enum _ADDER_MUX_SELECT
{
    REG_FILE_B = 0,
    HIGH_VALUE_1 = 1,
    LOW_VALUE_1 = 2,
    ZERO_VALUE = 3

} ADDER_MUX_SELECT;

typedef enum _OUTPUT_MUX_SELECT
{
    MULT_OUTPUT = 0,
    ADDER_OUTPUT = 1,
    SHIFTER_OUTPUT = 2,
    ZERO_OUTPUT = 3

} OUTPUT_MUX_SELECT;

// Shifter Directions
#define SHIFT_RIGHT true
#define SHIFT_LEFT  false

#define ON  true
#define OFF false

SC_MODULE (Driver)
{
    sc_in_clk CLK;

    sc_out< sc_uint<NN_DIGIT_BITS> > in_a, in_b;
    sc_out< sc_uint<3> > write_select;
    sc_out< bool > write_enable;
    sc_out< sc_uint<3> > read_a_select, read_b_select;
    sc_out< bool > read_enable, input_load;
    sc_out< sc_uint<2> > input_mux_select;
    sc_out< bool > reg_file_in_mux_select;
    sc_out< sc_uint<2> > adder_mux_select, output_mux_select;
    sc_out< bool > mult_enable, adder_enable;
    sc_out< bool > shifter_enable, shifter_direction;
    sc_in< bool > comp_eq, comp_gt, comp_lt;

    void process_driver(void);

    SC_CTOR (Driver)
    {
        SC_CTHREAD(process_driver, CLK.pos());
    }
};

#endif // _DRIVER_H_