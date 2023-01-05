#include <iostream>

#include "otsuFuncs.h"
#include "test.h"

int main(const int argc, const char* argv[])
{
    //const std::string fileName = "woman";
    //const std::string path = argc > 2 ? argv[2] : "images/" + fileName + ".pnm";
    const std::string inputPath = argc > 2 ? argv[2] : "itmo-comp-arch22-lab4-Ismaxis\\test_data\\in.pgm";
    threadsTest(3, inputPath);

    return 0;
}
