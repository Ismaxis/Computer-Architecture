#include <memory>

#include "MnemonicsStorage.h"

class Instruction {
   protected:
    InstProps props;
    uint32_t bits;
    uint32_t address;
    Instruction(uint32_t bits, InstProps props) : bits(bits), props(props) {}

    std::string addressString() const {
        return toHexString(address);
    }

    virtual std::string instructionString() const = 0;

   public:
    std::string toString() const {
        return addressString() + ":\t" + instructionString();
    }

    virtual ~Instruction() = default;

    void setAddress(uint32_t givenAddress) {
        address = givenAddress;
    }

    static uint16_t parseOpcodeBits(uint32_t bits) {
        uint16_t opcode = 0;
        for (size_t i = 0; i < 7; i++) {
            opcode += bits & (1 << i);
        }

        return opcode;
    }

    static uint8_t parseFunct3(uint32_t bits) {
        uint8_t funct3 = 0;

        for (size_t i = 0; i < 3; i++) {
            funct3 += isBitSet(bits, i + 12) > 0 ? (1 << i) : 0;
        }

        return funct3;
    }
    static uint8_t parseFunct7(uint32_t bits) {
        uint8_t funct7 = 0;

        for (size_t i = 0; i < 7; i++) {
            funct7 += isBitSet(bits, i + 25) > 0 ? (1 << i) : 0;
        }

        return funct7;
    }

    static std::string praseMnemonic(uint32_t bits) {
        int opcode = parseOpcodeBits(bits);

        int funct3 = parseFunct3(bits);

        for (size_t i = 0; i < 3; i++) {
            opcode += isBitSet(bits, 1 << i) > 0 ? (1 << (i + 7)) : 0;
        }

        return MnemonicsStorage::getMnemonic(opcode);
    }

    static uint8_t parseRegIndex(uint32_t bits, int startAddr) {
        uint8_t index = 0;
        for (size_t i = 0; i < 5; i++) {
            index += isBitSet(bits, i + startAddr) > 0 ? (1 << i) : 0;
        }
        return index;
    }

    static std::string parseRd(uint32_t bits) {
        return MnemonicsStorage::getRegisterName(parseRegIndex(bits, 7));
    }

    static std::string parseRs1(uint32_t bits) {
        return MnemonicsStorage::getRegisterName(parseRegIndex(bits, 15));
    }

    static std::string parseRs2(uint32_t bits) {
        return MnemonicsStorage::getRegisterName(parseRegIndex(bits, 20));
    }

    static bool isBitSet(uint32_t bits, int index) {
        return (bits & (1 << index)) > 0;
    }

    template <typename T>
    static std::string toHexString(T number) {
        std::ostringstream ss;
        ss << std::setfill('0') << std::setw(sizeof(T) * 2) << std::hex << number;
        return ss.str();
    }
};

class RType : public Instruction {
   public:
    RType(uint32_t bits, InstProps props) : Instruction(bits, props) {}
    ~RType() = default;

    std::string instructionString() const override {
        return props.getMnemonic() + '\t' + parseRd(bits) + '\t' + parseRs1(bits) + '\t' + parseRs2(bits);
    }
};

class IType : public Instruction {
   public:
    IType(uint32_t bits, InstProps props) : Instruction(bits, props) {}
    ~IType() = default;

    std::string instructionString() const override {
        return props.getMnemonic() + '\t' + parseRd(bits) + '\t' + parseRs1(bits) + '\t' + parseImm12();
    }

   private:
    std::string parseImm12() const {
        int16_t imm12 = 0;
        for (size_t i = 0; i < 11; i++) {
            imm12 += isBitSet(bits, i + 20) > 0 ? (1 << i) : 0;
        }

        imm12 -= isBitSet(bits, 11 + 20) > 0 ? (1 << 11) : 0;

        return std::to_string(imm12);
    }
};

class BType : public Instruction {
   public:
    BType(uint32_t bits, InstProps props) : Instruction(bits, props) {}
    ~BType() = default;

   private:
    std::string instructionString() const override {
        return props.getMnemonic() + '\t' + parseRs1(bits) + '\t' + parseRs1(bits) + '\t' + parseImm();
    }

    std::string parseImm() const {
        // printBin(bits);
        int16_t imm = 0;
        imm += isBitSet(bits, 7) ? (1 << 11) : 0;

        for (size_t i = 0; i < 4; i++) {
            imm += isBitSet(bits, i + 7 + 1) ? (1 << (i + 1)) : 0;
        }

        for (size_t i = 0; i < 6; i++) {
            imm += isBitSet(bits, i + 25) ? (1 << (i + 5)) : 0;
        }

        imm -= (bits & (1 << 25 + 6)) > 0 ? (1 << 12) : 0;

        return toHexString(address + imm);
    }

    static void printBin(uint32_t bits) {
        std::cout << '\n';
        for (int i = 31; i >= 0; i--) {
            if (i % 4 == 3) {
                std::cout << ' ';
            }
            std::cout << isBitSet(bits, i) ? '1' : '0';
        }
        std::cout << '\n';
    }
};
