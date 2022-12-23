#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "ElfParser.h"
#include "InstructionFabric.h"

ElfParser* parseFile(std::ifstream& input) {
    const auto parser = new ElfParser(input);
    try {
        parser->parse();
    } catch (const std::ifstream::failure& e) {
        std::cout << "ifstream::failure: " << e.what() << '\n';
    } catch (std::runtime_error& e) {
        std::cout << "Parse error: " << e.what() << std::endl;
    }
    return parser;
};

int main(const int argc, char const* argv[]) {
    if (argc < 3) {
        std::cout << "2 arguments expected, " + std::to_string(argc - 1) + " found\n";
        return 0;
    }

    std::ifstream input;
    input.open(argv[1], std::ios::binary);

    if (!input.is_open()) {
        std::cout << "Can't open input file";
        return 0;
    }

    ElfParser* parser = parseFile(input);
    input.close();

    FILE* output = fopen(argv[2], "w");
    if (output == nullptr) {
        std::cout << "Can't open output file";
        return 0;
    }

    parser->printDotText(output);
    fprintf(output, "\n");
    parser->printSymtab(output);

    fclose(output);
    delete parser;
    return 0;
}