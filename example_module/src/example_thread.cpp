/*
 * example_thread.cpp : An example thread receiving frames - and doing something with them
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
 *  @file    example_thread.cpp
 *  @author  Sampsa Riikonen
 *  @date    2018
 *  @version 0.1
 *  
 *  @brief   An example thread receiving frames - and doing something with them
 */ 

#include "example_thread.h"

ExampleThread::ExampleThread(const char *name, FrameFifoContext fifo_ctx) : Thread(name), infifo(name,fifo_ctx), infilter(name,&infifo), infilter_block(name,&infifo) {
}
    
ExampleThread::~ExampleThread() {
}

void ExampleThread::run() {
    bool ok;
    unsigned short subsession_index;
    Frame* f;
    time_t timer;
    time_t oldtimer;
    long int dt;
    
    time(&timer);
    oldtimer=timer;
    loop=true;
    
    while(loop) {
        f=infifo.read(timeout);
        if (!f) { // TIMEOUT
            std::cout << ": "<< this->name <<" timeout expired!" << std::endl;
        }
        else { // GOT FRAME // this must ALWAYS BE ACCOMPANIED WITH A RECYCLE CALL
            // Handle signal frames
            if (f->getFrameClass()==FrameClass::signal) {
                SignalFrame *signalframe = static_cast<SignalFrame*>(f);
                if (signalframe->custom_signal_ctx!=NULL) {
                    ExampleSignalContext *signal_ctx = (ExampleSignalContext*)(signalframe->custom_signal_ctx);
                    handleSignal(*signal_ctx);
                }
            }
            else {
                std::cout << ": "<< this->name <<" got payload frame "<< *f << std::endl;
                subsession_index=f->subsession_index;
            }
            infifo.recycle(f); // always recycle
        } // GOT FRAME
        
        time(&timer);
        
        // old-style ("interrupt") signal handling
        if (difftime(timer,oldtimer)>=timeout) { // time to check the signals..
            handleSignals();
            oldtimer=timer;
        }
    }
}

void ExampleThread::preRun() {
}
    
void ExampleThread::postRun() {
}

void ExampleThread::handleSignal(ExampleSignalContext &signal_ctx) {
    switch (signal_ctx.signal) {
        case ExampleSignal::exit:
            loop=false;
            break;
    }
}

void ExampleThread::sendSignal(ExampleSignalContext signal_ctx) {
    std::unique_lock<std::mutex> lk(this->mutex);
    this->signal_fifo.push_back(signal_ctx);
}

void ExampleThread::handleSignals() {
    std::unique_lock<std::mutex> lk(this->mutex);
    // handle pending signals from the signals fifo
    for (auto it = signal_fifo.begin(); it != signal_fifo.end(); ++it) { // it == pointer to the actual object (struct SignalContext)
        handleSignal(*it);
    }
    signal_fifo.clear();
}


FifoFrameFilter &ExampleThread::getFrameFilter() {
    return infilter;
}

FifoFrameFilter &ExampleThread::getBlockingFrameFilter() {
    return (FifoFrameFilter&)infilter_block;
}

void ExampleThread::requestStopCall() {
    if (!this->has_thread) { return; } // thread never started
    if (stop_requested) { return; }    // can be requested only once
    stop_requested = true;

    // use the old-style "interrupt" way of sending signals
    ExampleSignalContext signal_ctx;
    signal_ctx.signal = ExampleSignal::exit;
    
    this->sendSignal(signal_ctx);
}


