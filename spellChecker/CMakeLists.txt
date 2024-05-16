cmake_minimum_required(VERSION 3.26)
project(spellChecker)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(spellChecker
        hashtable.h
        hashtable.hpp
        makefile
        myspell.cpp)
