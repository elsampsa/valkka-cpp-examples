# Valkka Cpp Examples

Valkka is an OpenSource video programming library for Linux with a native Python3 interface.  For more details, see [here](https://elsampsa.github.io/valkka-examples/)

## Building extension modules

With Valkka, media streams (either encoded or decoded) can be copied, forked and diverted through filters and into various kinds of multithreads (live555 streaming thread, decoder threads, file writing threads, etc.).

Filters and multithreads are written at the cpp level.  The cpp code is interfaced to python, while the connection topology is programmed at the python level.

Here we provide a scaffold to create your own valkka cpp extensions that is interfaced automatically to python, under the "valkka.*" namespace.

As an example, we create the module "valkka.gizmo".  Proceed like this:

    cp -r example_module gizmo
    cd gizmo
    ./reinit.bash

After that, there are two ways of compiling your custom extension
    
**Valkka installed from a debian package**

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
    
**Valkka compiled from source**

If you compiled libValkka from the source code by yourself, we need a more fine-grained control over the build process.  Proceed like this:

    ./new_build.bash dev
    cd build_dev
    
In that directory, edit the "run_cmake.bash" script.  When you're ready, compile with:

    ./run_cmake.bash
    make
    
To use your development build, set

    source test_env.bash
    
Keep the $LD_LIBRARY_PATH and $PYTHONPATH consistent (they should include both libraries library and python paths)

    
## Using Valkka API from Cpp

libValkka is used mainly from python.  *examples(s) coming soon*


