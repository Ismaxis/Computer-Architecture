#include <Instruction.h>

class SType : public Instruction {
   private:
    static std::unordered_map<uint8_t, std::string> mnemonics;

   public:
    explicit SType(uint32_t bits) : Instruction(bits) {}
    ~SType() override = default;

   private:
    std::string instructionString() const override {
        return getMnemonic() + '\t' + parseRs2(bits) + ", " + parseImm() + '(' + parseRs1(bits) + ')';
    }

    std::string parseImm() const {
        int16_t imm = 0;

        for (size_t i = 0; i < 5; i++) {
            imm += isBitSet(bits, i + 7) ? (1 << i) : 0;
        }

        for (size_t i = 0; i < 6; i++) {
            imm += isBitSet(bits, i + 25) ? (1 << (i + 5)) : 0;
        }

        imm -= (bits & (1 << (25 + 6))) > 0 ? (1 << 11) : 0;

        return std::to_string(imm);
    }

    std::string getMnemonic() const {
        return mnemonics[parseFunct3(bits)];
    }
};
