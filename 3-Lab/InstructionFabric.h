#include "Instruction.h"

class InstructionFabric {
   public:
    static Instruction* createInsruction(uint32_t bits) {
        InstProps props = MnemonicsStorage::getProps(Instruction::parseOpcodeBits(bits));
        Type type = props.getType();
        if (type == Type::R) {
            return new RType(bits, props);
        } else if (type == Type::I) {
            return new IType(bits, props);
        } else if (type == Type::B) {
            return new BType(bits, props);
        } else {
            throw new std::runtime_error("Unknown type for instruction '" + std::to_string(props.getType()) + "'");
        }
    }
};