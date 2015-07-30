#include "systemc.h"
#include "digit.h"
#include "Driver.h"


void Driver::process_driver(void)
{
    // Load in a values to Input
    in_a.write((unsigned long int) 1147439747); // Input b
    in_b.write((unsigned long int) 2147483648); // Input c
    input_load.write(true);
    wait();
    input_load.write(false);

    // 10 ns
    // Load in a first value
    wait();

    // Stop simulation
    sc_stop();
}