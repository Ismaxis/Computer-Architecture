#include "otsuFuncs.h"

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

double* calculateOmegas(const double* probability)
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

double* calculateMus(const double* probabilities)
{
    auto* mu = new double[INTENSITY_LAYER_COUNT];
    mu[0] = 0;
    for (int i = 1; i < INTENSITY_LAYER_COUNT; ++i)
    {
        mu[i] = i * probabilities[i];
    }

    return mu;
}

double getOmegaRange(const double* probability, const std::vector<int>& thresholds, const int i)
{
    const int left = i > 0 ? thresholds[i - 1] : 0.0;
    const int right = i < thresholds.size() ? thresholds[i] : INTENSITY_LAYER_COUNT - 1;
    double res = 0.0;
    for (int j = left; j < right; ++j)
    {
        res += probability[j];
    }

    return res;
}

double getMuRange(const double* mu, const std::vector<int>& thresholds, const int i)
{
    const int left = i > 0 ? thresholds[i - 1] : 0.0;
    const int right = i < thresholds.size() ? thresholds[i] : INTENSITY_LAYER_COUNT - 1;
    double res = 0.0;
    for (int j = left; j < right; ++j)
    {
        res += mu[j];
    }
    return res;
}

void incDigit(std::vector<int>& thresholds, const int digitIndexToInc)
{
    if (thresholds[digitIndexToInc] >= INTENSITY_LAYER_COUNT - (thresholds.size() - digitIndexToInc))
    {
        incDigit(thresholds, digitIndexToInc - 1);
        thresholds[digitIndexToInc] = thresholds[digitIndexToInc - 1] + 1;
    }
    else
    {
        ++thresholds[digitIndexToInc];
    }
}

void generateFirstThresholdSignature(std::vector<int>& thresholds, int highestDigit)
{
    for (int& threshold : thresholds)
    {
        threshold = highestDigit++;
    }
}

bool isFinished(const std::vector<int>& thresholds)
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

std::vector<int> calculateOtsuThresholds(const PnmImage& image, const int thresholdsCount)
{
    // Probabilities calculations
    const auto* probability = calculateProbabilities(image);

    // Omega's calculations
    //const auto* omega = calculateOmegas(probability);

    // Mu's calculations
    const auto* mu = calculateMus(probability);
    
    // Global highest digit counter
    int curHighestDigit = 0;

    std::vector<std::pair<double, std::vector<int>>> results;
    bool isRunning = true;
#pragma omp parallel 
    {
        double bestSigma = 0.0;
        std::vector<int> localBestThresholds(thresholdsCount);
        std::vector<int> curLocalThresholdSignature(thresholdsCount);
        int curLocalHighestDigit;
        while (isRunning)
        {
#pragma omp critical
            {
                curLocalHighestDigit = curHighestDigit++;
            }

            if (curHighestDigit + thresholdsCount >= INTENSITY_LAYER_COUNT)
            {
                isRunning = false;
            }

            if (!isRunning)
            {
                break;
            }

            generateFirstThresholdSignature(curLocalThresholdSignature, curLocalHighestDigit);
            
            while (curLocalThresholdSignature[0] == curLocalHighestDigit)
            {
                updateToNextThresholdSignature(curLocalThresholdSignature);
                double sigma = 0.0;
                for (int i = 0; i <= thresholdsCount; ++i)
                {
                    const double omegaRange = getOmegaRange(probability, curLocalThresholdSignature, i);
                    const double muRange = getMuRange(mu, curLocalThresholdSignature, i);
                    sigma += muRange * muRange / omegaRange;
                }
                if (sigma > bestSigma)
                {
                    for (int i = 0; i < thresholdsCount; ++i)
                    {
                        localBestThresholds[i] = curLocalThresholdSignature[i];
                    }
                    bestSigma = sigma;
                }
            }
        }

#pragma omp critical
        {
            results.emplace_back(bestSigma, localBestThresholds);
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
    //delete[] omega;
    delete[] mu;

    return results[bestOverallIndex].second;
}
