cmake_minimum_required (VERSION 3.8)
project("RiscV-Disassembler")
 
file(GLOB h
    "${CMAKE_CURRENT_SOURCE_DIR}/*Type.h"
)
file(GLOB cpp
    "${CMAKE_CURRENT_SOURCE_DIR}/*Type.cpp"
)


target_sources(RiscV-Disassembler PRIVATE ${h} ${cpp})
target_include_directories(RiscV-Disassembler PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
