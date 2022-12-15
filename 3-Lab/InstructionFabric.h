#include "Instruction.h"

class InstructionFabric {
   public:
    static Instruction* createInsruction(uint32_t bits) {
        InstProps props = Storage::getProps(
            Instruction::parseOpcodeBits(bits),
            Instruction::parseFunct3(bits));
        Type type = props.getType();
        if (type == Type::R) {
            return new RType(bits, props);
        } else if (type == Type::I) {
            return new IType(bits, props);
        } else if (type == Type::IAddr) {
            return new IAddrType(bits, props);
        } else if (type == Type::S) {
            return new SType(bits, props);
        } else if (type == Type::B) {
            return new BType(bits, props);
        } else if (type == Type::U) {
            return new UType(bits, props);
        } else if (type == Type::J) {
            return new JType(bits, props);
        } else {
            throw new std::runtime_error("Unknown type '" + std::to_string(type) + "' '" + props.getMnemonic() + "'");
        }
    }
};