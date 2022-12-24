#include <Instruction.h>

class UnknownType : public Instruction {
   public:
    explicit UnknownType(uint32_t bits) : Instruction(bits) {}
    ~UnknownType() override = default;

   private:
    std::string instructionString() const override {
        return "unknown_instruction";
    }
};
