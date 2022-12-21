#include "../Instruction.h"

class BType : public Instruction {
   private:
    static std::unordered_map<uint8_t, std::string> mnemonics;

   public:
    explicit BType(uint32_t bits) : Instruction(bits) {}
    ~BType() override = default;

   private:
    std::string instructionString() const override {
        return getMnemonic() + '\t' + parseRs1(bits) + ", " + parseRs2(bits) + ", " + parseImm();
    }

    std::string parseImm() const {
        int16_t imm = 0;
        imm += isBitSet(bits, 7) ? (1 << 11) : 0;

        for (size_t i = 0; i < 4; i++) {
            imm += isBitSet(bits, i + 7 + 1) ? (1 << (i + 1)) : 0;
        }

        for (size_t i = 0; i < 6; i++) {
            imm += isBitSet(bits, i + 25) ? (1 << (i + 5)) : 0;
        }

        imm -= (bits & (1 << (25 + 6))) > 0 ? (1 << 12) : 0;

        return toHexString(address + imm);
    }

    // static void printBin(uint32_t bits) {  // TODO remove
    //     std::cout << '\n';
    //     for (int i = 31; i >= 0; i--) {
    //         if (i % 4 == 3) {
    //             std::cout << ' ';
    //         }
    //         std::cout << isBitSet(bits, i) ? '1' : '0';
    //     }
    //     std::cout << '\n';
    // }

    std::string getMnemonic() const {
        return mnemonics[parseFunct3(bits)];
    }
};
