#include "ElfParser.h"

ElfParser::ElfParser(std::ifstream& f) : file(f) {
    elfHeader = ElfHeader();
}

void ElfParser::parse() {
    elfHeader.fill(file);
    programmHeaders = new ProgrammHeader[elfHeader.phnum];

    for (int i = 0; i < elfHeader.phnum; i++) {
        programmHeaders[i].fill(file);
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

void ElfParser::skip(int n) {
    file.read(nullptr, n);
}

ElfParser::~ElfParser() {
    delete[] programmHeaders;
}