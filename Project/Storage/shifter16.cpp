#include "systemc.h"
#include "shifter16.h"


void shifter16::process_shift(void)
{
    sc_uint<32> in_sample = in.read();

    out.write(in_sample << 16);
}

