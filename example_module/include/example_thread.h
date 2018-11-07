#ifndef example_thread_HEADER_GUARD
#define example_thread_HEADER_GUARD
/*
 * example_thread.h : An example thread receiving frames - and doing something with them
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
 *  @file    example_thread.h
 *  @author  Sampsa Riikonen
 *  @date    2018
 *  @version 0.1
 *  
 *  @brief   An example thread receiving frames - and doing something with them
 */ 

#include "example_common.h"

/** Signals used by ExampleThread
 */
enum class ExampleSignal {
  none,
  exit
};

  
/** Redefinition of characteristic signal contexts (info that goes with the signal)
*/
struct ExampleSignalContext {
  ExampleSignal signal;
};


/** An example thread receiving frames
 * 
 * - Request a frame filter using the getFrameFilter method
 * - Start writing frames into that FrameFilter
 * - This thread queues those frames and then uses them for something .. say, for image analysis
 * - There are two ways to communicate with the thread ..
 * - (1) Use sendSignal to send a signal context to the internal signal queue.  Thread checks it every "timeout" milliseconds.
 * - sendSignal should be called using API methods
 * - (2) Send a message frame to the FrameFilter encapsulating a signal context.  This is processed faster.
 * 
 * - WARNING: untested!
 * 
 */
class ExampleThread : public Thread {         // <pyapi>

public:                                       // <pyapi>
    ExampleThread(const char *name, FrameFifoContext fifo_ctx=FrameFifoContext());  // <pyapi>
    ~ExampleThread();                                                               // <pyapi>

protected: // frame input
    FrameFifo               infifo;           ///< Incoming frames are read from here
    FifoFrameFilter         infilter;         ///< Write incoming frames here
    BlockingFifoFrameFilter infilter_block;   ///< Incoming frames can also be written here.  If stack runs out of frames, writing will block

    const static int        timeout = 1000;   ///< Thread timeout in milliseconds
    
protected: // Thread member redefinitions
    std::deque<ExampleSignalContext> signal_fifo;   ///< Redefinition of signal fifo.
  
public: // redefined virtual functions
    void run();
    void preRun();
    void postRun();
    void sendSignal(ExampleSignalContext signal_ctx);    ///< Insert a signal into the signal_fifo
      
protected:
    void handleSignal(ExampleSignalContext &signal_ctx); ///< Handle an individual signal.  Signal can originate from the frame fifo or from the signal_fifo deque
    void handleSignals();                                ///< Call ExampleThread::handleSignal for every signal in the signal_fifo
  
// public API section
public:                                         // <pyapi>
  FifoFrameFilter &getFrameFilter();            // <pyapi>
  FifoFrameFilter &getBlockingFrameFilter();    // <pyapi>
  void requestStopCall();                       // <pyapi>
};                                              // <pyapi>


#endif
