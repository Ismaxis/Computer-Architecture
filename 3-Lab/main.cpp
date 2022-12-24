#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "ElfParser.h"
#include "InstructionFabric.h"

ElfParser* parseFile(std::ifstream& input, const char* path) {
    input.open(path, std::ios_base::binary);
    if (!input.is_open()) {
        throw std::ios_base::failure("Can`t open input file");
    }

    const auto parser = new ElfParser(input);
    parser->parse();
    return parser;
};

void openOutFile(std::ofstream& output, const char* path) {
    output.open(path, std::ios_base::binary);
    if (!output.is_open()) {
        throw std::ios_base::failure("Can`t open output file");
    }
}

int main(const int argc, char const* argv[]) {
    if (argc < 3) {
        std::cout << "2 arguments expected, " + std::to_string(argc - 1) + " found\n";
        return 0;
    }

    try {
        std::ifstream input;
        ElfParser* parser = parseFile(input, argv[1]);
        try {
            std::ofstream output;
            openOutFile(output, argv[2]);
            parser->printDotText(output);
            output << "\n";
            parser->printSymtab(output);
        } catch (const std::ios_base::failure& e) {
            std::cout << e.what() << std::endl;
        }
        delete parser;
    } catch (std::ios_base::failure& e) {
        std::cout << e.what() << std::endl;
    } catch (std::runtime_error& e) {
        std::cout << e.what() << std::endl;
    }

    return 0;
}