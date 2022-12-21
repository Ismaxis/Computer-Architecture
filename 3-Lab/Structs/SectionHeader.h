#include "AbstractStruct.h"
class SectionHeader : AbstractStruct {
   public:
    Elf32_Word name;
    Elf32_Word type;
    Elf32_Word flags;
    Elf32_Addr addr;
    Elf32_Off offset;
    Elf32_Word size;
    Elf32_Word link;
    Elf32_Word info;
    Elf32_Word addralign;
    Elf32_Word entsize;

    void fill(std::ifstream& f) {
        read(name, sizeof(name), f);
        read(type, sizeof(type), f);
        read(flags, sizeof(flags), f);
        read(addr, sizeof(addr), f);
        read(offset, sizeof(offset), f);
        read(size, sizeof(size), f);
        read(link, sizeof(link), f);
        read(info, sizeof(info), f);
        read(addralign, sizeof(addralign), f);
        read(entsize, sizeof(entsize), f);
    }
};