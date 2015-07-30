
//****************************************************************************
//****************************************************************************
//
// Copyright (c) 2002 Thorsten Groetker, Stan Liao, Grant Martin, Stuart Swan
//
// Permission is hereby granted to use, modify, and distribute this source
// code in any way as long as this entire copyright notice is retained
// in unmodified form within the source code.
//
// This software is distributed on an "AS IS" basis, without warranty
// of any kind, either express or implied.
//
// This source code is from the book "System Design with SystemC".
// For detailed discussion on this example, see the relevant section
// within the "System Design with SystemC" book.
//
// To obtain the book and find additional source code downloads, etc., visit
//     www.systemc.org 
// Look in the "Products & Solutions" -> "SystemC Books". Then look at the
// entry for "System Design with SystemC".
//
//****************************************************************************
//****************************************************************************

// Modified by Daniel Apperloo, July 12, 2015

#include "systemc.h"

#ifndef _HW_CONST_H_
#define _HW_CONST_H_

template <int S>
SC_MODULE(hw_const)
{
    sc_out< sc_lv<S> > output;

    void process_const()
    {
        output.write(constant_);
    }

    SC_HAS_PROCESS(hw_const);

    hw_const(sc_module_name NAME, const int& CONSTANT) :
        sc_module(NAME), constant_(CONSTANT) { SC_METHOD(process_const); }

    int constant_;
};

#endif // _HW_CONST_H_