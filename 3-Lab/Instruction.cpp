#include "Instruction.h"

Instruction::Instruction(uint32_t bits) : bits(bits){};

std::string Instruction::addressString() const {
    return toHexString(address);
}

std::string Instruction::toString() const {
    return addressString() + ":\t" + instructionString();
}

Instruction::~Instruction() = default;

void Instruction::setAddress(uint32_t givenAddress) {
    address = givenAddress;
}

uint8_t Instruction::parseOpcodeBits(uint32_t bits) {
    uint16_t opcode = 0;
    for (size_t i = 0; i < 7; i++) {
        opcode += bits & (1 << i);
    }

    return opcode;
}

uint8_t Instruction::parseFunct3(uint32_t bits) {
    uint8_t funct3 = 0;

    for (size_t i = 0; i < 3; i++) {
        funct3 += isBitSet(bits, i + 12) > 0 ? (1 << i) : 0;
    }

    return funct3;
}

uint8_t Instruction::parseFunct7(uint32_t bits) {
    uint8_t funct7 = 0;

    for (size_t i = 0; i < 7; i++) {
        funct7 += isBitSet(bits, i + 25) > 0 ? (1 << i) : 0;
    }

    return funct7;
}

uint8_t Instruction::parseRegIndex(uint32_t bits, int startAddress) {
    uint8_t index = 0;
    for (size_t i = 0; i < 5; i++) {
        index += isBitSet(bits, i + startAddress) > 0 ? (1 << i) : 0;
    }
    return index;
}

std::string Instruction::parseRd(uint32_t bits) {
    return Storage::getRegisterName(parseRegIndex(bits, 7));
}

std::string Instruction::parseRs1(uint32_t bits) {
    return Storage::getRegisterName(parseRegIndex(bits, 15));
}

std::string Instruction::parseRs2(uint32_t bits) {
    return Storage::getRegisterName(parseRegIndex(bits, 20));
}

bool Instruction::isBitSet(uint32_t bits, int index) {
    return (bits & (1 << index)) > 0;
}
