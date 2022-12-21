#pragma once

#include "AbstractStruct.h"

class SymTabEntry : AbstractStruct {
   public:
    Elf32_Word name;
    Elf32_Addr value;
    Elf32_Word size;
    unsigned char info;
    unsigned char other;
    Elf32_Half shndx;

    void fill(std::istream& f) override {
        read(name, sizeof(name), f);
        read(value, sizeof(value), f);
        read(size, sizeof(size), f);
        read(info, sizeof(info), f);
        read(other, sizeof(other), f);
        read(shndx, sizeof(shndx), f);
    }
};