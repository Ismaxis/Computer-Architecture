#pragma once

#include <fstream>
#include <vector>

#include "InstructionFabric.h"
#include "Structs/ElfHeader.h"
#include "Structs/ProgrammHeader.h"
#include "Structs/SectionHeader.h"
#include "Structs/SymTabEntry.h"
class ElfParser {
    static const uint8_t SYM_TAB = 2;
    static const uint8_t STR_TAB = 3;
    static const uint8_t STT_FUNC = 2;

   public:
    ElfParser(std::ifstream& f);
    ~ElfParser();

    void parse();
    void printDotText();

   private:
    std::ifstream& file;

    ElfHeader elfHeader;
    ProgrammHeader* programmHeaders;

    int bufferOffset;  // offset of address in buff relative to file
    SectionHeader* sectionHeaders;

    // SYM_TAB
    uint32_t symTabAddress;
    uint32_t symTabEntrySize;
    uint32_t symTabEntriesCount;
    SymTabEntry* symTableEntries;
    // STR_TAB
    uint32_t strTabAddress;
    uint32_t strTabSize;
    void fillStrTab(char* buff);
    char* strTab;
    // SH_STR_TAB
    uint32_t shStrTabAddress;
    uint32_t shStrTabSize;
    void fillShStrTab(char* buff);
    char* shStrTab;

    std::string getStringFromStrTab(uint32_t offset);
    std::string getStringFromShStrTab(uint32_t offset);

    // .text
    uint32_t textAddress;
    uint32_t textVirtualAddress;
    uint32_t textSize;

    std::vector<Instruction*> instructions;
    std::unordered_map<uint32_t, std::string> labels;
};
