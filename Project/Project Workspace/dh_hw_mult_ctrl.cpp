#include "systemc.h"
#include "digit.h"
#include "dh_hw_mult_ctrl.h"


void dh_hw_mult_ctrl::process_ex_subfsm(eDH_HW_MULT_EX_SUBSTATE substate)
{
    switch (substate)
    {
        case DH_HW_MULT_EX_START:
            // Turn off the Register Load
            load_input.write(false);
            current_substate.write(DH_HW_MULT_EX_PHASE_1);
            break;

        case DH_HW_MULT_EX_PHASE_1:
            wait();
            // Trigger the First phase comparator
            if (true == phase_one_lt.read())
            {
                phase_one_select.write(false);
            }
            else
            {
                phase_one_select.write(true);
            }
            current_substate.write(DH_HW_MULT_EX_PHASE_2);
            break;

        case DH_HW_MULT_EX_PHASE_2:
            wait();
            // Save the Second Adder Phase output
            if (true == phase_two_lt.read())
            {
                phase_two_select.write(false);
            }
            else
            {
                phase_two_select.write(true);
            }
            current_substate.write(DH_HW_MULT_EX_PHASE_3);
            break;

        case DH_HW_MULT_EX_PHASE_3:
            // Save the final output and transition to done
            save_output.write(true);
            current_substate.write(DH_HW_MULT_EX_FINISH);
            break;

        case DH_HW_MULT_EX_FINISH:
            save_output.write(false);
            phase_one_select.write(false);
            phase_two_select.write(false);
            current_substate.write(DH_HW_MULT_EX_START);
            current_state.write(DH_HW_MULT_OUTPUT);
            break;
    }
}


void dh_hw_mult_ctrl::process_hw_mult_ctrl()
{
    while (true)
    {
        if (reset.read() == true)
        {
            current_state.write(DH_HW_MULT_WAIT);
            current_substate.write(DH_HW_MULT_EX_START);
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
                    // Process the Sub state
                    process_ex_subfsm(current_substate.read());
                    break;

                case DH_HW_MULT_OUTPUT:
                    // Trigger the done signal
                    hw_mult_done.write(true);

                    // Change to the Finish State
                    current_state.write(DH_HW_MULT_FINISH);
                    break;

                case DH_HW_MULT_FINISH:
                    // Check to see if the Enable has been deasserted
                    if (hw_mult_enable.read() == false)
                    {
                        // Deassert the hw_mult_done
                        hw_mult_done.write(false);
                        current_state.write(DH_HW_MULT_WAIT);
                    }
                    break;
            }
        }
        wait();
    }
}
