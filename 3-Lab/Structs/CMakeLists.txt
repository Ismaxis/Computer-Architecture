cmake_minimum_required (VERSION 3.8)
project("RiscV-Disassembler")
 
set(h 
    "${CMAKE_CURRENT_SOURCE_DIR}/AbstractStruct.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/ElfHeader.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/ProgrammHeader.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/SectionHeader.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/SymTabEntry.h"
)


target_sources(RiscV-Disassembler PRIVATE ${h})
target_include_directories(RiscV-Disassembler PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
