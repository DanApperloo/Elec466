#include "systemc.h"
#include "splitter32.h"


void splitter32::process_split(void)
{
    sc_uint<32> in_sample = in.read();

    out_high.write(in_sample >> 16);
    out_low.write(in_sample & 0x00FF);
}