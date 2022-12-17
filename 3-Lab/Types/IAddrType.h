#include "../Instruction.h"

class IAddrType : public Instruction {
   private:
    static std::unordered_map<uint8_t, std::string> mnemonics;

   public:
    explicit IAddrType(uint32_t bits) : Instruction(bits) {}
    ~IAddrType() override = default;

    std::string instructionString() const override {
        return getMnemonic() + '\t' + parseRd(bits) + '\t' + parseImm12() + '(' + parseRs1(bits) + ')';
    }

   private:
    std::string parseImm12() const {
        int16_t imm12 = 0;
        for (size_t i = 0; i < 11; i++) {
            imm12 += isBitSet(bits, i + 20) > 0 ? (1 << i) : 0;
        }

        imm12 -= isBitSet(bits, 11 + 20) > 0 ? (1 << 11) : 0;

        return std::to_string(imm12);
    }

    std::string getMnemonic() const {
        uint8_t key = (parseFunct3(bits) << 1) + (isBitSet(bits, 6) ? 1 : 0);
        return mnemonics[key];
    }
};
// [funct3 | opcode[6]]
std::unordered_map<uint8_t, std::string> IAddrType::mnemonics{
    {0b0001, "jalr"},
    {0b0000, "lb"},
    {0b0010, "lh"},
    {0b0100, "lw"},
    {0b1000, "lbu"},
    {0b1010, "lhu"},
};