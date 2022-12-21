#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "ElfParser.h"
#include "InstructionFabric.h"
using namespace std;

// template <typename T>
// void print(T n) {
//     cout << std::setfill('0') << std::setw(sizeof(T) * 2)
//          << std::hex << n;
// }

// void printFunc(ifstream& f, const string& name, int count, int start) {
//     cout << name << endl;

//     uint32_t read = 0;
//     for (size_t curAddr = 0; curAddr < count; curAddr += 4) {
//         if (f.read((char*)(&read), sizeof(uint32_t))) {
//             try {
//                 Instruction* curInst = InstructionFabric::createInsruction(read);
//                 curInst->setAddress(start + curAddr);
//                 cout << curInst->toString() << endl;
//                 delete curInst;
//             } catch (std::runtime_error* e) {
//                 cout << e->what() << endl;
//             }
//         } else {
//             cout << endl;
//             throw new runtime_error("Reading error");
//         }
//     }
// }

int main(int argc, char const* argv[]) {
    ifstream f;
    try {
        f.open("resources/out.elf", std::ios::binary);
        ElfParser parser = ElfParser(f);
        parser.parse();
        parser.printDotText();
        // int textSize = parser.getSizeOfText();
        // cout << textSize << "\n";
        // printFunc(f, "main/mmul", textSize, parser.getEntryPoint());
    } catch (const ifstream::failure& e) {
        cout << "ifstream::failure in main: " << e.what() << '\n';
    } catch (std::runtime_error* e) {
        std::cout << "Error in main: " << e->what() << std::endl;
    }

    f.close();
    return 0;
}