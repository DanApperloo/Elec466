#include "systemc.h"
#include "digit.h"
#include "dh_hw_mult_ctrl.h"


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


void dh_hw_mult_ctrl::process_ex_subfsm(void)
{
    // 10 ns
    // Load in a first value
    input_mux_select.write(B_HIGH);
    reg_file_in_mux_select.write(INPUT_LINE);
    write_enable.write(true);
    write_select.write(TEMP1_REG);
    read_enable.write(false);
    wait();

    // 20 ns
    // Load in a second value
    input_mux_select.write(B_LOW);
    reg_file_in_mux_select.write(INPUT_LINE);
    write_enable.write(true);
    write_select.write(U_REG);
    read_enable.write(false);
    wait();

    // 30 ns
    // Load in a third value
    input_mux_select.write(C_HIGH);
    reg_file_in_mux_select.write(INPUT_LINE);
    write_enable.write(true);
    write_select.write(TEMP2_REG);
    read_enable.write(false);
    wait(); 

    // 40 ns
    // Load in a fourth value
    input_mux_select.write(C_LOW);
    reg_file_in_mux_select.write(INPUT_LINE);
    write_enable.write(true);
    write_select.write(T_REG);
    read_enable.write(false);
    wait();

    // 50 ns
    // Load copy of C_Low
    input_mux_select.write(C_LOW);
    reg_file_in_mux_select.write(INPUT_LINE);
    write_select.write(A1_REG);
    write_enable.write(ON);
    read_enable.write(false);
    wait();

    // 50 ns
    // Read first pair
    write_enable.write(false);
    read_a_select.write(TEMP1_REG);
    read_b_select.write(U_REG);
    read_enable.write(true);
    wait();

    // 50 ns
    // Read second pair
    write_enable.write(false);
    read_a_select.write(TEMP2_REG);
    read_b_select.write(T_REG);
    read_enable.write(true);
    wait();

    // 60 ns
    // Multiply C_Low and B_Low
    write_select.write(A0_REG);
    write_enable.write(ON);
    reg_file_in_mux_select.write(DATA_PATH_LINE);
    read_a_select.write(T_REG);
    read_b_select.write(U_REG);
    read_enable.write(true);
    mult_enable.write(ON);
    output_mux_select.write(MULT_OUTPUT);
    wait();

    // 70 ns
    // Multiply C_High and B_Low
    write_select.write(T_REG);
    write_enable.write(ON);
    reg_file_in_mux_select.write(DATA_PATH_LINE);
    read_a_select.write(TEMP2_REG);
    read_b_select.write(U_REG);
    read_enable.write(true);
    mult_enable.write(ON);
    output_mux_select.write(MULT_OUTPUT);
    wait();

    // 80 ns
    // Multiply B_High and C_Low
    write_select.write(U_REG);
    write_enable.write(ON);
    reg_file_in_mux_select.write(DATA_PATH_LINE);
    read_a_select.write(TEMP1_REG);
    read_b_select.write(A1_REG);
    read_enable.write(true);
    mult_enable.write(ON);
    output_mux_select.write(MULT_OUTPUT);
    wait();

    // 90 ns
    // Multiply C_High and B_High
    write_select.write(A1_REG);
    write_enable.write(ON);
    reg_file_in_mux_select.write(DATA_PATH_LINE);
    read_a_select.write(TEMP2_REG);
    read_b_select.write(TEMP1_REG);
    read_enable.write(true);
    mult_enable.write(ON);
    output_mux_select.write(MULT_OUTPUT);
    wait();

    // Stop Multiply Cycles
    mult_enable.write(OFF);

    // 100 ns
    // Add U to T
    write_select.write(T_REG);
    write_enable.write(ON);
    reg_file_in_mux_select.write(DATA_PATH_LINE);
    read_a_select.write(T_REG);
    read_b_select.write(U_REG);
    read_enable.write(true);
    adder_enable.write(true);
    output_mux_select.write(ADDER_OUTPUT);
    wait();

    // Stop Adder
    adder_enable.write(OFF);

    // 110 ns
    // Perform Comparison of T and U
    write_enable.write(OFF);
    read_a_select.write(T_REG);
    read_b_select.write(U_REG);
    read_enable.write(ON);
    wait();

    // 120 ns
    // Add High 1 to A1
    if (comp_lt.read())
    {
        write_enable.write(ON);
        write_select.write(A1_REG);
        read_a_select.write(A1_REG);
        read_enable.write(ON);
        adder_mux_select.write(HIGH_VALUE_1);
        output_mux_select.write(ADDER_OUTPUT);
        reg_file_in_mux_select.write(DATA_PATH_LINE);
        adder_enable.write(ON);
        wait();
    }
    else
    {
        write_enable.write(OFF);
        wait();
    }

    // Stop Adder
    adder_enable.write(OFF);
    adder_mux_select.write(REG_FILE_B);

    // 130 ns
    // Set U to shifted T
    write_enable.write(ON);
    write_select.write(U_REG);
    read_a_select.write(T_REG);
    read_enable.write(ON);
    shifter_enable.write(ON);
    shifter_direction.write(SHIFT_LEFT);
    output_mux_select.write(SHIFTER_OUTPUT);
    reg_file_in_mux_select.write(DATA_PATH_LINE);
    wait();

    // Turn off Shifter
    shifter_enable.write(OFF);

    // 140 ns
    // Add u to A0
    write_select.write(A0_REG);
    write_enable.write(ON);
    reg_file_in_mux_select.write(DATA_PATH_LINE);
    read_a_select.write(A0_REG);
    read_b_select.write(U_REG);
    read_enable.write(true);
    adder_enable.write(true);
    output_mux_select.write(ADDER_OUTPUT);
    wait();

    // Turn off Adder
    adder_enable.write(OFF);

    // 150 ns
    // Perform Comparison of A0 and U
    write_enable.write(OFF);
    read_a_select.write(A0_REG);
    read_b_select.write(U_REG);
    read_enable.write(ON);
    wait();

    // 160 ns
    // Increment A1
    if (comp_lt.read())
    {
        write_enable.write(ON);
        write_select.write(A1_REG);
        read_a_select.write(A1_REG);
        read_enable.write(ON);
        adder_mux_select.write(LOW_VALUE_1);
        output_mux_select.write(ADDER_OUTPUT);
        reg_file_in_mux_select.write(DATA_PATH_LINE);
        adder_enable.write(ON);
        wait();
    }
    else
    {
        write_enable.write(OFF);
        wait();
    }

    // Stop Adder
    adder_enable.write(OFF);
    adder_mux_select.write(REG_FILE_B);

    // 170 ns
    // Shift T Right
    write_enable.write(ON);
    write_select.write(T_REG);
    read_a_select.write(T_REG);
    read_enable.write(ON);
    shifter_enable.write(ON);
    shifter_direction.write(SHIFT_RIGHT);
    output_mux_select.write(SHIFTER_OUTPUT);
    reg_file_in_mux_select.write(DATA_PATH_LINE);
    wait();

    // Stop Shifter
    shifter_enable.write(OFF);

    // 180 ns
    // Add High Half T to A1
    write_select.write(A1_REG);
    write_enable.write(ON);
    reg_file_in_mux_select.write(DATA_PATH_LINE);
    read_a_select.write(A1_REG);
    read_b_select.write(T_REG);
    read_enable.write(true);
    adder_enable.write(true);
    output_mux_select.write(ADDER_OUTPUT);
    wait();

    // Turn the Adder off
    adder_enable.write(OFF);

    // Read Computed Value
    write_enable.write(false);
    //printf("A0 and A1\r\n");
    read_a_select.write(A0_REG);
    read_b_select.write(A1_REG);
    read_enable.write(true);
    save_output.write(true);
    wait();
    read_enable.write(false);
    save_output.write(false);
}


void dh_hw_mult_ctrl::process_hw_mult_ctrl()
{
    // Log State
    //LOG("\r\nHW_MULT:    In State HW_MULT_WAIT.\r\n");

    while (true)
    {
        if (reset.read() == true)
        {
            current_state.write(DH_HW_MULT_WAIT);
            hw_mult_done.write(false);
        }
        else
        {
            switch (current_state.read())
            {
                case DH_HW_MULT_WAIT:
                    if (hw_mult_enable.read() == true)
                    {
                        // Save the inputs in the Datapath
                        load_input.write(true);
                        // Change the state to start the computation
                        current_state.write(DH_HW_MULT_EXECUTE);
                    }
                    break;

                case DH_HW_MULT_EXECUTE:
                    // Log State
                    //LOG("HW_MULT:    In State HW_MULT_EXECUTE.\r\n");
                    load_input.write(false);

                    // Process the Sub state
                    process_ex_subfsm();
                    current_state.write(DH_HW_MULT_OUTPUT);
                    break;

                case DH_HW_MULT_OUTPUT:
                    // Log State
                    //LOG("HW_MULT:    In State HW_MULT_OUTPUT.\r\n");

                    // Trigger the done signal
                    hw_mult_done.write(true);

                    // Change to the Finish State
                    current_state.write(DH_HW_MULT_FINISH);

                    // Log State
                    //LOG("HW_MULT:    In State HW_MULT_FINISH.\r\n");
                    break;

                case DH_HW_MULT_FINISH:
                    // Check to see if the Enable has been deasserted
                    if (hw_mult_enable.read() == false)
                    {
                        // Deassert the hw_mult_done
                        hw_mult_done.write(false);
                        current_state.write(DH_HW_MULT_WAIT);

                        // Log State
                        //LOG("HW_MULT:    In State HW_MULT_WAIT.\r\n");
                    }
                    break;
            }
        }
        wait();
    }
}
