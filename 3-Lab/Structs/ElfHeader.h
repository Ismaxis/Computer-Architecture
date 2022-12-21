#pragma once

#include "AbstractStruct.h"

class ElfHeader : AbstractStruct {
   public:
    static const int EI_NIDENT = 16;

    unsigned char name[EI_NIDENT];
    Elf32_Half type;
    Elf32_Half machine;
    Elf32_Word version;
    Elf32_Addr entry;
    Elf32_Off phoff;
    Elf32_Off shoff;
    Elf32_Word flags;
    Elf32_Half ehsize;
    Elf32_Half phentsize;  // размер entry
    Elf32_Half phnum;      // количество entry
    Elf32_Half shentsize;
    Elf32_Half shnum;
    Elf32_Half shstrndx;

    void fill(std::istream& f) override {
        read(name, EI_NIDENT, f);
        read(type, sizeof(type), f);
        read(machine, sizeof(machine), f);
        read(version, sizeof(version), f);
        read(entry, sizeof(entry), f);
        read(phoff, sizeof(phoff), f);
        read(shoff, sizeof(shoff), f);
        read(flags, sizeof(flags), f);
        read(ehsize, sizeof(ehsize), f);
        read(phentsize, sizeof(phentsize), f);
        read(phnum, sizeof(phnum), f);
        read(shentsize, sizeof(shentsize), f);
        read(shnum, sizeof(shnum), f);
        read(shstrndx, sizeof(shstrndx), f);
    }
};