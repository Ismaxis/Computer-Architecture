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
    const auto* omega = calculatePrefOmegas(probability);

    // Mu's calculations
    const auto* mu = calculatePrefMus(probability);

    // Calculation best thresholds

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
                if (curHighestDigit + thresholdsCount >= INTENSITY_LAYER_COUNT)
                {
                    isRunning = false;
                }
                else
                {
                    curLocalHighestDigit = curHighestDigit++;
                }
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
                    const double omegaRange = getOmegaRange(omega, curLocalThresholdSignature, i);
                    const double muRange = getMuRange(mu, curLocalThresholdSignature, i) / omegaRange;
                    sigma += muRange * muRange * omegaRange;
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
    delete[] omega;
    delete[] mu;

    return results[bestOverallIndex].second;
}
