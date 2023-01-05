#include <omp.h>

#include <iostream>

#include "PnmImage.h"

constexpr int INTENSITY_LAYER_COUNT = 256;

double* calculateProbabilities(const PnmImage& image)
{
    // Count
    auto* probability = new double[INTENSITY_LAYER_COUNT];
    memset(probability, 0.0, INTENSITY_LAYER_COUNT * sizeof(double));
    for (int x = 0; x < image.getXSize(); ++x)
    {
        for (int y = 0; y < image.getYSize(); ++y)
        {
            ++probability[image.getPixel(x, y)];
        }
    }

    // Probability
    const int totalPixelCount = image.getXSize() * image.getYSize();
    for (int i = 0; i < INTENSITY_LAYER_COUNT; ++i)
    {
        probability[i] /= totalPixelCount;
    }

    return probability;
}

double* calculatePrefOmegas(const double* probability)
{
    auto* omega = new double[INTENSITY_LAYER_COUNT];
    omega[0] = probability[0];
    // Pref sum
    for (int i = 1; i < INTENSITY_LAYER_COUNT; ++i)
    {
        omega[i] = omega[i - 1] + probability[i];
    }

    return omega;
}

int calculateMuTotal(const PnmImage& image)
{
    int sum = 0;
    for (int x = 0; x < image.getXSize(); ++x)
    {
        for (int y = 0; y < image.getYSize(); ++y)
        {
            sum += image.getPixel(x, y);
        }
    }

    return sum;
}

double* calculatePrefMus(const double* probabilities)
{
    auto* mu = new double[INTENSITY_LAYER_COUNT];
    mu[0] = 0;
    for (int i = 1; i < INTENSITY_LAYER_COUNT; ++i)
    {
        mu[i] = mu[i - 1] + i * probabilities[i];
    }

    return mu;
}

int resetDigit(std::vector<int>& thresholds, const int i)
{
    if (i == 0)
    {
        if (thresholds[i] >= INTENSITY_LAYER_COUNT - 1)
        {
            return INTENSITY_LAYER_COUNT + 1;
        }

        return ++thresholds[i] + 1;
    }
    if (thresholds[i] >= INTENSITY_LAYER_COUNT - (thresholds.size() - i))
    {
        thresholds[i] = resetDigit(thresholds, i - 1);
    }
    else
    {
        ++thresholds[i];
    }

    return thresholds[i] + 1;
}

bool incThresholds(std::vector<int>& thresholds)
{
    const int last = thresholds.size() - 1;
    if (thresholds[last] >= INTENSITY_LAYER_COUNT - 1)
    {
        if (resetDigit(thresholds, last) >= INTENSITY_LAYER_COUNT + 1)
        {
            return false;
        }
    }
    else
    {
        ++thresholds[last];
    }

    return true;
}

double getOmegaRange(const double* omega, const std::vector<int>& thresholds, const int i)
{
    const double left = i > 0 ? omega[thresholds[i - 1]] : 0.0;
    const double right = i < thresholds.size() ? omega[thresholds[i] - 1] : 1.0;
    return right - left;
}

double getMuRange(const double* mu, const std::vector<int>& thresholds, const int i)
{
    const double left = i > 0 ? mu[thresholds[i - 1]] : 0.0;
    if (i < thresholds.size())
    {
        const double right = mu[thresholds[i] - 1];
        return right - left;
    }
    const double right = mu[INTENSITY_LAYER_COUNT - 1]; 
    return right - left;
}

std::vector<int> otsuThreshold(const PnmImage& image, const int threshCount)
{
    // Probabilities calculations
    const auto* probability = calculateProbabilities(image);

    // Omega's calculations
    const auto* omega = calculatePrefOmegas(probability);

    // Mu's calculations
    const auto* mu = calculatePrefMus(probability);

    // Filling initial thresholds
    std::vector<int> curThresholds(threshCount);
    for (int i = 0; i < threshCount; ++i)
    {
        curThresholds[i] = i;
    }

    // Calculation best thresholds
    std::vector<std::pair<double, std::vector<int>>> results;
    bool running = true;

    omp_set_num_threads(1);
#pragma omp parallel shared(running, curThresholds, results, omega, mu)
    {
        double bestSigma = 0.0;
        std::vector<int> bestThresholds(threshCount);
        std::vector<int> curThresholdsCopy(threshCount);
        while (running)
        {
            double sigma = 0.0;
            curThresholdsCopy = curThresholds;
            for (int i = 0; i <= threshCount; ++i)
            {
                const double omegaRange = getOmegaRange(omega, curThresholdsCopy, i);
                const double muRange = getMuRange(mu, curThresholdsCopy, i) / omegaRange;
                sigma += muRange * muRange * omegaRange;
            }
            if (sigma > bestSigma)
            {
                for (int i = 0; i < threshCount; ++i)
                {
                    bestThresholds[i] = curThresholdsCopy[i];
                }
                bestSigma = sigma;
            }
#pragma omp critical
            {
                if (!incThresholds(curThresholds))
                {
                    running = false;
                }
            }
        }

#pragma omp critical
        {
            results.emplace_back(bestSigma, bestThresholds);
        }
    }

    for (const auto& element : curThresholds)
    {
        std::cout << element << ' ';
    }

    double bestOverallSigma = 0.0;
    int bestOverallIndex = 0;
    for (int i = 0; i < results.size(); ++i)
    {
        if (results[i].first > bestOverallSigma)
        {
            bestOverallSigma = results[i].first;
            bestOverallIndex = i;
        }
    }

    std::cout << "\n" << bestOverallIndex << " is the winner: " << bestOverallSigma << '\n';

    delete[] probability;
    delete[] omega;
    delete[] mu;

    return results[bestOverallIndex].second;
}

int main(const int argc, const char* argv[])
{
    //const std::string fileName = "woman";
    //const std::string path = argc > 2 ? argv[2] : "images/" + fileName + ".pnm";
    const std::string path = argc > 2 ? argv[2] : "itmo-comp-arch22-lab4-Ismaxis\\test_data\\in.pgm";
    //const std::string path = argc > 2 ? argv[2] : "images/image1_mod.pnm";
    try
    {
        PnmImage image;

        image.loadFromFile(path);
        std::vector<int> thresholds;
        for (int i = 0; i < 1; ++i)
        {
            float tstart = omp_get_wtime();

            thresholds = otsuThreshold(image, 3);

            float tend = omp_get_wtime();
            printf("Time (sec): %f\n", tend - tstart);

            for (int i = 0; i < thresholds.size(); ++i)
            {
                std::cout << thresholds[i] << ' ';
            }
            std::cout << "\n\n";
        }
        auto* map = new uint8_t[INTENSITY_LAYER_COUNT];
        int cur = 0;
        for (int i = 0; i < INTENSITY_LAYER_COUNT; ++i)
        {
            if (cur == thresholds.size())
            {
                map[i] = INTENSITY_LAYER_COUNT - 1;
            }
            else if (i >= thresholds[cur])
            {
                ++cur;
                --i;
            }
            else if (cur == 0)
            {
                map[i] = thresholds[0] / 2;
            }
            else
            {
                map[i] = (thresholds[cur] + thresholds[cur - 1]) / 2;
            }
        }

        for (int x = 0; x < image.getXSize(); ++x)
        {
            for (int y = 0; y < image.getYSize(); ++y)
            {
                const unsigned int pixel = image.getPixel(x, y);
                image.setPixel(map[pixel], x, y);
            }
        }

        delete[] map;

        //image.saveToFile("images/" + fileName + "_bin_new.pnm");
        image.saveToFile("itmo-comp-arch22-lab4-Ismaxis\\test_data\\out_new_1.pgm");
        //image.saveToFile("images/image1_bin.pnm");
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

// if (argc < 3) {
//     std::cout << "2 arguments expected, " + std::to_string(argc - 1) + " found\n";
//     return 0;
// }
// try {
//     std::ifstream input;
//     openInFile(input, argv[1]);
//     try {
//         std::ofstream output;
//         openOutFile(output, argv[2]);
//     } catch (const std::ios_base::failure& e) {
//         std::cout << e.what() << std::endl;
//     }
// } catch (std::ios_base::failure& e) {
//     std::cout << e.what() << std::endl;
// } catch (std::runtime_error& e) {
//     std::cout << e.what() << std::endl;
// }
//#pragma omp parallel
//    {
//        printf("Thread %d reporting\n", omp_get_thread_num());
//    }
//    return 0;
