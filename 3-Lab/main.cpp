#include <fstream>
#include <iomanip>
#include <iostream>

#include "parseFuncs.h"

using namespace std;

template <typename T>
void print(T n) {
    cout << std::setfill('0') << std::setw(sizeof(T) * 2)
         << std::hex << n;
}

void printInstruction(uint32_t bits) {
    print(bits);
    cout << '\t' << praseMnemonic(bits) << '\t' << parseRd(bits) << '\t' << parseRs1(bits) << '\t' << parseRs2(bits) << ' ';
}

void printFunc(ifstream& f, const string& name, int count, int start) {
    cout << name << '\n';

    uint16_t ch = 0;
    uint32_t curInstr = 0;
    for (size_t i = 0; i < count * 4; i++) {
        if (f.read((char*)(&ch), sizeof(uint8_t))) {
            int mod = i % 4;
            curInstr += ch << mod * 8;

            if (mod == 3) {
                print((uint16_t)(start + 1 + i - 3));
                cout << ": ";
                printInstruction(curInstr);
                cout << '\n';

                curInstr = 0;
            }
        } else {
            throw new runtime_error("Reading error");
        }
    }
}

int main(int argc, char const* argv[]) {
    ifstream f("resources/out.elf", std::ios::binary);
    int mainStart = 0x54;
    int mainLengt = (0xa4 - mainStart) / 4 + 1;
    int factorialStart = 0xa8;
    int factorialLength = (0xd4 - factorialStart) / 4 + 1;

    int ctr = 0;
    uint16_t ch = 0;
    uint32_t curInstr = 0;
    while (f.read((char*)(&ch), sizeof(uint8_t))) {
        if (ctr == mainStart - 1) {
            printFunc(f, "main", mainLengt, ctr);
            ctr += mainLengt * 4;
        }
        if (ctr == factorialStart - 1) {
            printFunc(f, "factorial", factorialLength, ctr);
            ctr += factorialLength * 4;
        }
        ++ctr;
    }

    f.close();
    return 0;
}