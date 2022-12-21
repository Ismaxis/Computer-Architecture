#pragma once

#include "AbstractStruct.h"

class ProgrammHeader : AbstractStruct {
   public:
    Elf32_Word type;
    Elf32_Off offset;
    Elf32_Addr vaddr;
    Elf32_Addr paddr;
    Elf32_Word filesz;
    Elf32_Word memsz;
    Elf32_Word flags;
    Elf32_Word align;

    void fill(std::ifstream& f) {
        read(type, sizeof(type), f);
        read(offset, sizeof(offset), f);
        read(vaddr, sizeof(vaddr), f);
        read(paddr, sizeof(paddr), f);
        read(filesz, sizeof(filesz), f);
        read(memsz, sizeof(memsz), f);
        read(flags, sizeof(flags), f);
        read(align, sizeof(align), f);
    }
};