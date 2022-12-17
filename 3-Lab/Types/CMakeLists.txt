cmake_minimum_required (VERSION 3.8)
project("RiscV-Disassembler")
 
set(h 
    "${CMAKE_CURRENT_SOURCE_DIR}/BType.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/IAddrType.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/BType.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/IType.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/JType.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/RType.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/SType.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/UType.h"
)


target_sources(RiscV-Disassembler PRIVATE ${h})
target_include_directories(RiscV-Disassembler PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
