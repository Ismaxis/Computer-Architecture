cmake_minimum_required (VERSION 3.8)
project("RiscV-Disassembler")
 
file(GLOB h
    "${CMAKE_CURRENT_SOURCE_DIR}/*Type.h"
)
file(GLOB cpp
    "${CMAKE_CURRENT_SOURCE_DIR}/*Type.cpp"
)


target_sources(${executableName} PRIVATE ${h} ${cpp})
target_include_directories(${executableName} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
    