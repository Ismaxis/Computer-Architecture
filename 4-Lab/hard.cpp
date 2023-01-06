#include <iostream>

#include "otsuFuncs.h"
#include "test.h"

int main(const int argc, const char* argv[])
{
    const std::string fileName = "woman";
    const std::string inputPath = argc > 2 ? argv[2] : "images/" + fileName + ".pnm";
    threadsTimeTest(4, inputPath);

    return 0;
}
