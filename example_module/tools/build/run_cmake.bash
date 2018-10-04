#!/bin/bash
# #
# # 1) Create a separate directory for your build (let's call it $BUILD)
# # 
# # 2) Copy this script into $BUILD
# #
# # 3) Go to $BUILD and run this script there
# #
# # (consider this: https://cmake.org/pipermail/cmake/2006-October/011711.html )
# #
# # 4) Now CMake has been configured and you can run "make" in $BUILD
# #

options=""

# # Choose either one of these:
build_type="Debug"
# build_type="Release"

# # Where to find header files?
# #
# # If you have installed Valkka from a .deb package (with "dpkg -i" or with "apt-get install") there is no need to use these
# # switches : all header files have been installed to your system at the default location /usr/include/valkka
# # 
# # live555 and ffmpeg root directories are the ones where you find their "README" files.  Use absolute paths
#valkka_root=$HOME"/C/valkka/"
#live555_root=$HOME"/live555/live/"
#ffmpeg_root=$HOME"/ffmpeg/ffmpeg_git_lgpl/"

# # Where is libValkka.so ?
# #
# # Again, if you installed from .deb package, just keep this commented.  If you compiled from source, define it
#valkka_lib=$HOME"/C/valkka_builds/build_dev/lib"

# # Substitute here your absolute path to the main dir (where you have "CMakeLists.txt")
MY_CMAKE_DIR=/home/sampsa/C/valkka-cpp-examples/example_module

echo
echo $MY_CMAKE_DIR
echo
cmake $options -DCMAKE_BUILD_TYPE=$build_type -Dvalkka_lib=$valkka_lib -Dvalkka_root=$valkka_root -Dlive555_root=$live555_root -Dffmpeg_root=$ffmpeg_root $MY_CMAKE_DIR
echo
echo Run \"make\" or \"make VERBOSE=1\" to compile
echo Run \"make package\" to generate the .deb package
echo
