#include "SType.h"

std::unordered_map<uint8_t, std::string> SType::mnemonics{
    {0b000, "sb"},
    {0b001, "sh"},
    {0b010, "sw"},
};