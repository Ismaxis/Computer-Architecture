#include <iostream>
#include <unordered_map>

#include "InstProps.h"
class Container {
   private:
    std::unordered_map<uint8_t, InstProps> map;

   public:
    Container() = default;
    Container(std::unordered_map<uint8_t, InstProps> map) : map(map) {}

    InstProps& getProps(uint8_t key) {
        if (map.size() == 1) {
            return map.at(0);
        } else {
            return map[key];
        }
    }
};

class MnemonicsStorage {
   private:
    MnemonicsStorage();
    static std::unordered_map<uint16_t, Container> opcodeMap;

   public:
    // static std::string getMnemonic(uint16_t opcode) {
    //     return opcodeMap[opcode].getMnemonic();
    // }
    static InstProps getProps(uint16_t opcode, uint8_t funct3) {
        if (opcodeMap.count(opcode) > 0) {
            return opcodeMap[opcode].getProps(funct3);
        } else {
            return InstProps("Unknown command", Type::UNKNOWN);
        }
    }
    static std::string getRegisterName(uint8_t index) {
        if (index == 0) {
            return "zero";
        } else if (index == 1) {
            return "ra";
        } else if (index == 2) {
            return "sp";
        } else if (index == 3) {
            return "gp";
        } else if (index == 4) {
            return "tp";
        } else if (5 <= index && index <= 7) {
            return "t" + std::to_string(index - 5);
        } else if (index == 8) {
            return "s0/fp";
        } else if (index == 9) {
            return "s1";
        } else if (10 <= index && index <= 27) {
            return "a" + std::to_string(index - 10);
        } else if (15 <= index && index <= 27) {
            return "s" + std::to_string(index - 15 + 2);
        } else if (28 <= index && index <= 31) {
            return "t" + std::to_string(index - 28 + 3);
        } else {
            return "error";
        }
    }
};

std::unordered_map<uint16_t, Container> MnemonicsStorage::opcodeMap = {
    {0b0110111, Container(std::unordered_map<uint8_t, InstProps>{
                    {0b0, InstProps("lui", Type::U)}})},
    {0b0010111, Container(std::unordered_map<uint8_t, InstProps>{
                    {0b0, InstProps("auipc", Type::U)}})},
    {0b1101111, Container(std::unordered_map<uint8_t, InstProps>{
                    {0b0, InstProps("jal", Type::J)}})},
    {0b1100111, Container(std::unordered_map<uint8_t, InstProps>{
                    {0b0, InstProps("jalr", Type::I)}})},
    {0b0000011, Container(std::unordered_map<uint8_t, InstProps>{
                    {0b000, InstProps("lb", Type::I)},
                    {0b001, InstProps("lh", Type::I)},
                    {0b010, InstProps("lw", Type::I)},
                    {0b100, InstProps("lbu", Type::I)},
                    {0b101, InstProps("lhu", Type::I)}})},
    {0b1100011, Container(std::unordered_map<uint8_t, InstProps>{
                    {0b000, InstProps("beq", Type::B)},
                    {0b001, InstProps("bne", Type::B)},
                    {0b100, InstProps("blt", Type::B)},
                    {0b101, InstProps("bge", Type::B)},
                    {0b110, InstProps("bltu", Type::B)},
                    {0b111, InstProps("bgeu", Type::B)}})},
    {0b0100011, Container(std::unordered_map<uint8_t, InstProps>{
                    {0b000, InstProps("sb", Type::S)},
                    {0b001, InstProps("sh", Type::S)},
                    {0b010, InstProps("sw", Type::S)}})},
    {0b0010011, Container(std::unordered_map<uint8_t, InstProps>{
                    {0b000, InstProps("addi", Type::I)},
                    {0b010, InstProps("slti", Type::I)},
                    {0b011, InstProps("sltiu", Type::I)},
                    {0b100, InstProps("xori", Type::I)},
                    {0b110, InstProps("ori", Type::I)},
                    {0b111, InstProps("andi", Type::I)}})},

    {0b0010011, Container(std::unordered_map<uint8_t, InstProps>{
                    {0b001, InstProps("slli", Type::R)},
                    {0b101, InstProps("srli/srai", Type::R)}})},

    {0b0110011, Container(std::unordered_map<uint8_t, InstProps>{
                    {0b000, InstProps("add/sub", Type::R)},
                    {0b001, InstProps("sll", Type::R)},
                    {0b010, InstProps("slt", Type::R)},
                    {0b011, InstProps("sltu", Type::R)},
                    {0b100, InstProps("xor", Type::R)},
                    {0b101, InstProps("srl/sra", Type::R)},
                    {0b110, InstProps("or", Type::R)},
                    {0b111, InstProps("and", Type::R)}})},
    {0b0110011, Container(std::unordered_map<uint8_t, InstProps>{
                    {0b000, InstProps("mul", Type::R)}})},
};
