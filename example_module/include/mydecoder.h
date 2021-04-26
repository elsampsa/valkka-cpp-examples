#ifndef MyDecoder_HEADER_GUARD
#define MyDecoder_HEADER_GUARD
/*
 * MyDecoder.h : your custom hw decoder
 * 
 */

/** 
 *  @file    MyDecoder.h
 *  @author  you
 *  @date    2021
 *  @version 1.0.0 
 *  
 *  @brief   your custom hw decoder
 */ 

#include "semaring.h"
#include "example_common.h"

void my_decoder_callback(void* callbackpars, void* some_data);

/**How decoder thread uses the decoder API
 * 
 * - myThread populates member MyDecoder::in_frame
 * - myThread calls method MyDecoder::pull
 *      - MyDecoder::pull uses MyDecoder::in_frame and myvdecSendFrame 
 *        & sends the data for decoding
 *      - Frame is decoded asynchronously, see MyDecoder::callback
 *      - MyDecoder::callback places the result into ringbuffer MyDecoder::out_frame_rb
 * - MyDecoder::pull inspects the ringbuffer.  If there is an available frame,
 *   returns true, otherwise false
 * - myThread calls immediately MyDecoder::output
 * - MyDecoder::output returns the correct out_frame (AVBitmapFrame) from the ringbuffer
 * 
 */
class MyDecoder : public Decoder {

public:
    MyDecoder(AVCodecID av_codec_id, int n_buf=5);
    virtual ~MyDecoder();

public:
    AVCodecID av_codec_id;  ///< FFmpeg AVCodecId, identifying the codec

protected:
    AVBitmapFrame out_frame;
    bool    active;

private:
    SemaRingBuffer  semaring;
    std::mutex      mutex;
    std::vector<AVBitmapFrame*> out_frame_rb;

public:
    /** Your hw API works with callbacks for sure
    *
    * let's imagine this is the callback that's called
    * once your hw has decoded a frame
    */
    void callback(void* some_data);     

public:
    virtual Frame *output();            ///< Returns a pointer to the current Frame object
    virtual void flush();               ///< Clears the decoder state
    /** pull(): Decodes a frame 
     * 
     * Please see the mother class definition here: 
     * https://elsampsa.github.io/valkka-core/html/classDecoder.html
     * 
     */
    virtual bool pull();              
    virtual void releaseOutput();
    virtual bool isOk();
    void deactivate(const char* err);
};


#endif
