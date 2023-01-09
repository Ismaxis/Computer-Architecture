#include <iostream>

#include "otsuFuncs.h"
#include "test.h"

int THRESHOLDS_COUNT = 3;

int main(const int argc, const char* argv[])
{
    if (argc < 4)
    {
        std::cout << "3 arguments expected, " + std::to_string(argc - 1) + " found\n";
    }

    const std::string threadsCountStr = argv[1];
    const std::string in = argv[2];
    const std::string out = argv[3];

    int threadsCount;
    try
    {
        threadsCount = std::stoi(threadsCountStr);
        if (threadsCount < 1)
        {
            std::cout << "Thread count " + std::string(threadsCountStr) +
                " is invalid.\nIt should be greater then zero.\n";
            return 0;
        }
    }
    catch (std::invalid_argument& e)
    {
        std::cout << e.what() << ": " << std::string(threadsCountStr) << std::endl;
        return 0;
    }

    try
    {
        PnmImage image;
        image.loadFromFile(in);

        omp_set_num_threads(threadsCount);
        const std::vector<int> thresholds = calculateOtsuThresholds(image, THRESHOLDS_COUNT);
        printf("%u %u %u\n", thresholds[0], thresholds[1], thresholds[2]);
        image.applyThresholds(thresholds);

        image.saveToFile(out);
    }
    catch (std::ios_base::failure& e)
    {
        std::cout << "IOError: " << e.what() << std::endl;
    }
    catch (std::runtime_error& e)
    {
        std::cout << "RuntimeError: " << e.what() << std::endl;
    }

    return 0;
}
