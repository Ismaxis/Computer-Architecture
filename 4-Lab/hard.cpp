#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>

#include "otsuFuncs.h"

int main(const int argc, const char* argv[])
{
    if (argc < 4)
    {
        std::cout << "3 arguments expected, " << std::to_string(argc - 1) << " found" << std::endl;
        return -1;
    }

    std::string threadsCountStr = argv[1];
    std::string in = argv[2];
    std::string out = argv[3];

    int threadsCount;
    try
    {
        threadsCount = std::stoi(threadsCountStr);
        if (threadsCount < -1)
        {
            std::cout << "Threads count " << threadsCountStr <<
                " is invalid.\nValid values:\n\t0: Default number of threads\n      >0: Given number of threads\n       -1: Disable OpenMP (SingleThread)" << std::endl;
            return -1;
        }
    }
    catch (std::invalid_argument& e)
    {
        std::cout << "Threads count should be a number: " << threadsCountStr << std::endl;
        return -1;
    }

    try
    {
        PnmImage image;
        image.loadFromFile(in);

        if (threadsCount > 0)
        {
            omp_set_num_threads(threadsCount);
        }
        bool isOmpEnabled = threadsCount != -1;

        double start = omp_get_wtime();
        std::vector<int> thresholds = calculateOtsuThresholds(image, isOmpEnabled);
        image.applyThresholds(thresholds, isOmpEnabled);
        double end = omp_get_wtime();

        printf("Time (%i thread(s)): %g ms\n", threadsCount == 0 ? omp_get_max_threads() : threadsCount, (end - start) * 1000);
        printf("%u %u %u\n", thresholds[0], thresholds[1], thresholds[2]);

        image.saveToFile(out);
    }
    catch (const std::ios_base::failure& e)
    {
        std::cout << e.what() << std::endl;
    }
    catch (const std::runtime_error& e)
    {
        std::cout << e.what() << std::endl;
    }

    return 0;
}
