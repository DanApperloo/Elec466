#ifndef _HW_COMPONENTS_H_
#define _HW_COMPONENTS_H_

#include "systemc.h"
#include "digit.h"


static sc_logic Z[32] = {SC_LOGIC_Z, SC_LOGIC_Z, SC_LOGIC_Z, SC_LOGIC_Z, SC_LOGIC_Z, SC_LOGIC_Z, SC_LOGIC_Z, SC_LOGIC_Z,
                         SC_LOGIC_Z, SC_LOGIC_Z, SC_LOGIC_Z, SC_LOGIC_Z, SC_LOGIC_Z, SC_LOGIC_Z, SC_LOGIC_Z, SC_LOGIC_Z,
                         SC_LOGIC_Z, SC_LOGIC_Z, SC_LOGIC_Z, SC_LOGIC_Z, SC_LOGIC_Z, SC_LOGIC_Z, SC_LOGIC_Z, SC_LOGIC_Z,
                         SC_LOGIC_Z, SC_LOGIC_Z, SC_LOGIC_Z, SC_LOGIC_Z, SC_LOGIC_Z, SC_LOGIC_Z, SC_LOGIC_Z, SC_LOGIC_Z};


/********************************************************************************/
/* Shifter Template                                                             */
/********************************************************************************/
SC_MODULE(shifter16)
{
    sc_in< sc_logic > enable;
    sc_in< sc_logic > direction;
    sc_in< sc_lv<NN_DIGIT_BITS> > in;
    sc_out< sc_lv<NN_DIGIT_BITS> > out;

    void process_shift(void)
    {
        sc_uint<32> in_sample = in.read();

        if ( SC_LOGIC_1 == enable.read() )
        {
            if ( SC_LOGIC_0 == direction.read() )
            {
                // Shift Left
                out.write((in_sample << NN_HALF_DIGIT_BITS) & 0xffff0000);
            }
            else if ( SC_LOGIC_1 == direction.read() )
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
            out.write(Z);
        }
    }

    SC_CTOR (shifter16)
    {
        SC_METHOD(process_shift);
        sensitive << in << direction << enable;
        out.initialize(Z);
    }
};


/********************************************************************************/
/* Mux2 Template                                                                */
/********************************************************************************/
SC_MODULE (mux2)
{
    sc_in< sc_lv<NN_DIGIT_BITS> > in_a;
    sc_in< sc_lv<NN_DIGIT_BITS> > in_b;
    sc_in< sc_logic > select;
    sc_out< sc_lv<NN_DIGIT_BITS> > out;


    void process_mux2(void)
    {
        if (SC_LOGIC_0 == select.read())
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
        out.initialize(Z);
    }
};

SC_MODULE (mux4)
{
    sc_in< sc_lv<NN_DIGIT_BITS> > in_a, in_b, in_c, in_d;
    sc_in< sc_lv<2> > select;
    sc_out< sc_lv<NN_DIGIT_BITS> > out;


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
        out.initialize(Z);
    }
};


/********************************************************************************/
/* Multiplier Template                                                          */
/********************************************************************************/
SC_MODULE (multiplier)
{
    sc_in< sc_logic > enable;
    sc_in< sc_lv<NN_DIGIT_BITS> > in_a;
    sc_in< sc_lv<NN_DIGIT_BITS> > in_b;
    sc_out< sc_lv<NN_DIGIT_BITS> > out;


    void process_mult(void)
    {
        sc_uint<NN_DIGIT_BITS> a_sample = in_a.read();
        sc_uint<NN_DIGIT_BITS> b_sample = in_b.read();

        if ( SC_LOGIC_1 == enable.read() )
        {
            out.write(a_sample * a_sample);
        }
        else
        {
            out.write(Z);
        }
    }

    SC_CTOR (multiplier)
    {
        SC_METHOD(process_mult);
        sensitive << in_a << in_b << enable;
        out.initialize(Z);
    }
};


/********************************************************************************/
/* Comparator Template                                                          */
/********************************************************************************/
SC_MODULE (comparator)
{
    sc_in< sc_lv<NN_DIGIT_BITS> > in_a;
    sc_in< sc_lv<NN_DIGIT_BITS> > in_b;
    sc_out< sc_logic > eq, gt, lt;


    void process_compare(void)
    {
        // Sample the input
        sc_uint<NN_DIGIT_BITS> a_sample = in_a.read();
        sc_uint<NN_DIGIT_BITS> b_sample = in_b.read();

        // Default to SC_LOGIC_0
        eq.write(SC_LOGIC_0);
        lt.write(SC_LOGIC_0);
        gt.write(SC_LOGIC_0);
    
        if (a_sample == b_sample)
        {
            eq.write(SC_LOGIC_1);
        }
        else if (a_sample > b_sample)
        {
            gt.write(SC_LOGIC_1);
        }
        else if (a_sample < b_sample)
        {
            lt.write(SC_LOGIC_1);
        }
    }

    SC_CTOR (comparator)
    {
        SC_METHOD(process_compare);
        sensitive << in_a << in_b;
        eq.initialize(SC_LOGIC_0);
        gt.initialize(SC_LOGIC_0);
        lt.initialize(SC_LOGIC_0);
    }
};


/********************************************************************************/
/* Adder Template                                                               */
/********************************************************************************/
SC_MODULE (adder)
{
    sc_in< sc_logic > enable;
    sc_in< sc_lv<NN_DIGIT_BITS> > in_a;
    sc_in< sc_lv<NN_DIGIT_BITS> > in_b;
    sc_out< sc_lv<NN_DIGIT_BITS> > out;

    void process_add(void)
    {
        sc_uint<NN_DIGIT_BITS> a_sample = in_a.read();
        sc_uint<NN_DIGIT_BITS> b_sample = in_b.read();

        if ( SC_LOGIC_1 == enable.read() )
        {
            out.write(a_sample + b_sample);
        }
        else
        {
            out.write(Z);
        }
    }

    SC_CTOR (adder)
    {
        SC_METHOD(process_add);
        sensitive << in_a << in_b << enable;
        out.initialize(Z);
    }
};


/********************************************************************************/
/* Register Template                                                            */
/********************************************************************************/
SC_MODULE (register_32)
{
    sc_in_clk clk;

    sc_in< sc_lv<NN_DIGIT_BITS> > in;
    sc_in< sc_logic > load;
    
    sc_out< sc_lv<NN_DIGIT_BITS> > out;


    void process_reg32(void)
    {
        while (true)
        {
            if ( SC_LOGIC_1 == load.read() )
            {
                out.write(in.read());
            }
            wait();
        }
    }

    SC_CTOR (register_32)
    {
        SC_CTHREAD(process_reg32, clk.pos());
        out.initialize(Z);
    }
};

SC_MODULE (register_wt_half_index)
{
    sc_in_clk clk;

    sc_in< sc_lv<NN_DIGIT_BITS> > in;
    sc_in< sc_logic > load;
    
    sc_out< sc_lv<NN_DIGIT_BITS> > out_high, out_low;


    void process_reg_with_index(void)
    {
        while (true)
        {
            if ( SC_LOGIC_1 == load.read() )
            {
                out_high.write(HIGH_HALF(in.read()));
                out_low.write(LOW_HALF(in.read()));
            }
            wait();
        }
    }

    SC_CTOR (register_wt_half_index)
    {
        SC_CTHREAD(process_reg_with_index, clk.pos());
        out_high.initialize(Z);
        out_low.initialize(Z);
    }
};

SC_MODULE (register_file)
{
    sc_in_clk clk;

    sc_in< sc_lv<NN_DIGIT_BITS> > in_a;
    sc_in< sc_lv<REG_ADDRESS_SPACE> > write_select;
    sc_in< sc_logic > write_enable;
    sc_in< sc_lv<REG_ADDRESS_SPACE> > read_a_select, read_b_select;
    sc_in< sc_logic > read_enable;

    sc_out< sc_lv<NN_DIGIT_BITS> > out_a;
    sc_out< sc_lv<NN_DIGIT_BITS> > out_b;

    sc_signal< sc_lv<NN_DIGIT_BITS> > reg_array[6];


    void process_reg_file_write(void)
    {
        while (true)
        {
            if ( SC_LOGIC_1 == write_enable.read() )
            {
                // Process Write Requests
                switch ( write_select.read().to_uint() )
                {
                    case 0:
                        reg_array[0].write(in_a.read());
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
            if ( SC_LOGIC_1 == read_enable.read() )
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

        SC_THREAD(process_reg_file_read);
        sensitive << read_a_select << read_b_select << read_enable 
        << reg_array[0] << reg_array[1] << reg_array[2] << reg_array[3];

        out_a.initialize(Z);
        out_b.initialize(Z);
    }
};


#endif // _HW_COMPONENTS_H_
