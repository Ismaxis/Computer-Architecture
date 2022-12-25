cmake_minimum_required (VERSION 3.8)

project ("RiscV-Disassembler")

set(executableName "rv3")
set(CMAKE_CXX_STANDARD 17)

file (GLOB h
	"${CMAKE_CURRENT_SOURCE_DIR}/*.h"
)
file (GLOB cpp
	"${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

add_executable (${executableName} ${h} ${cpp})
target_include_directories(${executableName} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
add_subdirectory(Instructions)
add_subdirectory(Structs)
