cmake_minimum_required (VERSION 3.8)

project ("RiscV-Disassembler")

add_executable (RiscV-Disassembler 
	main.cpp
	Instruction.h Instruction.cpp
	Storage.h Storage.cpp
	Type.h
)

add_subdirectory(Types)
