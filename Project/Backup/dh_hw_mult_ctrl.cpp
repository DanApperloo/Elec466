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
#define INPUT_LINE      SC_LOGIC_0
#define DATA_PATH_LINE  SC_LOGIC_1

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
#define SHIFT_RIGHT SC_LOGIC_1
#define SHIFT_LEFT  SC_LOGIC_0

#define ON  SC_LOGIC_1
#define OFF SC_LOGIC_0


void dh_hw_mult_ctrl::process_ex_subfsm(eDH_HW_MULT_EX_SUBSTATE substate)
{
    // Default Control Values
    input_mux_select.write(0);
    reg_file_in_mux_select.write(DATA_PATH_LINE);
    reg_file_write_enable.write(SC_LOGIC_0);
    reg_file_read_enable.write(ON);
    reg_file_write_address.write(0);
    reg_file_read_a_address.write(0);
    reg_file_read_b_address.write(0);
    adder_mux_select.write(0);
    output_mux_select.write(ZERO_OUTPUT);
    mult_enable.write(SC_LOGIC_0);
    adder_enable.write(SC_LOGIC_0);
    shifter_enable.write(SC_LOGIC_0);
    shifter_direction.write(SC_LOGIC_0);

    switch (substate)
    {
        case DH_HW_MULT_EX_IDLE:
            // Log State
            LOG("HW_MULT_EX: In State DH_HW_MULT_EX_IDLE.\r\n");

            // Turn off the Register Load
            load_input.write(SC_LOGIC_0);

            current_substate.write(DH_HW_MULT_EX_LD_IN_1);
            break;

        case DH_HW_MULT_EX_LD_IN_1:
            // Log State
            LOG("HW_MULT_EX: In State DH_HW_MULT_EX_LD_IN_1.\r\n");

            input_mux_select.write(B_HIGH);
            reg_file_in_mux_select.write(INPUT_LINE);
            reg_file_write_address.write(TEMP1_REG);
            reg_file_write_enable.write(ON);

            current_substate.write(DH_HW_MULT_EX_LD_IN_2);
            break;

        case DH_HW_MULT_EX_LD_IN_2:
            // Log State
            LOG("HW_MULT_EX: In State DH_HW_MULT_EX_LD_IN_2.\r\n");

            input_mux_select.write(B_LOW);
            reg_file_in_mux_select.write(INPUT_LINE);
            reg_file_write_address.write(U_REG);
            reg_file_write_enable.write(ON);

            current_substate.write(DH_HW_MULT_EX_LD_IN_3);
            break;

        case DH_HW_MULT_EX_LD_IN_3:
            // Log State
            LOG("HW_MULT_EX: In State DH_HW_MULT_EX_LD_IN_3.\r\n");

            input_mux_select.write(C_HIGH);
            reg_file_in_mux_select.write(INPUT_LINE);
            reg_file_write_address.write(TEMP2_REG);
            reg_file_write_enable.write(ON);

            current_substate.write(DH_HW_MULT_EX_LD_IN_4);
            break;

        case DH_HW_MULT_EX_LD_IN_4:
            // Log State
            LOG("HW_MULT_EX: In State DH_HW_MULT_EX_LD_IN_4.\r\n");

            input_mux_select.write(C_LOW);
            reg_file_in_mux_select.write(INPUT_LINE);
            reg_file_write_address.write(T_REG);
            reg_file_write_enable.write(ON);

            current_substate.write(DH_HW_MULT_EX_MULT_1);
            break;

        case DH_HW_MULT_EX_LD_IN_TEMP:
            // Log State
            LOG("HW_MULT_EX: In State DH_HW_MULT_EX_LD_IN_4.\r\n");

            input_mux_select.write(C_LOW);
            reg_file_in_mux_select.write(INPUT_LINE);
            reg_file_write_address.write(A1_REG);
            reg_file_write_enable.write(ON);

            current_substate.write(DH_HW_MULT_EX_MULT_1);
            break;

        case DH_HW_MULT_EX_MULT_1:
            // Log State
            LOG("HW_MULT_EX: In State DH_HW_MULT_EX_MULT_1.\r\n");

            reg_file_write_address.write(A0_REG);
            reg_file_write_enable.write(ON);
            reg_file_read_a_address.write(T_REG);
            reg_file_read_b_address.write(U_REG);
            mult_enable.write(ON);
            output_mux_select.write(MULT_OUTPUT);

            current_substate.write(DH_HW_MULT_EX_MULT_2);
            break;

        case DH_HW_MULT_EX_MULT_2:
            // Log State
            LOG("HW_MULT_EX: In State DH_HW_MULT_EX_MULT_2.\r\n");

            reg_file_write_address.write(T_REG);
            reg_file_write_enable.write(ON);
            reg_file_read_a_address.write(TEMP2_REG);
            reg_file_read_b_address.write(U_REG);
            mult_enable.write(ON);
            output_mux_select.write(MULT_OUTPUT);

            current_substate.write(DH_HW_MULT_EX_MULT_3);
            break;

        case DH_HW_MULT_EX_MULT_3:
            // Log State
            LOG("HW_MULT_EX: In State DH_HW_MULT_EX_MULT_3.\r\n");

            reg_file_write_address.write(U_REG);
            reg_file_write_enable.write(ON);
            reg_file_read_a_address.write(TEMP1_REG);
            reg_file_read_b_address.write(A1_REG);
            mult_enable.write(ON);
            output_mux_select.write(MULT_OUTPUT);

            current_substate.write(DH_HW_MULT_EX_MULT_4);
            break;

        case DH_HW_MULT_EX_MULT_4:
            // Log State
            LOG("HW_MULT_EX: In State DH_HW_MULT_EX_MULT_4.\r\n");

            reg_file_write_address.write(A1_REG);
            reg_file_write_enable.write(ON);
            reg_file_read_a_address.write(TEMP1_REG);
            reg_file_read_b_address.write(TEMP2_REG);
            mult_enable.write(ON);
            output_mux_select.write(MULT_OUTPUT);

            current_substate.write(DH_HW_MULT_EX_T_U);
            break;

        case DH_HW_MULT_EX_T_U:
            // Log State
            LOG("HW_MULT_EX: In State DH_HW_MULT_EX_T_U.\r\n");

            reg_file_write_address.write(T_REG);
            reg_file_write_enable.write(ON);
            reg_file_read_a_address.write(T_REG);
            reg_file_read_b_address.write(U_REG);
            adder_enable.write(ON);
            output_mux_select.write(ADDER_OUTPUT);

            current_substate.write(DH_HW_MULT_EX_COMPARE_1);
            break;

        case DH_HW_MULT_EX_COMPARE_1:
            // Log State
            LOG("HW_MULT_EX: In State DH_HW_MULT_EX_COMPARE_1.\r\n");

            reg_file_read_a_address.write(T_REG);
            reg_file_read_b_address.write(U_REG);

            wait();

            if (SC_LOGIC_1 == comp_lt.read())
            {
                current_substate.write(DH_HW_MULT_EX_ADD_HIGH_1);
            }
            else
            {
                current_substate.write(DH_HW_MULT_EX_SET_U);
            }
            break;

        case DH_HW_MULT_EX_ADD_HIGH_1:
            // Log State
            LOG("HW_MULT_EX: In State DH_HW_MULT_EX_ADD_HIGH_1.\r\n");

            reg_file_write_address.write(A1_REG);
            reg_file_write_enable.write(ON);
            reg_file_read_a_address.write(A1_REG);
            adder_enable.write(ON);
            adder_mux_select.write(HIGH_VALUE_1);
            output_mux_select.write(ADDER_OUTPUT);

            current_substate.write(DH_HW_MULT_EX_SET_U);
            break;

        case DH_HW_MULT_EX_SET_U:
            // Log State
            LOG("HW_MULT_EX: In State DH_HW_MULT_EX_SET_U.\r\n");

            reg_file_write_address.write(U_REG);
            reg_file_write_enable.write(ON);
            reg_file_read_a_address.write(T_REG);
            shifter_enable.write(ON);
            shifter_direction.write(SHIFT_LEFT);
            output_mux_select.write(SHIFTER_OUTPUT);

            current_substate.write(DH_HW_MULT_EX_A0_U);
            break;

        case DH_HW_MULT_EX_A0_U:
            // Log State
            LOG("HW_MULT_EX: In State DH_HW_MULT_EX_A0_U.\r\n");

            reg_file_write_address.write(A0_REG);
            reg_file_write_enable.write(ON);
            reg_file_read_a_address.write(A0_REG);
            reg_file_read_b_address.write(U_REG);
            adder_enable.write(ON);
            output_mux_select.write(ADDER_OUTPUT);

            current_substate.write(DH_HW_MULT_EX_COMPARE_2);
            break;

        case DH_HW_MULT_EX_COMPARE_2:
            // Log State
            LOG("HW_MULT_EX: In State DH_HW_MULT_EX_COMPARE_2.\r\n");

            reg_file_read_a_address.write(A0_REG);
            reg_file_read_b_address.write(U_REG);

            wait();

            if (SC_LOGIC_1 == comp_lt.read())
            {
                current_substate.write(DH_HW_MULT_EX_INCREMENT);
            }
            else
            {
                current_substate.write(DH_HW_MULT_EX_SHIFT_T);
            }
            break;

        case DH_HW_MULT_EX_INCREMENT:
            // Log State
            LOG("HW_MULT_EX: In State DH_HW_MULT_EX_INCREMENT.\r\n");

            reg_file_write_address.write(A1_REG);
            reg_file_write_enable.write(ON);
            reg_file_read_a_address.write(A1_REG);
            adder_enable.write(ON);
            adder_mux_select.write(LOW_VALUE_1);
            output_mux_select.write(ADDER_OUTPUT);

            current_substate.write(DH_HW_MULT_EX_SHIFT_T);
            break;

        case DH_HW_MULT_EX_SHIFT_T:
            // Log State
            LOG("HW_MULT_EX: In State DH_HW_MULT_EX_SHIFT_T.\r\n");

            reg_file_write_address.write(T_REG);
            reg_file_write_enable.write(ON);
            reg_file_read_a_address.write(T_REG);
            shifter_enable.write(ON);
            shifter_direction.write(SHIFT_RIGHT);
            output_mux_select.write(SHIFTER_OUTPUT);

            current_substate.write(DH_HW_MULT_EX_ADD_TO_A1);
            break;

        case DH_HW_MULT_EX_ADD_TO_A1:
            // Log State
            LOG("HW_MULT_EX: In State DH_HW_MULT_EX_ADD_TO_A1.\r\n");

            reg_file_write_address.write(A1_REG);
            reg_file_write_enable.write(ON);
            reg_file_read_a_address.write(A1_REG);
            reg_file_read_a_address.write(T_REG);
            adder_enable.write(ON);
            output_mux_select.write(ADDER_OUTPUT);

            current_substate.write(DH_HW_MULT_EX_FINISH);
            break;

        case DH_HW_MULT_EX_FINISH:
            // Log State
            LOG("HW_MULT_EX: In State HW_MULT_EX_FINISH.\r\n");
            
            current_substate.write(DH_HW_MULT_EX_IDLE);
            current_state.write(DH_HW_MULT_OUTPUT);
            break;
    }
}


void dh_hw_mult_ctrl::process_hw_mult_ctrl()
{
    // Log State
    LOG("\r\nHW_MULT:    In State HW_MULT_WAIT.\r\n");

    while (true)
    {
        if (reset.read() == true)
        {
            current_state.write(DH_HW_MULT_WAIT);
            current_substate.write(DH_HW_MULT_EX_IDLE);
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
                        load_input.write(SC_LOGIC_1);
                        // Change the state to start the computation
                        current_state.write(DH_HW_MULT_EXECUTE);
                    }
                    break;

                case DH_HW_MULT_EXECUTE:
                    // Log State
                    LOG("HW_MULT:    In State HW_MULT_EXECUTE.\r\n");

                    // Process the Sub state
                    process_ex_subfsm(current_substate.read());
                    break;

                case DH_HW_MULT_OUTPUT:
                    // Log State
                    LOG("HW_MULT:    In State HW_MULT_OUTPUT.\r\n");

                    // Trigger the done signal
                    hw_mult_done.write(true);

                    // Change to the Finish State
                    current_state.write(DH_HW_MULT_FINISH);

                    // Log State
                    LOG("HW_MULT:    In State HW_MULT_FINISH.\r\n");
                    break;

                case DH_HW_MULT_FINISH:
                    // Check to see if the Enable has been deasserted
                    if (hw_mult_enable.read() == false)
                    {
                        // Deassert the hw_mult_done
                        hw_mult_done.write(false);
                        current_state.write(DH_HW_MULT_WAIT);

                        // Log State
                        LOG("HW_MULT:    In State HW_MULT_WAIT.\r\n");
                    }
                    break;
            }
        }
        wait();
    }
}
