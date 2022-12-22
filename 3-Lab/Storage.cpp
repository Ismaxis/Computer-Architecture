#include "Storage.h"

std::unordered_map<uint8_t, Type> Storage::typesMap = {
    {0b0110111, Type::U},
    {0b0010111, Type::U},
    {0b1101111, Type::J},
    {0b1100111, Type::IAddr},
    {0b0000011, Type::IAddr},
    {0b1100011, Type::B},
    {0b0100011, Type::S},
    {0b0010011, Type::I},
    {0b0110011, Type::R},
    {0b1110011, Type::E},
};
