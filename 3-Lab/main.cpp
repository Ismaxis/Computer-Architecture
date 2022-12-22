#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "ElfParser.h"
#include "InstructionFabric.h"

int main(int argc, char const* argv[]) {
    std::ifstream f;
    try {
        f.open("resources/out.elf", std::ios::binary);
        ElfParser parser = ElfParser(f);
        parser.parse();
        parser.printDotText();
        std::cout << std::endl;
        parser.printSymtab();
    } catch (const std::ifstream::failure& e) {
        std::cout << "ifstream::failure in main: " << e.what() << '\n';
    } catch (std::runtime_error* e) {
        std::cout << "Error in main: " << e->what() << std::endl;
    }

    f.close();
    return 0;
}