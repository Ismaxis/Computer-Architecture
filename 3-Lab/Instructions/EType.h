#pragma once
#include <Instruction.h>

class EType : public Instruction {
   public:
    explicit EType(uint32_t bits) : Instruction(bits) {}
    ~EType() override = default;

   private:
    std::string instructionString() const override {
        return getMnemonic() + "\t\t";
    }

    std::string getMnemonic() const {
        if ((bits >> 20) == 0) {
            return "ecall";
        } else if ((bits >> 20) == 1) {
            return "ebreak";
        } else {
            return "unknown EType";
        }
    }
};