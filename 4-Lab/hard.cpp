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

void incDigit(std::vector<int>& thresholds, const int digitIndexToInc)
{
    if (thresholds[digitIndexToInc] >= INTENSITY_LAYER_COUNT - (thresholds.size() - digitIndexToInc)) // 1 2 3 4
    {
        incDigit(thresholds, digitIndexToInc - 1);
        thresholds[digitIndexToInc] = thresholds[digitIndexToInc - 1] + 1;
    }
    else
    {
        ++thresholds[digitIndexToInc];
    }
}

bool isFinished(const std::vector<int>& thresholds) // is next step possible
{
    return thresholds[0] + thresholds.size() >= INTENSITY_LAYER_COUNT;
}

void updateToNextThresholdSignature(std::vector<int>& thresholds)
{
    const int lastDigitIndex = thresholds.size() - 1;
    if (thresholds[lastDigitIndex] < INTENSITY_LAYER_COUNT - 1)
    {
        ++thresholds[lastDigitIndex];
    }
    else
    {
        incDigit(thresholds, lastDigitIndex);
    }
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

std::vector<int> otsuThreshold(const PnmImage& image, const int thresholdsCount)
{
    // Probabilities calculations
    const auto* probability = calculateProbabilities(image);

    // Omega's calculations
    const auto* omega = calculatePrefOmegas(probability);

    // Mu's calculations
    const auto* mu = calculatePrefMus(probability);


    // Calculation best thresholds
    std::vector<std::pair<double, std::vector<int>>> results;
    bool isRunning = true;

    std::vector<int> curThresholdSignature(thresholdsCount);
    // Filling initial thresholds
    for (int i = 0; i < thresholdsCount; ++i)
    {
        curThresholdSignature[i] = i;
    }
    curThresholdSignature[curThresholdSignature.size() - 1] -= 1;

    //bool isCurThresholdSignatureInitialized = false;

#pragma omp parallel shared(isRunning, curThresholdSignature, results, omega, mu)
    {
        std::cout << omp_get_num_threads() << '\n';
        double bestSigma = 0.0;
        std::vector<int> bestThresholds(thresholdsCount);
        std::vector<int> curThresholdSignatureCopy(thresholdsCount);
        while (true)
        {
#pragma omp critical
            {
                if (isFinished(curThresholdSignature))
                {
                    isRunning = false;
                }
                else
                {
                    //if (!isCurThresholdSignatureInitialized)
                    //{
                    //    // Filling initial thresholds
                    //    // Its possible to do -1 before cycle
                    //    for (int i = 0; i < thresholdsCount; ++i)
                    //    {
                    //        curThresholdSignature[i] = i;
                    //    }
                    //    isCurThresholdSignatureInitialized = true;
                    //}
                    //else
                    //{
                        updateToNextThresholdSignature(curThresholdSignature);
                    //}

                    curThresholdSignatureCopy = curThresholdSignature;
                }
            }

            if (!isRunning)
            {
                break;
            }

            double sigma = 0.0;
            for (int i = 0; i <= thresholdsCount; ++i)
            {
                const double omegaRange = getOmegaRange(omega, curThresholdSignatureCopy, i);
                const double muRange = getMuRange(mu, curThresholdSignatureCopy, i) / omegaRange;
                sigma += muRange * muRange * omegaRange;
            }
            if (sigma > bestSigma)
            {
                for (int i = 0; i < thresholdsCount; ++i)
                {
                    bestThresholds[i] = curThresholdSignatureCopy[i];
                }
                bestSigma = sigma;
            }
        }

#pragma omp critical
        {
            results.emplace_back(bestSigma, bestThresholds);
        }
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
    try
    {
        PnmImage image;

        image.loadFromFile(path);
        auto* map = new uint8_t[INTENSITY_LAYER_COUNT];
        for (int i = 1; i < 14; ++i)
        {
            omp_set_num_threads(i);
            const float tstart = omp_get_wtime();

            std::vector<int> thresholds;
            thresholds = otsuThreshold(image, 3);

            const float tend = omp_get_wtime();
            printf("%d Threads:\n\tTime (sec): %f\n", i, tend - tstart);

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
            PnmImage copyOfImage(image);
            for (int x = 0; x < copyOfImage.getXSize(); ++x)
            {
                for (int y = 0; y < copyOfImage.getYSize(); ++y)
                {
                    const unsigned int pixel = copyOfImage.getPixel(x, y);
                    copyOfImage.setPixel(map[pixel], x, y);
                }
            }


            //image.saveToFile("images/" + fileName + "_bin_new.pnm");
            copyOfImage.saveToFile(
                "itmo-comp-arch22-lab4-Ismaxis\\test_data\\out_parallel_" + std::to_string(i) + ".pgm");
        }
        delete[] map;
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
