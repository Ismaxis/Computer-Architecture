#pragma once
#include <omp.h>
#include "otsuFuncs.h"
#include "PnmImage.h"

inline std::vector<int> testImageThresholds(const int thresholdsCount, const std::string& inputPath,
                                            const std::string& outputPath = "")
{
    std::vector<int> thresholds;
    try
    {
        PnmImage image;
        image.loadFromFile(inputPath);
        thresholds = calculateOtsuThresholds(image);
        if (!outputPath.empty())
        {
            PnmImage copyOfImage(image);
            copyOfImage.applyThresholds(thresholds);
            copyOfImage.saveToFile(outputPath);
        }
    }
    catch (std::ios_base::failure& e)
    {
        std::cout << "IOError in threads test: " << e.what() << std::endl;
    }
    catch (std::runtime_error& e)
    {
        std::cout << "RuntimeError in threads test: " << e.what() << std::endl;
    }

    return thresholds;
}

inline double threadsTimeTest(const int thresholdsCount, const int threadsCount, const std::string& inputPath,
                              const std::string& outputPath = "")
{
    double sum = 0.0;
    constexpr int iterations = 5;
    try
    {
        PnmImage image;
        image.loadFromFile(inputPath);

        omp_set_num_threads(threadsCount);
        calculateOtsuThresholds(image);
        for (int i = 0; i < iterations; ++i)
        {
            const double start = omp_get_wtime();

            std::vector<int> thresholds = calculateOtsuThresholds(image);

            const double end = omp_get_wtime();

            sum += end - start;

            if (outputPath != "")
            {
                PnmImage copyOfImage(image);
                copyOfImage.applyThresholds(thresholds);
                copyOfImage.saveToFile(outputPath + "_" + std::to_string(i) + ".pnm");
            }
        }
    }
    catch (std::ios_base::failure& e)
    {
        std::cout << "IOError in threads test: " << e.what() << std::endl;
    }
    catch (std::runtime_error& e)
    {
        std::cout << "RuntimeError in threads test: " << e.what() << std::endl;
    }

    return sum / iterations;
}
