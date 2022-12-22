#pragma once
#include <iomanip>
#include <sstream>

#include "Storage.h"

class Instruction {
   protected:
    uint32_t bits;
    uint32_t address{};

    explicit Instruction(uint32_t bits);

    std::string addressString() const;

    virtual std::string instructionString() const = 0;

   public:
    virtual void toString(FILE* out) const;

    virtual ~Instruction();

    void setAddress(uint32_t givenAddress);

    static uint8_t parseOpcodeBits(uint32_t bits);

    static uint8_t parseFunct3(uint32_t bits);

    static uint8_t parseFunct7(uint32_t bits);

    static uint8_t parseRegIndex(uint32_t bits, int startAddress);

    static std::string parseRd(uint32_t bits);

    static std::string parseRs1(uint32_t bits);

    static std::string parseRs2(uint32_t bits);

    static bool isBitSet(uint32_t bits, int index);

    template <typename T>
    static std::string toHexString(T number) {
        std::ostringstream ss;
        ss << std::setfill('0') << std::setw(sizeof(T) * 2) << std::hex << number;
        return ss.str();
    }
};