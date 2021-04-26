/*
 * MyDecoder.cpp : your custom hw decoder
 *  
 */

/** 
 *  @file    MyDecoder.cpp
 *  @author  you
 *  @date    2021
 *  @version 1.0.0 
 *  
 *  @brief   your custom hw decoder
 */

#include "mydecoder.h"

#define MyDecoder_VERBOSE 1


void my_decoder_callback(void* callbackpars, void* somedata) {
        MyDecoder* my_decoder = (MyDecoder*)(somedata);
        my_decoder->callback(
            callbackpars
        );
    }

MyDecoder::MyDecoder(AVCodecID av_codec_id, int n_buf) : Decoder(), 
    active(true), av_codec_id(av_codec_id), semaring(n_buf)
{
    // ringbuffer of AVBitmapFrame(s)
    int i;
    for(i=0; i<=n_buf; i++) {
        out_frame_rb.push_back(new AVBitmapFrame());
    }
    // interact here with your hw decoder's API
    // & instantiate all necessary objects
    // your hw decoder probably works with callbacks,
    // so give it "my_decoder_callback" & as a parameter
    // this object, aka. "this"
    //
    // If something goes wrong in the initializations,
    // (1) set the corresponding object into NULL (like in the init list of this ctor)
    // (2) set this->active to false 
    // (3) and return
}


MyDecoder::~MyDecoder()
{
#ifdef MyDecoder_VERBOSE
    std::cout << "MyDecoder:: dtor" << std::endl;
#endif
    // check every object if they are null
    // release non-null objects correctly
    for (auto it=out_frame_rb.begin(); it!=out_frame_rb.end(); ++it) {
        delete *it;
    }
    std::cout << "End of MyDecoder:: dtor" << std::endl;
}


void MyDecoder::callback(void* callbackpars) {
#ifdef MyDecoder_VERBOSE
            std::cout << "MyDecoder: enters callback successfully" << std::endl;
#endif
       { // PROTECTED
            std::unique_lock<std::mutex> lk(mutex);
            int ind = semaring.write(); // get ringbuffer index for writing the next frame decoded frame
            if (ind < 0) {
                decoderlogger.log(LogLevel::normal) << "MyDecoder: my_decoder_callback: overflow!" << std::endl;
                return;
            }
            AVBitmapFrame *f = (AVBitmapFrame*)(out_frame_rb[ind]);
            //
            // you need to access the metadata of the frame somehow
            // normally, we would do this (in order to copy the n_slot & subsession_index members)
            //
            // f->copyMetaFrom(&(in_frame));
            //
            // but that's wrong since this is an asynchronous callback, running
            // in some other thread evoked by the hw API
            //
            // if your API has the possibility to give custom
            // data at each decoding call to the API
            // (you do this calling in MyDecoder::push)
            // then take advantage of that
            //
            // if that's not the case, since n_slot & subsession_index
            // are not going to change between frames in most user cases
            // keep them constant and insert here into members n_slot & subsession_index
            //
            // next, copy your decoded frame to f->data
            // Remember that it must be in 420P, i.e.
            // completely planar 420P.  You might need
            // to deinterlace from your hw accelerator's format to 420P
        } // PROTECTED
}


void MyDecoder::deactivate(const char *err)
{
    std::cout << "MyDecoder: deactivated: " << err << std::endl;
    this->active = false;
}

bool MyDecoder::isOk()
{
    return this->active;
}

void MyDecoder::flush()
{
    if (!active)
    {
        return;
    }
    { // PROTECTED
        std::unique_lock<std::mutex> lk(this->mutex);
        semaring.reset();
    } // PROTECTED
}

Frame *MyDecoder::output()
{
    if (!active)
    {
        return NULL;
    }
    { // PROTECTED
        std::unique_lock<std::mutex> lk(this->mutex);
        int ind = semaring.getIndex();
        if (ind < 0)
        {
            return NULL;
        }
    #ifdef MyDecoder_VERBOSE
        std::cout << "MyDecoder: output: returning index " << ind << std::endl;
    #endif
        return out_frame_rb[ind];
    } // PROTECTED
}

void MyDecoder::releaseOutput()
{
    if (!active)
    {
        return;
    }
    { // PROTECTED
        std::unique_lock<std::mutex> lk(this->mutex);
        int ind = semaring.read();
    }
}


bool MyDecoder::pull()
{
    if (!active)
    {
        return false;
    }
#ifdef MyDecoder_VERBOSE
    std::cout << "MyDecoder: pull" << std::endl;
#endif

    // feed data from this->in_frame (of the class BasicFrame)
    // to your hw decoder.  Don't forget timestamps!
    // data length:
    std::size_t size_ = in_frame.payload.size();
    /* A small catch here:

    One can typically feed the hw decoder API the PTS (presentation timestamp),
    but no more custom data.  However, we'll need n_slot & subsession_index
    in function MyDecoder::callback (see that function for more details)
    */
#ifdef MyDecoder_VERBOSE
    std::cout << "timestamp: " << in_frame.mstimestamp << std::endl;
#endif
    { // PROTECTED
        // check if there is stuff in the ringbuffer
        std::unique_lock<std::mutex> lk(this->mutex);
        if (semaring.isEmpty())
        {
#ifdef MyDecoder_VERBOSE
            std::cout << "MyDecoder: pull returning false" << std::endl;
#endif
            return false;
        }
        else
        {
#ifdef MyDecoder_VERBOSE
            std::cout << "MyDecoder: pull returning true" << std::endl;
#endif
            return true;
            // The decoder thread calls MyDecoder::output() immediately after calling this method
        }
    } // PROTECTED
}

