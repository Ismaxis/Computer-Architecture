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

double getMuRange(const double* mu, const int left, const int right)
{
    double res = 0.0;
    for (int j = left + 1; j <= right; ++j)
    {
        res += mu[j];
    }
    return res;
}

double getPrefMuRange(const double* mu, const int left, const int right)
{
    return mu[right] - (left >= 0 ? mu[left] : 0.0);
}

std::vector<int> calculateOtsuThresholds(const PnmImage& image)
{
    // Probabilities calculations
    const auto* probability = calculateProbabilities(image);

    // Omega's calculations
    const auto* prefOmega = calculateOmegas(probability);

    // Mu's calculations
    const auto* prefMu = calculatePrefMus(probability);

    std::vector<std::pair<double, std::vector<int>>> results;

#pragma omp parallel
    {
        double bestSigma = 0.0;
        std::vector<int> localBestThresholds(3);
#pragma omp for
        for (int i = 1; i < INTENSITY_LAYER_COUNT - 3; ++i)
        {
            for (int j = i + 1; j < INTENSITY_LAYER_COUNT - 2; ++j)
            {
                for (int k = j + 1; k < INTENSITY_LAYER_COUNT - 1; ++k)
                {
                    double sigma = 0.0;

                    double omegaRange = getPrefOmegaRange(prefOmega, -1, i);
                    double muRange = getPrefMuRange(prefMu, -1, i);
                    sigma += muRange * muRange / omegaRange;

                    omegaRange = getPrefOmegaRange(prefOmega, i, j);
                    muRange = getPrefMuRange(prefMu, i, j);
                    sigma += muRange * muRange / omegaRange;

                    omegaRange = getPrefOmegaRange(prefOmega, j, k);
                    muRange = getPrefMuRange(prefMu, j, k);
                    sigma += muRange * muRange / omegaRange;

                    omegaRange = getPrefOmegaRange(prefOmega, k, INTENSITY_LAYER_COUNT - 1);
                    muRange = getPrefMuRange(prefMu, k, INTENSITY_LAYER_COUNT - 1);
                    sigma += muRange * muRange / omegaRange;
                    
                    if (sigma > bestSigma)
                    {
                        localBestThresholds[0] = i;
                        localBestThresholds[1] = j;
                        localBestThresholds[2] = k;
                        
                        bestSigma = sigma;
                    }
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
    delete[] prefOmega;
    delete[] prefMu;

    return results[bestOverallIndex].second;
}
