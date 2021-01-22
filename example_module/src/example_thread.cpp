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
#include "numpy_no_import.h"

ExampleThread::ExampleThread(const char *name, FrameFifoContext fifo_ctx) : Thread(name), infifo(name,fifo_ctx), infilter(name,&infifo), infilter_block(name,&infifo) {
}
    
ExampleThread::~ExampleThread() {
    std::cout << "ExampleThread : dtor " << std::endl;
}

void ExampleThread::run() {
    bool ok;
    unsigned short subsession_index;
    long int dt;
    long int mstime, oldmstime;
    Frame* f;
    
    mstime = getCurrentMsTimestamp();
    oldmstime = mstime;
    loop=true;

    while(loop) {
        f=infifo.read(timeout);
        if (!f) { // TIMEOUT
            std::cout << "ExampleThread : "<< this->name <<" timeout expired!" << std::endl;
        }
        else { // GOT FRAME // this must ALWAYS BE ACCOMPANIED WITH A RECYCLE CALL
            // Handle signal frames
            if (f->getFrameClass()==FrameClass::signal) {
                SignalFrame *signalframe = static_cast<SignalFrame*>(f);
                /*//old way
                if (signalframe->custom_signal_ctx!=NULL) {
                    ExampleSignalContext *signal_ctx = (ExampleSignalContext*)(signalframe->custom_signal_ctx);
                    handleSignal(*signal_ctx);
                }
                */

                /* at thread frontend, you would do this:
                put_signal_context(&f, signal_ctx, SignalType::example_signal);
                infilter.run(&f);
                */
                if (signalframe->signaltype==SignalType::example_signal) {
                        ExampleSignalContext signal_ctx = ExampleSignalContext();
                        get_signal_context(signalframe, signal_ctx);
                        handleSignal(signal_ctx);
                    }
            }
            else {
                std::cout << "ExampleThread : "<< this->name <<" got payload frame "<< *f << std::endl;
                subsession_index=f->subsession_index;
            }
            infifo.recycle(f); // always recycle
        } // GOT FRAME
        
        mstime = getCurrentMsTimestamp();
        dt = mstime-oldmstime;
        // old-style ("interrupt") signal handling
        if (dt>=timeout) { // time to check the signals..
            std::cout << "ExampleThread : run : interrupt, dt= " << dt << std::endl;
            handleSignals();
            oldmstime=mstime;
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

