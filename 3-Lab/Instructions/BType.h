#include "../Instruction.h"

class BType : public Instruction {
   private:
    static std::unordered_map<uint8_t, std::string> mnemonics;

   public:
    explicit BType(uint32_t bits) : Instruction(bits) {}
    ~BType() override = default;

    bool needLabel() const override {
        return true;
    }

    uint32_t getImmAddr() const override {
        return address + getImm();
    }

   private:
    std::string instructionString() const override {
        return getMnemonic() + '\t' + parseRs1(bits) + ", " + parseRs2(bits) + ", " + parseImm();
    }

    int16_t getImm() const {
        int16_t imm = 0;
        imm += isBitSet(bits, 7) ? (1 << 11) : 0;

        for (size_t i = 0; i < 4; i++) {
            imm += isBitSet(bits, i + 7 + 1) ? (1 << (i + 1)) : 0;
        }

        for (size_t i = 0; i < 6; i++) {
            imm += isBitSet(bits, i + 25) ? (1 << (i + 5)) : 0;
        }

        imm -= (bits & (1 << (25 + 6))) > 0 ? (1 << 12) : 0;

        return imm;
    }

    std::string parseImm() const {
        return toHexString(address + getImm());
    }

    std::string getMnemonic() const {
        return mnemonics[parseFunct3(bits)];
    }
};
