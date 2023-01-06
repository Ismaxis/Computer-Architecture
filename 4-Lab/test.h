#pragma once
#include <omp.h>
#include "otsuFuncs.h"
#include "PnmImage.h"

inline std::vector<int> testImageThresholds(const int thresholdsCount, const std::string& inputPath,
                                            const std::string& outputPath = "")
{
    try
    {
        PnmImage image;
        image.loadFromFile(inputPath);
        std::vector<int> thresholds = calculateOtsuThresholds(image, thresholdsCount);
        if (!outputPath.empty())
        {
            PnmImage copyOfImage(image);
            copyOfImage.applyThresholds(thresholds);
            copyOfImage.saveToFile(outputPath);
        }

        return thresholds;
    }
    catch (std::ios_base::failure& e)
    {
        std::cout << "IOError in threads test: " << e.what() << std::endl;
    }
    catch (std::runtime_error& e)
    {
        std::cout << "RuntimeError in threads test: " << e.what() << std::endl;
    }
}

inline void threadsTimeTest(const int thresholdsCount, const std::string& inputPath, const std::string& outputPath = "")
{
    std::cout << "ThreadsTEST\n\n";
#ifdef _OPENMP
    std::cout << "OpenMP is active\n";
#else
    std::cout << "OpenMP is disabled\n";
#endif

    try
    {
        PnmImage image;
        image.loadFromFile(inputPath);

        for (int i = 1; i < 13; ++i)
        {
            omp_set_num_threads(i);
            const double start = omp_get_wtime();

            std::vector<int> thresholds = calculateOtsuThresholds(image, thresholdsCount);

            const double end = omp_get_wtime();
            printf("%d Threads:\n\tTime (sec): %lf\n", i, end - start);

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
}
