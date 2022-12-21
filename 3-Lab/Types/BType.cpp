#include "BType.h"

std::unordered_map<uint8_t, std::string> BType::mnemonics{
    {0b000, "beq"},
    {0b001, "bne"},
    {0b100, "blt"},
    {0b101, "bge"},
    {0b110, "bltu"},
    {0b111, "bgeu"},
};