#include <iostream>
#include <unordered_map>

#include "InstProps.h"
class MnemonicsStorage {
   private:
    MnemonicsStorage();
    static std::unordered_map<uint16_t, InstProps> opcodeMap;

   public:
    static std::string getMnemonic(uint16_t opcode) {
        return opcodeMap[opcode].getMnemonic();
    }
    static InstProps getProps(uint16_t opcode) {
        return opcodeMap[opcode];
    }
    static std::string getRegisterName(uint8_t index) {
        if (index == 0) {
            return "zero";
        } else if (index == 1) {
            return "ra";
        } else if (10 <= index && index <= 27) {
            return "a" + std::to_string(index - 10);
        } else {
            return "error";
        }
    }
};

std::unordered_map<uint16_t, InstProps> MnemonicsStorage::opcodeMap = {
    {0b0000010011, InstProps("addi", Type::I)},
    {0b0001100011, InstProps("beq", Type::B)},
    {0b0011100011, InstProps("bne", Type::B)},
    {0b1001100011, InstProps("blt", Type::B)},
    {0b0000110011, InstProps("mul", Type::R)},
    {0b0010010011, InstProps("slli", Type::R)},
    {0b0100110011, InstProps("slt", Type::R)},
    {0b1010110011, InstProps("srl", Type::R)},
    {0b0010110011, InstProps("sll", Type::R)},
    {0b0001100111, InstProps("jalr", Type::I)},
};
