#pragma once
#include <Instruction.h>

class JType : public Instruction {
   public:
    explicit JType(uint32_t bits) : Instruction(bits) {}
    ~JType() override = default;

    bool needLabel() const override {
        return true;
    }

    uint32_t getImmAddr() const override {
        return address + getImm();
    }

   private:
    std::string instructionString() const override {
        return getMnemonic() + '\t' + parseRd(bits) + ", " + parseImm();
    }

    int32_t getImm() const {
        int32_t imm = 0;

        for (size_t i = 12; i < 20; i++) {
            imm += isBitSet(bits, i) ? (1 << i) : 0;
        }

        imm += isBitSet(bits, 20) ? (1 << 11) : 0;

        for (size_t i = 0; i < 10; i++) {
            imm += isBitSet(bits, i + 21) ? (1 << (i + 1)) : 0;
        }

        imm -= isBitSet(bits, 31) ? (1 << 20) : 0;
        return imm;
    }

    std::string parseImm() const {
        return toHexString(address + getImm());
    }

    static std::string getMnemonic() {
        return "jal";
    }
};
