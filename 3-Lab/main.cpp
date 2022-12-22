#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "ElfParser.h"
#include "InstructionFabric.h"

int main(int argc, char const* argv[]) {
    std::ifstream input;
    FILE* output;
    output = fopen("resources/myOut.s", "w");
    try {
        input.open("resources/out.elf", std::ios::binary);
        ElfParser parser = ElfParser(input);
        parser.parse();
        parser.printDotText(output);
        fprintf(output, "\n");
        std::cout << std::endl;
        parser.printSymtab(output);
    } catch (const std::ifstream::failure& e) {
        std::cout << "ifstream::failure in main: " << e.what() << '\n';
    } catch (std::runtime_error* e) {
        std::cout << "Error in main: " << e->what() << std::endl;
    }

    fclose(output);
    input.close();
    return 0;
}