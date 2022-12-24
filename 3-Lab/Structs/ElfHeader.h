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
    Elf32_Half phentsize;
    Elf32_Half phnum;
    Elf32_Half shentsize;
    Elf32_Half shnum;
    Elf32_Half shstrndx;

    void fill(std::istream& f) override {
        read(name, EI_NIDENT, f);
        validateName();
        read(type, sizeof(type), f);
        read(machine, sizeof(machine), f);
        validateMachine();
        read(version, sizeof(version), f);
        validateVersion();
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
        validateShstrndx();
    }

    void validateName() {
        // ID
        const char id[] = {0x7f, 'E', 'L', 'F'};
        for (int i = 0; i < 4; i++) {
            if (name[i] != id[i]) {
                throw std::runtime_error("Invalid file identification");
            }
        }
        // CLASS
        if (name[4] != 1) {
            const std::string classStr = (name[4] == 2) ? "64-bit" : "Invalid class";
            throw std::runtime_error("Invalid class. Expected: 32-bit, Found: " + classStr);
        }
        // DATA ENCODING
        if (name[5] != 1) {
            const std::string encodingStr = (name[5] == 2) ? "ELFDATA2MSB (most significant)" : "Invalid data encoding";
            throw std::runtime_error("Invalid data encoding. Expected: ELFDATA2LSB (least significant), Found: " + encodingStr);
        }
        // VERSION
        if (name[6] != 1) {
            throw std::runtime_error("Invalid version. Expected: Current version, Found: Invalid version");
        }
    }
    void validateMachine() {
        if (machine != 243) {
            throw std::runtime_error("Invalid class. Expected: RISC-V");
        }
    }
    void validateVersion() {
        if (version != 1) {
            throw std::runtime_error("Invalid version. Expected: Current version");
        }
    }
    void validateShstrndx() {
        if (shstrndx >= shnum) {
            throw std::runtime_error("Invalid shstrndx. In should be less than shnum");
        }
    }
};