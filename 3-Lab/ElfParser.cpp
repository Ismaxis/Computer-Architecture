#include "ElfParser.h"

#include <sstream>
ElfParser::ElfParser(std::ifstream& f) : file(f) {}

void ElfParser::parse() {
    // ELF HEADER
    elfHeader.fill(file);

    // PROGREMM HEADERS
    programHeaders = new ProgrammHeader[elfHeader.phnum];
    for (int i = 0; i < elfHeader.phnum; i++) {
        programHeaders[i].fill(file);
    }

    // SECTION HEADERS
    bufferOffset = elfHeader.phoff + elfHeader.phnum * elfHeader.phentsize;
    const int bufferSize = elfHeader.shoff - bufferOffset;

    const auto buff = new char[bufferSize];
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

    for (uint32_t curAddress = 0; curAddress < textSize; curAddress += 4) {
        Instruction* newInstr = InstructionFabric::createInstruction(*reinterpret_cast<uint32_t*>(&buff[curAddress]));
        newInstr->setAddress(textVirtualAddress + curAddress);
        instructions.push_back(newInstr);
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

void ElfParser::printDotText(FILE* out) {
    fprintf(out, ".text\n");
    int curAddress = textVirtualAddress;
    for (int i = 0; i < instructions.size(); i++, curAddress += 4) {
        if (labels.count(curAddress) > 0) {
            fprintf(out, "%08x   <%s>:\n", curAddress, labels[curAddress].c_str());
        }
        instructions.at(i)->toString(out);
    }
}

void ElfParser::printSymtab(FILE* out) const {
    fprintf(out, ".symtab\n");
    fprintf(out, "Symbol Value          	  Size Type 	 Bind 	 Vis   	  Index Name\n");
    for (int i = 0; i < symTabEntriesCount; i++) {
        SymTabEntry curEntry = symTableEntries[i];

        fprintf(out, "[%4i] 0x%-15X %5i %-8s %-8s %-8s %6s %s\n",
                i, curEntry.value, curEntry.size, 
				toStringSTT(static_cast<STT>(curEntry.info % 0b10000)).c_str(),
                toStringSTB(static_cast<STB>(curEntry.info >> 4)).c_str(), 
				toStringSTV(static_cast<STV>(curEntry.other)).c_str(), 
				toStringSHN(static_cast<SHN>(curEntry.shndx)).c_str(),
                getStringFromStrTab(curEntry.name).c_str());
    }
}

void ElfParser::fillStrTab(const char* buff) {
    strTab = new char[strTabSize];
    for (int j = 0; j < strTabSize; j++) {
        strTab[j] = buff[strTabAddress + j - bufferOffset];
    }
}

void ElfParser::fillShStrTab(const char* buff) {
    shStrTab = new char[shStrTabSize];
    for (int j = 0; j < shStrTabSize; j++) {
        shStrTab[j] = buff[shStrTabAddress + j - bufferOffset];
    }
}

std::string ElfParser::getStringFromStrTab(const uint32_t offset) const {
    if (offset > strTabSize) {
        throw std::runtime_error("strTab index '" + std::to_string(offset) + "' is out of bound for size '" + std::to_string(strTabSize) + "'");
    }

    std::stringstream ss;
    int charsRead = 0;
    while (strTab[offset + charsRead] != '\0') {
        ss << strTab[offset + charsRead];
        ++charsRead;
    }

    return ss.str();
}

std::string ElfParser::getStringFromShStrTab(const uint32_t offset) const {
    if (offset > shStrTabSize) {
        throw std::runtime_error("shStrTab index '" + std::to_string(offset) + "' is out of bound for size '" + std::to_string(shStrTabSize) + "'");
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
    for (const auto& instruction : instructions) {
        delete instruction;
    }
    delete[] programHeaders;
    delete[] sectionHeaders;
    delete[] symTableEntries;
    delete[] strTab;
    delete[] shStrTab;
}