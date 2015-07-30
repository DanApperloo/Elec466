#include "systemc.h"
#include "digit.h"
#include "Driver.h"


void Driver::process_driver(void)
{
    // Load in a values to Input
    in_a.write((unsigned long int) 1147439747);   // Input b
    in_b.write((unsigned long int) 2147483648);   // Input c
    //in_a.write((unsigned long int) 4065718627); // Input b
    //in_b.write((unsigned long int) 3743741895); // Input c
    input_load.write(true);
    wait();
    input_load.write(false);

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
        printf("First Comparison True\r\n");
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
        printf("First Comparison False\r\n");
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
        printf("Second Comparison True\r\n");
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
        printf("Second Comparison False\r\n");
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

    // Turn off Adder
    adder_enable.write(false);

    // Read Computed Value
    write_enable.write(false);
    printf("A1 and A0\r\n");
    read_a_select.write(A1_REG);
    read_b_select.write(A0_REG);
    read_enable.write(true);
    wait();
    read_enable.write(false);

    wait();
    wait();
    wait();

    // Stop simulation
    sc_stop();
}