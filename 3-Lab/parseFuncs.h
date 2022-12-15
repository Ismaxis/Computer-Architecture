#include "MnemonicsStorage.h"
using namespace std;

uint16_t parseOpcodeBits(uint32_t bits) {
    uint16_t opcode = 0;
    for (size_t i = 0; i < 7; i++) {
        opcode += bits & (1 << i);
    }

    return opcode;
}

uint8_t parseFunct3(uint32_t bits) {
    uint8_t funct3 = 0;

    for (size_t i = 0; i < 3; i++) {
        funct3 += (bits & (1 << (i + 12))) > 0 ? (1 << i) : 0;
    }

    return funct3;
}
uint8_t parseFunct7(uint32_t bits) {
    uint8_t funct7 = 0;

    for (size_t i = 0; i < 7; i++) {
        funct7 += (bits & (1 << (i + 25))) > 0 ? (1 << i) : 0;
    }

    return funct7;
}

string praseMnemonic(uint32_t bits) {
    int opcode = parseOpcodeBits(bits);

    int funct3 = parseFunct3(bits);

    for (size_t i = 0; i < 3; i++) {
        opcode += (funct3 & (1 << i)) > 0 ? (1 << (i + 7)) : 0;
    }

    return MnemonicsStorage::getInstruction(opcode);
}

uint8_t parseRegIndex(uint32_t bits, int startAddr) {
    uint8_t index = 0;
    for (size_t i = 0; i < 5; i++) {
        index += (bits & (1 << (i + startAddr))) > 0 ? (1 << i) : 0;
    }
    return index;
}

string parseRd(uint32_t bits) {
    return MnemonicsStorage::getRegisterName(parseRegIndex(bits, 7));
}

string parseRs1(uint32_t bits) {
    return MnemonicsStorage::getRegisterName(parseRegIndex(bits, 15));
}

string parseRs2(uint32_t bits) {
    return MnemonicsStorage::getRegisterName(parseRegIndex(bits, 20));
}
