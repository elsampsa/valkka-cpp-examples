#ifndef MyThread_HEADER_GUARD
#define MyThread_HEADER_GUARD
/*
 * MyThread.h : your hw accelerated decode thread
 * 
 * Authors:
 * 
 * (c) Copyright 2021
 * 
 *            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE 
 *   TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION 
 * 
 *  0. You just DO WHAT THE FUCK YOU WANT TO. 
 * 
 */

/** 
 *  @file    MyThread.h
 *  @author  you
 *  @date    2021
 *  @version 1.0.0 
 *  
 *  @brief   your hw accelerated decode thread
 */ 

#include "example_common.h"

class MyThread : public DecoderThread { // <pyapi>
  
public: // <pyapi>
    /** Default constructor
    * 
    * @param name              Name of the thread
    * @param outfilter         Outgoing frames are written here.  Outgoing frames may be of type FrameType::avframe
    * @param fifo_ctx          Parametrization of the internal FrameFifo
    * 
    */
    MyThread(const char* name, FrameFilter& outfilter, FrameFifoContext fifo_ctx=FrameFifoContext());   // <pyapi>
    virtual ~MyThread(); ///< Default destructor.  Calls AVThread::stopCall                             // <pyapi>

protected:
    virtual Decoder* chooseAudioDecoder(AVCodecID codec_id);
    virtual Decoder* chooseVideoDecoder(AVCodecID codec_id);
    virtual Decoder* fallbackAudioDecoder(AVCodecID codec_id);
    virtual Decoder* fallbackVideoDecoder(AVCodecID codec_id);

private:
    /**my stream can be created as per MyThread (decoding thread)
    Or it could be passed to the MyThread as a parameter
    (if you want to use the same my stream for a group of decoders)
    */
    // myrtStream* my_stream;

}; // <pyapi>

#endif
