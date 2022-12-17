#include "../Instruction.h"

class RType : public Instruction {
   private:
    static std::unordered_map<uint16_t, std::string> mnemonics;

   public:
    explicit RType(uint32_t bits) : Instruction(bits) {}
    ~RType() override = default;

    std::string instructionString() const override {
        return praseMnemonic() + '\t' + parseRd(bits) + '\t' + parseRs1(bits) + '\t' + parseRs2(bits);
    }

   private:
    std::string praseMnemonic() const {
        uint16_t key = parseFunct7(bits) << 3 + parseFunct3(bits);
        return mnemonics[key];
    }
};

std::unordered_map<uint16_t, std::string> RType::mnemonics {
    // RV32I
    {0b0000000000, "add"},
    {0b0100000000, "sub"},
    {0b0000000001, "sll"},
    {0b0000000010, "slt"},
    {0b0000000011, "sltu"},
    {0b0000000100, "xor"},
    {0b0000000101, "srl"},
    {0b0100000101, "sra"},
    {0b0000000110, "or"},
    {0b0000000111, "and"},

    // RV32M
    {0b0000001000, "mul"},
    {0b0000001001, "mulh"},
    {0b0000001010, "mulhsu"},
    {0b0000001011, "mulhu"},
    {0b0000001100, "div"},
    {0b0000001101, "divu"},
    {0b0000001110, "rem"},
    {0b0000001111, "remu"},
};
