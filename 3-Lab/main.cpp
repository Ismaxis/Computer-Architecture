#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "ElfParser.h"
#include "InstructionFabric.h"

ElfParser* parseFile(const char* path) {
    std::ifstream input;
    input.open(path, std::ios_base::binary);
    if (!input.is_open()) {
        throw std::ios_base::failure("Can`t open input file");
    }

    const auto parser = new ElfParser(input);
    parser->parse();
    return parser;
};

FILE* openOutFile(const char* path) {
    FILE* output;
    if (fopen_s(&output, path, "w")) {
        throw std::ios_base::failure("Can`t open output file");
    }
    return output;
}

int main(const int argc, char const* argv[]) {
    if (argc < 3) {
        std::cout << "2 arguments expected, " + std::to_string(argc - 1) + " found\n";
        return 0;
    }

    ElfParser* parser = nullptr;
    try {
        parser = parseFile(argv[1]);
        FILE* output = nullptr;
        try {
            output = openOutFile(argv[2]);
            parser->printDotText(output);
            fprintf(output, "\n");
            parser->printSymtab(output);
        } catch (const std::ios_base::failure& e) {
            std::cout << e.what() << std::endl;
        }
        if (output != nullptr) {
            fclose(output);
        }
    } catch (std::ios_base::failure& e) {
        std::cout << e.what() << std::endl;
    } catch (std::runtime_error& e) {
        std::cout << e.what() << std::endl;
    }

    delete parser;
    return 0;
}