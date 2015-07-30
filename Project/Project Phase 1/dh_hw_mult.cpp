#include "systemc.h"
#include "digit.h"
#include "dh_hw_mult.h"


void dh_hw_mult::process_hw_mult()
{
    NN_DIGIT a[2], b, c, t, u;
    NN_HALF_DIGIT bHigh, bLow, cHigh, cLow;

    // Log State
    LOG("\r\nHW_MULT:    In State HW_MULT_WAIT.\r\n");
    
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
                        // Read inputs
                        b = in_data_1.read();
                        c = in_data_2.read();

                        // Change the state to start the computation
                        current_state.write(DH_HW_MULT_EXECUTE);
                    }
                    break;

                case DH_HW_MULT_EXECUTE:
                    // Log State
                    LOG("HW_MULT:    In State HW_MULT_EXECUTE.\r\n");

                    // Original code from NN_DigitMult()...
                    bHigh = (NN_HALF_DIGIT)HIGH_HALF (b);
                    bLow = (NN_HALF_DIGIT)LOW_HALF (b);
                    cHigh = (NN_HALF_DIGIT)HIGH_HALF (c);
                    cLow = (NN_HALF_DIGIT)LOW_HALF (c);

                    a[0] = (NN_DIGIT)bLow * (NN_DIGIT)cLow;
                    t = (NN_DIGIT)bLow * (NN_DIGIT)cHigh;
                    u = (NN_DIGIT)bHigh * (NN_DIGIT)cLow;
                    a[1] = (NN_DIGIT)bHigh * (NN_DIGIT)cHigh;
                    
                    if ((t += u) < u) a[1] += TO_HIGH_HALF (1);
                    u = TO_HIGH_HALF (t);
                    
                    if ((a[0] += u) < u) a[1]++;
                    a[1] += HIGH_HALF (t);

                    current_state.write(DH_HW_MULT_OUTPUT);
                    break;

                case DH_HW_MULT_OUTPUT:
                    // Log State
                    LOG("HW_MULT:    In State HW_MULT_OUTPUT.\r\n");

                    // Write outputs
                    out_data_low.write(a[0]);
                    out_data_high.write(a[1]);

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
