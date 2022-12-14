#include <fstream>
#include <iomanip>
#include <iostream>
using namespace std;

template <typename T>
void print(T n) {
    cout << std::setfill('0') << std::setw(sizeof(T) * 2)
         << std::hex << n;
}

void printFunc(ifstream& f, const string& name, int count) {
    cout << name << '\n';

    uint16_t ch = 0;
    uint32_t curInstr = 0;
    for (size_t i = 0; i < count * 4; i++) {
        if (f.read((char*)(&ch), sizeof(uint8_t))) {
            int mod = i % 4;
            if (mod == 3) {
                print((uint16_t)(i - 3));
                cout << ": ";
                print(curInstr);
                cout << '\n';

                curInstr = 0;
            }
            curInstr += ch << mod * 8;
        } else {
            throw new runtime_error("pizda");
        }
    }
}

int main(int argc, char const* argv[]) {
    ifstream f("resources/out.elf", std::ios::binary);
    int mainIndex = 0x54;
    int factorialIndex = 0x84;

    int ctr = 0;
    uint16_t ch = 0;
    uint32_t curInstr = 0;
    while (f.read((char*)(&ch), sizeof(uint8_t))) {
        if (ctr == mainIndex - 1) {
            printFunc(f, "main", 12);
            ctr += 12 * 4;
        }
        if (ctr == factorialIndex - 1) {
            printFunc(f, "factorial", 12);
            ctr += 12 * 4;
        }
        ++ctr;
    }

    f.close();
    return 0;
}