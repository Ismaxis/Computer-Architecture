#include <iostream>
#include <unordered_map>

class MnemonicsStorage {
   private:
    MnemonicsStorage();
    static std::unordered_map<uint16_t, std::string> opcodeMap;

   public:
    static std::string getInstruction(uint16_t opcode) {
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

std::unordered_map<uint16_t, std::string> MnemonicsStorage::opcodeMap = {
    {0b0000010011, "addi"},
    {0b0001100011, "beq"},
    {0b0011100011, "bne"},
    {0b1001100011, "blt"},
    {0b0000110011, "mul"},
    {0b0010010011, "slli"},
    {0b0100110011, "slt"},
    {0b1010110011, "srl"},
    {0b0010110011, "sll"},
    {0b0001100111, "jalr"},
};
