#ifndef _HW_COMPONENTS_H_
#define _HW_COMPONENTS_H_

#include "systemc.h"
#include "digit.h"
#include "common.h"


/********************************************************************************/
/* Shifter Template                                                             */
/********************************************************************************/
SC_MODULE(shifter16)
{
    sc_in< bool > enable;
    sc_in< bool > direction;
    sc_in< sc_uint<NN_DIGIT_BITS> > in;
    sc_out< sc_uint<NN_DIGIT_BITS> > out;

    void process_shift(void)
    {
        sc_uint<32> in_sample = in.read();

        if ( true == enable.read() )
        {
            if ( false == direction.read() )
            {
                // Shift Left
                out.write((in_sample << NN_HALF_DIGIT_BITS) & 0xffff0000);
            }
            else if ( true == direction.read() )
            {
                // Shift Right
                out.write(HIGH_HALF(in_sample));
            }
            else
            {
                out.write(in_sample);
            }
        }
        else
        {
            out.write(0);
        }
    }

    SC_CTOR (shifter16)
    {
        SC_METHOD(process_shift);
        sensitive << in << direction << enable;
        out.initialize(0);
    }
};


/********************************************************************************/
/* Mux2 Template                                                                */
/********************************************************************************/
SC_MODULE (mux2)
{
    sc_in< sc_uint<NN_DIGIT_BITS> > in_a;
    sc_in< sc_uint<NN_DIGIT_BITS> > in_b;
    sc_in< bool > select;
    sc_out< sc_uint<NN_DIGIT_BITS> > out;


    void process_mux2(void)
    {
        if (false == select.read())
        {
            out.write(in_a.read());
        }
        else
        {
            out.write(in_b.read());
        }
    }

    SC_CTOR (mux2)
    {
        SC_METHOD(process_mux2);
        sensitive << in_a << in_b << select;
        out.initialize(0);
    }
};

SC_MODULE (mux4)
{
    sc_in< sc_uint<NN_DIGIT_BITS> > in_a, in_b, in_c, in_d;
    sc_in< sc_uint<2> > select;
    sc_out< sc_uint<NN_DIGIT_BITS> > out;


    void process_mux4(void)
    {
        switch ( select.read().to_uint() )
        {
            case 0:
                out.write(in_a.read());
                break;

            case 1:
                out.write(in_b.read());
                break;

            case 2:
                out.write(in_c.read());
                break;

            case 3:
                out.write(in_d.read());
                break;
        }
    }

    SC_CTOR (mux4)
    {
        SC_METHOD(process_mux4);
        sensitive << in_a << in_b << in_c << in_d << select;
        out.initialize(0);
    }
};


/********************************************************************************/
/* Multiplier Template                                                          */
/********************************************************************************/
SC_MODULE (multiplier)
{
    sc_in< bool > enable;
    sc_in< sc_uint<NN_DIGIT_BITS> > in_a;
    sc_in< sc_uint<NN_DIGIT_BITS> > in_b;
    sc_out< sc_uint<NN_DIGIT_BITS> > out;


    void process_mult(void)
    {
        sc_uint<NN_DIGIT_BITS> a_sample = in_a.read();
        sc_uint<NN_DIGIT_BITS> b_sample = in_b.read();

        if ( true == enable.read() )
        {
            out.write(a_sample * a_sample);
        }
        else
        {
            out.write(0);
        }
    }

    SC_CTOR (multiplier)
    {
        SC_METHOD(process_mult);
        sensitive << in_a << in_b << enable;
        out.initialize(0);
    }
};


/********************************************************************************/
/* Comparator Template                                                          */
/********************************************************************************/
SC_MODULE (comparator)
{
    sc_in< sc_uint<NN_DIGIT_BITS> > in_a;
    sc_in< sc_uint<NN_DIGIT_BITS> > in_b;
    sc_out< bool > eq, gt, lt;


    void process_compare(void)
    {
        // Sample the input
        sc_uint<NN_DIGIT_BITS> a_sample = in_a.read();
        sc_uint<NN_DIGIT_BITS> b_sample = in_b.read();

        // Default to SC_LOGIC_0
        eq.write(false);
        lt.write(false);
        gt.write(false);
    
        if (a_sample == b_sample)
        {
            eq.write(true);
        }
        else if (a_sample > b_sample)
        {
            gt.write(true);
        }
        else if (a_sample < b_sample)
        {
            lt.write(true);
        }
    }

    SC_CTOR (comparator)
    {
        SC_METHOD(process_compare);
        sensitive << in_a << in_b;
        eq.initialize(false);
        gt.initialize(false);
        lt.initialize(false);
    }
};


/********************************************************************************/
/* Adder Template                                                               */
/********************************************************************************/
SC_MODULE (adder)
{
    sc_in< bool > enable;
    sc_in< sc_uint<NN_DIGIT_BITS> > in_a;
    sc_in< sc_uint<NN_DIGIT_BITS> > in_b;
    sc_out< sc_uint<NN_DIGIT_BITS> > out;

    void process_add(void)
    {
        sc_uint<NN_DIGIT_BITS> a_sample = in_a.read();
        sc_uint<NN_DIGIT_BITS> b_sample = in_b.read();

        if ( true == enable.read() )
        {
            out.write(a_sample + b_sample);
        }
        else
        {
            out.write(0);
        }
    }

    SC_CTOR (adder)
    {
        SC_METHOD(process_add);
        sensitive << in_a << in_b << enable;
        out.initialize(0);
    }
};


/********************************************************************************/
/* Register Template                                                            */
/********************************************************************************/
SC_MODULE (register_32)
{
    sc_in_clk clk;

    sc_in< sc_uint<NN_DIGIT_BITS> > in;
    sc_in< bool > load;
    
    sc_out< sc_uint<NN_DIGIT_BITS> > out;


    void process_reg32(void)
    {
        while (true)
        {
            if ( true == load.read() )
            {
                out.write(in.read());
            }
            wait();
        }
    }

    SC_CTOR (register_32)
    {
        SC_CTHREAD(process_reg32, clk.pos());
        out.initialize(0);
    }
};

SC_MODULE (register_wt_half_index)
{
    sc_in_clk clk;

    sc_in< sc_uint<NN_DIGIT_BITS> > in;
    sc_in< bool > load;
    
    sc_out< sc_uint<NN_DIGIT_BITS> > out_high, out_low;


    void process_reg_with_index(void)
    {
        sc_uint<NN_DIGIT_BITS> value_sample;

        while (true)
        {
            if ( true == load.read() )
            {
                value_sample = in.read();
                out_high.write(value_sample.range(31,16));
                out_low.write(value_sample.range(15,0));
            }
            wait();
        }
    }

    SC_CTOR (register_wt_half_index)
    {
        SC_CTHREAD(process_reg_with_index, clk.pos());
        out_high.initialize(0);
        out_low.initialize(0);
    }
};

SC_MODULE (register_file)
{
    sc_in_clk clk;

    sc_in< sc_uint<NN_DIGIT_BITS> > in_a;
    sc_in< sc_uint<REG_ADDRESS_SPACE> > write_select;
    sc_in< bool > write_enable;
    sc_in< sc_uint<REG_ADDRESS_SPACE> > read_a_select, read_b_select;
    sc_in< bool > read_enable;

    sc_out< sc_uint<NN_DIGIT_BITS> > out_a;
    sc_out< sc_uint<NN_DIGIT_BITS> > out_b;


    sc_signal< sc_uint<NN_DIGIT_BITS> > reg_array[6];


    void process_reg_file_write(void)
    {
        while (true)
        {
            if ( true == write_enable.read() )
            {
                // Process Write Requests
                switch ( write_select.read().to_uint() )
                {
                    case 0:
                        reg_array[0].write(in_a.read());
                        printf("A0 Reg: %u\r\n", in_a.read());
                        break;

                    case 1:
                        reg_array[1].write(in_a.read());
                        break;

                    case 2:
                        reg_array[2].write(in_a.read());
                        break;

                    case 3:
                        reg_array[3].write(in_a.read());
                        break;

                    case 4:
                        reg_array[4].write(in_a.read());
                        break;

                    case 5:
                        reg_array[5].write(in_a.read());
                        break;
                }
            }
            wait();
        }
    }

    void process_reg_file_read(void)
    {
        while (true)
        {
            if ( true == read_enable.read() )
            {
                // Process Read Requests
                switch ( read_a_select.read().to_uint() )
                {
                    case 0:
                        out_a.write(reg_array[0]);
                        break;

                    case 1:
                        out_a.write(reg_array[1]);
                        break;

                    case 2:
                        out_a.write(reg_array[2]);
                        break;

                    case 3:
                        out_a.write(reg_array[3]);
                        break;

                    case 4:
                        out_a.write(reg_array[4]);
                        break;

                    case 5:
                        out_a.write(reg_array[5]);
                        break;
                }
                switch ( read_b_select.read().to_uint() )
                {
                    case 0:
                        out_b.write(reg_array[0]);
                        break;

                    case 1:
                        out_b.write(reg_array[1]);
                        break;

                    case 2:
                        out_b.write(reg_array[2]);
                        break;

                    case 3:
                        out_b.write(reg_array[3]);
                        break;

                    case 4:
                        out_b.write(reg_array[4]);
                        break;

                    case 5:
                        out_b.write(reg_array[5]);
                        break;
                }
            }
            wait();
        }
    }


    SC_CTOR (register_file)
    {
        SC_CTHREAD(process_reg_file_write, clk.pos());
        SC_CTHREAD(process_reg_file_read, clk.neg());

        reg_array[0].write(0);
        reg_array[1].write(0);
        reg_array[2].write(0);
        reg_array[3].write(0);
        reg_array[4].write(0);
        reg_array[5].write(0);

        out_a.initialize(0);
        out_b.initialize(0);
    }
};


#endif // _HW_COMPONENTS_H_
