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
            symTabEntrySize = sectionHeaders[i].entsize;
            symTabEntriesCount = sectionHeaders[i].size / symTabEntrySize;  // by default ent size is 0x10
        }
    }

    fillStrTab(buff);
    fillShStrTab(buff);

    // Search for .text section
    for (int i = 1; i < elfHeader.shnum; i++) {
        if (getStringFromShStrTab(sectionHeaders[i].name) == ".text") {
            textAddress = sectionHeaders[i].offset;
            textVirtualAddress = sectionHeaders[i].addr;
            textSize = sectionHeaders[i].size;
            break;
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
    symTableEntries = new SymTabEntry[symTabEntriesCount];
    std::stringstream bufferStream;
    bufferStream.rdbuf()->pubsetbuf(buff + symTabAddress - bufferOffset, bufferSize - (symTabAddress - bufferOffset));
    for (int i = 0; i < symTabEntriesCount; i++) {
        symTableEntries[i].fill(bufferStream);
        if (symTableEntries[i].info % 0b00010000 == STT::FUNC) {
            labels[symTableEntries[i].value] = getStringFromStrTab(symTableEntries[i].name);
        }
    }

    file.close();
    delete[] buff;
}

void ElfParser::printDotText() {
    int curAddr = textVirtualAddress;
    for (int i = 0; i < instructions.size(); i++, curAddr += 4) {
        if (labels.count(curAddr) > 0) {
            printf("%08x   <%s>:\n", curAddr, labels[curAddr].c_str());
        }
        instructions.at(i)->toString();
    }
}

void ElfParser::printSymtab() {
    printf("Symbol Value          	  Size Type 	Bind 	 Vis   	   Index  Name\n");
    for (int i = 0; i < symTabEntriesCount; i++) {
        SymTabEntry curEntry = symTableEntries[i];

        printf("[%4i] 0x%-15X %5i %-8s %-8s %-8s %6s %s\n",
               i, curEntry.value, curEntry.size, toStringSTT((STT)(curEntry.info % 0b10000)).c_str(),
               toStringSTB((STB)(curEntry.info >> 4)).c_str(), toStringSTV((STV)curEntry.other).c_str(), toStringSHN((SHN)curEntry.shndx).c_str(),
               getStringFromStrTab(curEntry.name).c_str());
    }
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
        ++charsRead;
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
    delete[] symTableEntries;
    delete[] strTab;
    delete[] shStrTab;
}