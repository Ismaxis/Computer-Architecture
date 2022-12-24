#pragma once

#include <ElfHeader.h>
#include <InstructionFabric.h>
#include <ProgrammHeader.h>
#include <SectionHeader.h>
#include <SymTabEntry.h>
#include <SymTabInfoEnum.h>

#include <fstream>
#include <sstream>
#include <vector>
class ElfParser {
    static constexpr uint8_t SYM_TAB = 2;
    static constexpr uint8_t STR_TAB = 3;

   public:
    explicit ElfParser(std::ifstream& f);
    ~ElfParser();

    void parse();
    void printDotText(std::ostream& out);
    void printSymtab(std::ostream& out) const;

   private:
    std::ifstream& file;

    ElfHeader elfHeader;
    ProgrammHeader* programHeaders;

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
    void fillStrTab(const char* buff);
    char* strTab;
    // SH_STR_TAB
    uint32_t shStrTabAddress;
    uint32_t shStrTabSize;
    void fillShStrTab(const char* buff);
    char* shStrTab;

    std::string getStringFromStrTab(uint32_t offset) const;
    std::string getStringFromShStrTab(uint32_t offset) const;

    // .text
    uint32_t textAddress;
    uint32_t textVirtualAddress;
    uint32_t textSize;

    std::vector<Instruction*> instructions;
    std::unordered_map<uint32_t, std::string> labels;
    // std::unordered_map<uint32_t, int> addressLabels;
};
