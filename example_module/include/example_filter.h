#ifndef example_filter_HEADER_GUARD
#define example_filter_HEADER_GUARD
/*
 * example_filter.h : An example FrameFilter, implemented in its own cpp module and interfaced to python
 * 
 * Copyright 2018 Valkka Security Ltd. and Sampsa Riikonen.
 * 
 * Authors: Sampsa Riikonen <sampsa.riikonen@iki.fi>
 * 
 * This file is part of Valkka cpp examples
 * 
 * Valkka cpp examples is free software: you can redistribute it and/or modify
 * it under the terms of the MIT License.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

/** 
 *  @file    example_filter.h
 *  @author  Sampsa Riikonen
 *  @date    2018
 *  @version 0.1
 *  
 *  @brief   An example FrameFilter, implemented in its own cpp module and interfaced to python
 */ 

#include "example_common.h"

/** A "hello world" FrameFilter demo class: prints its own name if verbose is set to true.
 */
class MyFrameFilter : public FrameFilter {                                       // <pyapi>
  
public:                                                                          // <pyapi>
  MyFrameFilter(const char* name, bool verbose=true, FrameFilter* next=NULL);    // <pyapi>
  
protected:
  bool verbose;
  
protected:
  void go(Frame* frame);
  
};                                                                               // <pyapi>


/** Test that numpy array interface works properly..
 * 
 */
PyObject* testnumpy(PyObject* array);                                            // <pyapi>
#endif

