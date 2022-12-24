#include "IType.h"

std::unordered_map<uint8_t, std::string> IType::mnemonics{
    {0b000, "addi"},
    {0b010, "slti"},
    {0b011, "sltiu"},
    {0b100, "xori"},
    {0b110, "ori"},
    {0b111, "andi"},

    // shamt [funct7 >> 5 | funct3]
    {0b0001, "slli"},
    {0b0101, "srli"},
    {0b1101, "srai"},
};