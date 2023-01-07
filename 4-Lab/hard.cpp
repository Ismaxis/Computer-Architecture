#include <iostream>

#include "otsuFuncs.h"
#include "test.h"

constexpr int THRESHOLDS_COUNT = 5;

int main(const int argc, const char* argv[])
{
    if (argc < 2)
    {
        std::cout << "2 arguments expected, " + std::to_string(argc - 1) + " found\n";
        return 0;
    }

    int threadsCount;
    try
    {
        threadsCount = std::stoi(argv[1]);
        if (threadsCount < 1)
        {
            std::cout << "Thread count " + std::string(argv[1]) + " is invalid.\nIt should be greater then zero.\n";
            return 0;
        }
    }
    catch (std::invalid_argument& e)
    {
        std::cout << e.what() << ": " << std::string(argv[1]) << std::endl;
        return 0;
    }

    try
    {
        PnmImage image;
        image.loadFromFile(argv[2]);

        const std::vector<int> thresholds = calculateOtsuThresholds(image, THRESHOLDS_COUNT);

        omp_set_num_threads(threadsCount);
        image.applyThresholds(thresholds);

        image.saveToFile(argv[3]);
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
