/*
 * example_filter.cpp : An example FrameFilter, implemented in its own cpp module and interfaced to python
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
 *  @file    example_filter.cpp
 *  @author  Sampsa Riikonen
 *  @date    2018
 *  @version 0.1
 *  
 *  @brief   An example FrameFilter, implemented in its own cpp module and interfaced to python
 */ 

#include "example_filter.h"

// subclass like this:
MyFrameFilter::MyFrameFilter(const char* name, bool verbose, FrameFilter* next) : FrameFilter(name,next), verbose(verbose) {
  // std::cout << ">>>>>>" << verbose << std::endl;
}
  
void MyFrameFilter::go(Frame* frame) { 
  if (verbose) {
    // std::cout << "MyFrameFilter : "<< this->name << " " << verbose << " : got frame : " << *(frame) << std::endl;
    std::cout << "MyFrameFilter : "<< this->name << " : got frame : " << *(frame) << std::endl;
  }
}

