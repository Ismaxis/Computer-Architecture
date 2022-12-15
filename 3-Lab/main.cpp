#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "InstructionFabric.h"
using namespace std;

template <typename T>
void print(T n) {
    cout << std::setfill('0') << std::setw(sizeof(T) * 2)
         << std::hex << n;
}

void printFunc(ifstream& f, const string& name, int count, int start) {
    cout << name << '\n';

    uint32_t read = 0;

    for (size_t curAddr = 0; curAddr < count * 4; curAddr += 4) {
        if (f.read((char*)(&read), sizeof(uint32_t))) {
            try {
                Instruction* curInst = InstructionFabric::createInsruction(read);
                curInst->setAddress(start + curAddr);
                cout << curInst->toString();
                delete curInst;
            } catch (std::runtime_error* e) {
                cout << e->what();
            }
            cout << '\n';
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
    uint32_t read = 0;
    while (f.read((char*)(&read), sizeof(uint32_t))) {
        if (ctr == mainStart - 4) {
            printFunc(f, "main", mainLengt, mainStart);
            ctr += mainLengt * 4;
        }
        if (ctr == factorialStart - 4) {
            printFunc(f, "factorial", factorialLength, factorialStart);
            ctr += factorialLength * 4;
        }
        ctr += 4;
    }

    f.close();
    return 0;
}