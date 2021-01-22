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
#include "numpy_no_import.h"

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


PyObject* testnumpy(PyObject* array) {
    Py_INCREF(array);
    PyArrayObject *pa = (PyArrayObject*)(array);

    // get array dimensions
    int n_dims = PyArray_NDIM(pa);
    npy_intp *dims = PyArray_DIMS(pa);

    // get data buffer
    uint8_t* buf = (uint8_t*)PyArray_BYTES(pa); // the data byte-buffer
    std::size_t size = (std::size_t)(dims[0]);
    std::cout << "got numpy array with dimension " << size << std::endl;
    // now: buf is a buffer of the data, size is it's size
    // remember to check that buf doesn't overflow
    Py_DECREF(array);
    // If you want to return True/False value
    Py_RETURN_TRUE;
    //Py_RETURN_FALSE;
}
