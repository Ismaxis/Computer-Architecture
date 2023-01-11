#include <iostream>

#include "otsuFuncs.h"

int main(const int argc, const char* argv[])
{
    if (argc < 4)
    {
        std::cout << "3 arguments expected, " + std::to_string(argc - 1) + " found\n";
        return 0;
    }

    const std::string threadsCountStr = argv[1];
    const std::string in = argv[2];
    const std::string out = argv[3];

    int threadsCount;
    try
    {
        threadsCount = std::stoi(threadsCountStr);
        if (threadsCount < -1)
        {
            std::cout << "Threads count " + std::string(threadsCountStr) +
                " is invalid.\nValid values:\n\t0: Default number of threads\n      >0: Given number of threads\n       -1: Disable OpenMP (SingleThread)\n";
            return 0;
        }
    }
    catch (std::invalid_argument& e)
    {
        std::cout << "Threads count should be a number: " << std::string(threadsCountStr) << std::endl;
        return 0;
    }

    try
    {
        PnmImage image;
        image.loadFromFile(in);

        if (threadsCount > 0)
        {
            omp_set_num_threads(threadsCount);
        }

        const bool isOmpEnabled = threadsCount != -1;
        const double start = omp_get_wtime();
        const std::vector<int> thresholds = calculateOtsuThresholds(image, isOmpEnabled);
        image.applyThresholds(thresholds, isOmpEnabled);
        const double end = omp_get_wtime();

        printf("Time (%i thread(s)): %g ms\n", threadsCount == 0 ? omp_get_max_threads() : threadsCount,
               (end - start) * 1000);
        printf("%u %u %u\n", thresholds[0], thresholds[1], thresholds[2]);

        image.saveToFile(out);
    }
    catch (std::ios_base::failure& e)
    {
        std::cout << e.what() << std::endl;
    }
    catch (std::runtime_error& e)
    {
        std::cout << e.what() << std::endl;
    }

    return 0;
}
