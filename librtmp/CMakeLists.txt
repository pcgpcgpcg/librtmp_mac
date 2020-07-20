# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)



# specify the c++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.


add_library( # Sets the name of the library.
             rtmp

             # Sets the library as a shared library.
             STATIC

             # Provides a relative path to your source file(s).
             amf.c
             amf.h
             bytes.h
             dh.h
             dhgroups.h
             handshake.h
             hashswf.c
             http.h
             log.c
             log.h
             parseurl.c
             rtmp.c
             rtmp.h
             rtmp_sys.h)

# include_directories(librtmp)

add_definitions(-DNO_CRYPTO)
