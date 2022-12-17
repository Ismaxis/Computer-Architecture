#include <string>
#include <unordered_map>

#include "Type.h"

class Storage {
   private:
    Storage();

    static std::unordered_map<uint8_t, Type> typesMap;

   public:
    static Type getType(uint8_t opcode) {
        if (typesMap.count(opcode) > 0) {
            return typesMap[opcode];
        } else {
            return Type::UNKNOWN;
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
