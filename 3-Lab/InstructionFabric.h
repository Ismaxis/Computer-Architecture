#pragma once

#include <BType.h>
#include <EType.h>
#include <IAddrType.h>
#include <IType.h>
#include <JType.h>
#include <RType.h>
#include <SType.h>
#include <UType.h>
#include <UnknownType.h>

class InstructionFabric {
   public:
    static Instruction* createInstruction(const uint32_t bits) {
        const Type type = Storage::getType(
            Instruction::parseOpcodeBits(bits));
        if (type == Type::R) {
            return new RType(bits);
        } else if (type == Type::I) {
            return new IType(bits);
        } else if (type == Type::IAddr) {
            return new IAddrType(bits);
        } else if (type == Type::S) {
            return new SType(bits);
        } else if (type == Type::B) {
            return new BType(bits);
        } else if (type == Type::U) {
            return new UType(bits);
        } else if (type == Type::J) {
            return new JType(bits);
        } else if (type == Type::E) {
            return new EType(bits);
        } else {
            return new UnknownType(bits);
        }
    }
};