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
    cout << name << endl;

    uint32_t read = 0;
    for (size_t curAddr = 0; curAddr < count * 4; curAddr += 4) {
        if (f.read((char*)(&read), sizeof(uint32_t))) {
            try {
                Instruction* curInst = InstructionFabric::createInsruction(read);
                curInst->setAddress(start + curAddr);
                cout << curInst->toString() << endl;
                delete curInst;
            } catch (std::runtime_error* e) {
                cout << e->what();
            }
        } else {
            cout << endl;
            throw new runtime_error("Reading error");
        }
    }
}

int main(int argc, char const* argv[]) {
    int mainStart = 0x74;
    int mainLengt = (0xac - mainStart) / 4 + 1;
    int factorialStart = 0xb0;
    int factorialLength = (0x124 - factorialStart) / 4 + 1;

    int ctr = 0;
    uint32_t read = 0;

    ifstream f;
    try {
        f.open("resources/out.elf", std::ios::binary);
        while (f.read((char*)(&read), sizeof(uint32_t))) {
            if (ctr == mainStart - 4) {
                printFunc(f, "main", mainLengt, mainStart);
                ctr += mainLengt * 4;
            }
            if (ctr == factorialStart - 4) {
                cout << endl;
                printFunc(f, "mmul", factorialLength, factorialStart);
                ctr += factorialLength * 4;
            }
            ctr += 4;
        }
    } catch (const ifstream::failure& e) {
        cout << e.what() << '\n';
    }

    f.close();
    return 0;
}