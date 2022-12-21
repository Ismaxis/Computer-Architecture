#include "ElfParser.h"

#include <sstream>
ElfParser::ElfParser(std::ifstream& f) : file(f) {
    elfHeader = ElfHeader();
}

void ElfParser::parse() {
    // ELF HEADER
    elfHeader.fill(file);

    // PROGREMM HEADERS
    programmHeaders = new ProgrammHeader[elfHeader.phnum];
    for (int i = 0; i < elfHeader.phnum; i++) {
        programmHeaders[i].fill(file);
    }

    // SECTION HEADERS
    bufferOffset = elfHeader.phoff + elfHeader.phnum * elfHeader.phentsize;
    const int bufferSize = elfHeader.shoff - bufferOffset;

    char* buff = new char[bufferSize];
    for (int i = 0; i < bufferSize; i++) {
        file.read(&buff[i], sizeof(char));
    }

    sectionHeaders = new SectionHeader[elfHeader.shnum];
    for (int i = 0; i < elfHeader.shnum; i++) {
        sectionHeaders[i].fill(file);
        if (sectionHeaders[i].type == STR_TAB) {
            if (i == elfHeader.shstrndx) {
                shStrTabAddress = sectionHeaders[i].offset;
                shStrTabSize = sectionHeaders[i].size;
            } else {
                strTabAddress = sectionHeaders[i].offset;
                strTabSize = sectionHeaders[i].size;
            }
        } else if (sectionHeaders[i].type == SYM_TAB) {
            symTabAddress = sectionHeaders[i].offset;
            symTabEntriesCount = sectionHeaders[i].size / sectionHeaders[i].entsize;  // by default ent size is 0x10
        }
    }

    fillStrTab(buff);
    fillShStrTab(buff);

    // search for .text section
    for (int i = 1; i < elfHeader.shnum; i++) {
        if (getStringFromShStrTab(sectionHeaders[i].name) == ".text") {
            textAddress = sectionHeaders[i].offset;
            textVirtualAddress = sectionHeaders[i].addr;
            textSize = sectionHeaders[i].size;
        }
    }

    for (int curAddr = 0; curAddr < textSize; curAddr += 4) {
        try {
            Instruction* newInstr = InstructionFabric::createInsruction(*(uint32_t*)(&buff[curAddr]));
            newInstr->setAddress(textVirtualAddress + curAddr);
            instructions.push_back(newInstr);
        } catch (std::runtime_error* e) {
            std::cout << "Error: " << e->what() << std::endl;
        }
    }

    // SYMBOL TABLE
    // symTableEntries = new SymTabEntry[symTabEntriesCount];
    // for (int i = 0; i < symTabEntriesCount; i++) {
    //     symTableEntries[i].fill(file);
    // }

    file.close();
    delete[] buff;
}

void ElfParser::printDotText() {
    for (auto& instruction : instructions) {
        std::cout << instruction->toString() << std::endl;
    }
}

uint32_t ElfParser::getSizeOfText() {
    // вся инфа из section_table
    // 68 - .eh_frame s_size
    // эту же информацию можно найти в .text s_size
    return programmHeaders[1].offset - 68 - (elfHeader.phoff + elfHeader.phnum * elfHeader.phentsize);
}

uint32_t ElfParser::getEntryPoint() {
    return elfHeader.entry;
}

void ElfParser::fillStrTab(char* buff) {
    strTab = new char[strTabSize];
    for (int j = 0; j < strTabSize; j++) {
        strTab[j] = buff[strTabAddress + j - bufferOffset];
    }
}

void ElfParser::fillShStrTab(char* buff) {
    shStrTab = new char[shStrTabSize];
    for (int j = 0; j < shStrTabSize; j++) {
        shStrTab[j] = buff[shStrTabAddress + j - bufferOffset];
    }
}

std::string ElfParser::getStringFromStrTab(uint32_t offset) {
    if (offset > strTabSize) {
        throw new std::runtime_error("strTab index '" + std::to_string(offset) + "' is out of bound for size '" + std::to_string(strTabSize) + "'");
    }

    std::stringstream ss;
    int charsRead = 0;
    while (strTab[offset + charsRead] != '\0') {
        ss << strTab[offset + charsRead];
    }

    return ss.str();
}

std::string ElfParser::getStringFromShStrTab(uint32_t offset) {
    if (offset > shStrTabSize) {
        throw new std::runtime_error("shStrTab index '" + std::to_string(offset) + "' is out of bound for size '" + std::to_string(shStrTabSize) + "'");
    }

    std::stringstream ss;
    int charsRead = 0;
    while (shStrTab[offset + charsRead] != '\0') {
        ss << shStrTab[offset + charsRead];
        ++charsRead;
    }

    return ss.str();
}

ElfParser::~ElfParser() {
    for (auto& instruction : instructions) {
        delete instruction;
    }
    delete[] programmHeaders;
    delete[] sectionHeaders;
    // delete[] symTableEntries;
    delete[] strTab;
    delete[] shStrTab;
}