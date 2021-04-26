# Valkka Cpp Examples

Valkka is an OpenSource media streaming library for Linux with a native Python3 interface.  For more details, see [here](https://elsampsa.github.io/valkka-examples/)

## Building extension modules

For info about Valkka extension modules, see [here](https://elsampsa.github.io/valkka-examples/_build/html/modules.html)

With Valkka, media streams (either encoded or decoded) can be copied, forked and diverted through filters and into various kinds of multithreads (live555 streaming thread, decoder threads, file writing threads, etc.).

Filters and multithreads are written at the cpp level.  The cpp code is interfaced to python, while the connection topology is programmed at the python level.

Here we provide a scaffold to create your own valkka cpp extensions that is interfaced automatically to python, under the **valkka.*** namespace.

As an example, we create the module "valkka.gizmo".  Proceed like this:

    git clone git@github.com:elsampsa/valkka-cpp-examples.git
    cd valkka-cpp-examples
    cp -r example_module gizmo
    cd gizmo
    ./reinit.bash

After that, there are two ways of compiling your custom extension

## FrameFilter and a Thread Example

Please find then in the example code

## Custom HW / GPU Decoder Example/sketch

This is included in a very sketchy way into the examples source code

libValkka employes a decoder thread (base class DecoderThread) which uses a decoder (base class Decoder) like this:

- 1. DecoderThread populates member Decoder::in_frame
- 2. DecoderThread calls method Decoder::pull
      - Decoder::pull uses Decoder::in_frame's bytebuffer and decodes the frame
      - Decoder::pull inspects the 3.rd party decoding API to see if there's a decoded frame
      - ..If there is an available frame, returns true, otherwise false
- 3. DecoderThread calls immediately Decoder::output
- 4. Decoder::output returns a pointer to the member out_frame (AVBitmapFrame)

This case applies to, for example, the ffmpeg library.

Hardware vendors, however, typically offer you an asynchronous API, so you the situation looks like this:

- 1. DecoderThread populates member Decoder::in_frame
- 2. DecoderThread calls method Decoder::pull
      - Decoder::pull uses Decoder::in_frame bytebuffer and sends the data for decoding
      - Frame is decoded asynchronously by the API, see MyDecoder::callback in the example code
      - Decoder::callback places the result into ringbuffer Decoder::out_frame_rb
      - Decoder::pull inspects the ringbuffer.  If there is an available frame, returns true, otherwise false
 - 3. DecoderThread calls immediately Decoder::output
 - 4. Decoder::output returns the correct out_frame (AVBitmapFrame) from the ringbuffer

### Valkka installed from a debian package**

If you have installed valkka with "apt-get install" or "dpkg -i", continue building your extension module with

    ./easy_build.bash
    
After that, execute

    source use_build.bash
    
which tunes your system variables so that the shared library and the python part are found by the system.
    
Next, confirm that the python interface works:

    cd python
    python3 quicktest

If no errors were present, then congrats, you have a new valkka extension, usable in python from "valkka.gizmo".

If you want to install it system-wide, continue with:

    cd ..
    cd build_dir
    make package
    sudo dpkg -i valkka_gizmo-0.1.0-Linux.deb
    
Now, from any terminal try:

    ipython3
    from valkka.gizmo import *
    
### Valkka compiled from source

If you compiled libValkka from the source code by yourself, we need a more fine-grained control over the build process.  Proceed like this:

    ./new_build.bash dev
    cd build_dev
    
In that directory, edit the "run_cmake.bash" script.  When you're ready, compile with:

    ./run_cmake.bash
    make
    
To use your development build, set 

    source test_env.bash
    
Keep the $LD_LIBRARY_PATH and $PYTHONPATH consistent (they should include correct libraries and python paths)

