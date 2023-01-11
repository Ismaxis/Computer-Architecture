#include "otsuFuncs.h"

double* calculateProbabilities(const PnmImage& image, const bool ompEnabled)
{
    auto* probability = new double[INTENSITY_LAYER_COUNT];
    memset(probability, 0.0, INTENSITY_LAYER_COUNT * sizeof(double));
#pragma omp parallel if (ompEnabled)
    {
#pragma omp for
        for (int x = 0; x < image.getXSize(); ++x)
        {
            for (int y = 0; y < image.getYSize(); ++y)
            {
                ++probability[image.getPixel(x, y)];
            }
        }
    }

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
    for (int i = 1; i < INTENSITY_LAYER_COUNT; ++i)
    {
        omega[i] = omega[i - 1] + probability[i];
    }

    return omega;
}

double* calculatePrefMus(const double* probability)
{
    auto* mu = new double[INTENSITY_LAYER_COUNT];
    mu[0] = 0.0;
    for (int i = 1; i < INTENSITY_LAYER_COUNT; ++i)
    {
        mu[i] = mu[i - 1] + i * probability[i];
    }

    return mu;
}

double getPrefOmegaRange(const double* omega, const int left, const int right)
{
    return omega[right] - (left >= 0 ? omega[left] : 0.0);
}

double getPrefMuRange(const double* mu, const int left, const int right)
{
    return mu[right] - (left >= 0 ? mu[left] : 0.0);
}

double calculateSigmaForClass(const double* prefOmega, const double* prefMu, const int left, const int right)
{
    const double omegaRange = getPrefOmegaRange(prefOmega, left, right);
    const double muRange = getPrefMuRange(prefMu, left, right);
    return muRange * muRange / omegaRange;
}

std::vector<int> calculateOtsuThresholds(const PnmImage& image, const bool ompEnabled)
{
    const auto* probability = calculateProbabilities(image, ompEnabled);
    const auto* prefOmega = calculatePrefOmegas(probability);
    const auto* prefMu = calculatePrefMus(probability);

    std::vector<std::pair<double, std::vector<int>>> results;
#pragma omp parallel if (ompEnabled) default(none) shared(prefOmega, prefMu, results)
    {
        double localBestSigma = 0.0;
        std::vector<int> localBestThresholds(3);
#pragma omp for
        for (int i = 1; i < INTENSITY_LAYER_COUNT - 3; ++i)
        {
            const double firstClassSigma = calculateSigmaForClass(prefOmega, prefMu, -1, i);
            for (int j = i + 1; j < INTENSITY_LAYER_COUNT - 2; ++j)
            {
                const double secondClassSigma = calculateSigmaForClass(prefOmega, prefMu, i, j);
                for (int k = j + 1; k < INTENSITY_LAYER_COUNT - 1; ++k)
                {
                    const double thirdClassSigma = calculateSigmaForClass(prefOmega, prefMu, j, k);
                    const double fourthClassSigma = calculateSigmaForClass(
                        prefOmega, prefMu, k, INTENSITY_LAYER_COUNT - 1);

                    const double curSigma = firstClassSigma + secondClassSigma + thirdClassSigma + fourthClassSigma;
                    if (curSigma > localBestSigma)
                    {
                        localBestSigma = curSigma;

                        localBestThresholds[0] = i;
                        localBestThresholds[1] = j;
                        localBestThresholds[2] = k;
                    }
                }
            }
        }

#pragma omp critical
        {
            results.emplace_back(localBestSigma, localBestThresholds);
        }
    }

    double overallBestSigma = 0.0;
    int overallBestIndex = 0;
    for (int i = 0; i < results.size(); ++i)
    {
        if (results[i].first > overallBestSigma)
        {
            overallBestSigma = results[i].first;
            overallBestIndex = i;
        }
    }

    delete[] probability;
    delete[] prefOmega;
    delete[] prefMu;

    return results[overallBestIndex].second;
}
