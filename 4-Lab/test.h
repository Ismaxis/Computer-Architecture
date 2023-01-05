#pragma once
#include <omp.h>
#include "otsuFuncs.h"
#include "PnmImage.h"

inline void threadsTest(const int thresholdsCount, const std::string& inputPath, const std::string& outputPath = "")
{
    std::cout << "ThreadsTEST\n";
#ifdef _OPENMP
    std::cout << "OpenMP is active\n";
#else
    std::cout << "OpenMP is disabled\n";
#endif

    try
    {
        PnmImage image;
        image.loadFromFile(inputPath);

        auto* classes = new uint8_t[INTENSITY_LAYER_COUNT];
        for (int i = 1; i < 13; ++i)
        {
            omp_set_num_threads(i);
            const double tstart = omp_get_wtime();

            std::vector<int> thresholds = otsuThreshold(image, thresholdsCount);

            const double tend = omp_get_wtime();
            printf("%d Threads:\n\tTime (sec): %lf\n", i, tend - tstart);

            if (outputPath != "")
            {
                int cur = 0;
                for (int i = 0; i < INTENSITY_LAYER_COUNT; ++i)
                {
                    if (cur == thresholds.size())
                    {
                        classes[i] = INTENSITY_LAYER_COUNT - 1;
                    }
                    else if (i >= thresholds[cur])
                    {
                        ++cur;
                        --i;
                    }
                    else if (cur == 0)
                    {
                        classes[i] = thresholds[0] / 2;
                    }
                    else
                    {
                        classes[i] = (thresholds[cur] + thresholds[cur - 1]) / 2;
                    }
                }

                PnmImage copyOfImage(image);
                copyOfImage.applyThresholds(classes);

                copyOfImage.saveToFile(outputPath + + "_" + std::to_string(i) + ".pnm");
            }
        }

        delete[] classes;
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
