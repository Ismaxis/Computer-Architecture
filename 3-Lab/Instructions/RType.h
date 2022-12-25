#include <Instruction.h>

class RType : public Instruction {
   private:
    static std::unordered_map<uint16_t, std::string> mnemonics;

   public:
    explicit RType(uint32_t bits) : Instruction(bits) {}
    ~RType() override = default;

    std::string instructionString() const override {
        return getMnemonic() + '\t' + parseRd(bits) + ", " + parseRs1(bits) + ", " + parseRs2(bits);
    }

   private:
    std::string getMnemonic() const {
        uint16_t key = (parseFunct7(bits) << 3) + parseFunct3(bits);
        return mnemonics[key];
    }
};
