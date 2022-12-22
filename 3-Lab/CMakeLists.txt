cmake_minimum_required (VERSION 3.8)

project ("RiscV-Disassembler")

set(executableName "rv3")

file (GLOB h
	"${CMAKE_CURRENT_SOURCE_DIR}/*.h"
)
file (GLOB cpp
	"${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

add_executable (${executableName} ${h} ${cpp})

add_subdirectory(Types)
add_subdirectory(Structs)
