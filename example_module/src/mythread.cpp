/*
 * MyThread.cpp : your hw accelerated decoder thread
 * 
 */


/** 
 *  @file    MyThread.cpp
 *  @author  you
 *  @date    2021
 *  @version 1.0.0 
 *  
 *  @brief   your hw accelerated decoder thread
 */ 

#include "mythread.h"
#include "mydecoder.h"

MyThread::MyThread(const char* name, FrameFilter& outfilter, FrameFifoContext fifo_ctx) 
    : DecoderThread(name, outfilter, fifo_ctx)
    {
    }

MyThread::~MyThread() {

}

Decoder* MyThread::chooseAudioDecoder(AVCodecID codec_id) {
    DecoderThread::chooseAudioDecoder(codec_id);
}

Decoder* MyThread::chooseVideoDecoder(AVCodecID codec_id) {
    //TODO: try to get MyDecoder, if it's not possible, default
    //to AVDecoder
    switch (codec_id) { // switch: video codecs
        case AV_CODEC_ID_H264:
            std::cout << "Obtained MyDecoder successfully" << std::endl;
            return new MyDecoder(AV_CODEC_ID_H264);
            break;
        default:
            return NULL;
            break;        
    }
}

Decoder* MyThread::fallbackAudioDecoder(AVCodecID codec_id) {
    return DecoderThread::chooseAudioDecoder(codec_id);
}


Decoder* MyThread::fallbackVideoDecoder(AVCodecID codec_id) {
    return DecoderThread::chooseVideoDecoder(codec_id);
}
