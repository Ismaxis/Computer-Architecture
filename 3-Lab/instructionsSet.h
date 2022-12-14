#include <iostream>
#include <unordered_map>

class instructionsSet {
   private:
    instructionsSet();
    std::unordered_map<int, std::string> map{
        {0b00010011, "addi"},
        {0b01100011, "beq/bne"},
        {0b00110011, "mul"},
        {0b00010011, "slli"},
        {0b00110011, "slt"},
        {0b01100111, "jalr"},
    };

   public:
    ~instructionsSet();

    std::string getInstruction(int ch) {
        return map[ch];
    }
};
