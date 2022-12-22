cmake_minimum_required (VERSION 3.8)
project("RiscV-Disassembler")

file (GLOB h
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
)


target_sources(${executableName} PRIVATE ${h})
target_include_directories(${executableName} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
