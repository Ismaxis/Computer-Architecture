#include <omp.h>

#include <iostream>

#include "PnmImage.h"

constexpr int INTENSITY_LAYER_NUMBER = 256;

void calculateOmegas(const PnmImage& image, double *omega)
{
    for (int x = 0; x < image.getXSize(); ++x)
    {
        for (int y = 0; y < image.getYSize(); ++y)
        {
            ++omega[image.getPixel(x,y)];
        }
    }
}

void calculatePrefOmegas(const PnmImage& image, double *omega)
{
    calculateOmegas(image, omega);

    for (int i = 1; i < INTENSITY_LAYER_NUMBER; ++i)
    {
        omega[i] += omega[i-1];
    }

    const int totalPixelCount = image.getXSize() * image.getYSize();
    for (int i = 0; i < INTENSITY_LAYER_NUMBER; ++i)
    {
        omega[i] /= totalPixelCount;
    }
}

int calculateIntensitySum(const PnmImage& image)
{
    int sum = 0;
    for (int x = 0; x < image.getXSize(); ++x)
    {
        for (int y = 0; y < image.getYSize(); ++y)
        {
            sum += image.getPixel(x,y);
        }
    }

    return sum;
}

std::vector<int> otsuThreshold1(const PnmImage& image)
{
    double* hist = new double[INTENSITY_LAYER_NUMBER];
    for (int i = 0; i < INTENSITY_LAYER_NUMBER; ++i)
    {
        hist[i] = 0;
    }
    calculateOmegas(image, hist);

    const int allPixelCount = image.getXSize() * image.getYSize();
    const int allIntensitySum = calculateIntensitySum(image);

    std::vector<int> res(2);

    double bestSigma = 0.0;

    int firstClassPixelCount = 0;
    int firstClassIntensitySum = 0;
    for (int thresh1 = 0; thresh1 < INTENSITY_LAYER_NUMBER - 2; ++thresh1) 
    {
        firstClassPixelCount += hist[thresh1];
        firstClassIntensitySum += thresh1 * hist[thresh1];
        const double firstClassProb = firstClassPixelCount / (double)allPixelCount;
    
        int secondClassPixelCount = 0;
        int secondClassIntensitySum = 0;
        int bestThresh2 = 0;
        double bestSigma2 = 0.0;
        for (int thresh2 = thresh1; thresh2 < INTENSITY_LAYER_NUMBER - 1; ++thresh2)
        {
            secondClassPixelCount += hist[thresh1];
            secondClassIntensitySum += thresh1 * hist[thresh1];
            const double secondClassProb = firstClassPixelCount / (double)allPixelCount;
            const double thirdClassProb = 1.0 - secondClassProb - firstClassProb;

            const double secondClassMean = secondClassIntensitySum / (double)secondClassPixelCount;
            const double thirdClassMean = (allIntensitySum - secondClassIntensitySum - firstClassIntensitySum) / (double)(allPixelCount - secondClassPixelCount - firstClassPixelCount);

            const double meanDelta2 = secondClassMean - thirdClassMean;

            const double sigma2 = secondClassProb * thirdClassProb * meanDelta2 * meanDelta2;

            if (sigma2 > bestSigma2) 
            {
                bestSigma2 = sigma2;
                bestThresh2 = thresh2;
            }
        }
        
        const double firstClassMean = firstClassIntensitySum / (double)firstClassPixelCount;
        const double secondClassMean = (allIntensitySum - firstClassIntensitySum) / (double)(allPixelCount - firstClassPixelCount);

        const double meanDelta1 = firstClassMean - secondClassMean;

        const double sigma1 = firstClassProb * (1.0 - firstClassProb) * meanDelta1 * meanDelta1;

        if (sigma1 + bestSigma2 > bestSigma) 
        {
            bestSigma = sigma1 + bestSigma2;
            res[0] = thresh1;
            res[1] = bestThresh2;
        }
    }
    //delete[] hist;
    return res;
}

int resetDigit(std::vector<int>& thresholds, const int i) {
    if (i == 0)
    {
        if (thresholds[i] == INTENSITY_LAYER_NUMBER - 1)
        {
            return INTENSITY_LAYER_NUMBER + 1;
        }

        return ++thresholds[i] + 1;
    }
    if (thresholds[i] >= INTENSITY_LAYER_NUMBER - (thresholds.size() - i))
    {
        thresholds[i] = resetDigit(thresholds, i - 1);
    }
    else
    {
        ++thresholds[i];
    }
    return thresholds[i] + 1;
}

bool incThresholds(std::vector<int>& thresholds) {
    //if (thresholds[2] == 254)
    //{
    //    std::cout << thresholds[0] << ' ' << thresholds[1] << ' ' << thresholds[2] << '\n';
    //}
    const int last = thresholds.size() - 1;
    if (thresholds[last] == INTENSITY_LAYER_NUMBER - 1) 
    {
        if (resetDigit(thresholds, last) == INTENSITY_LAYER_NUMBER + 1)
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

double getOmegaRange(const double* omega, const std::vector<int>& thresholds, const int i) {
    const double left = i > 0 ? omega[thresholds[i-1]] : 0.0;
    const double right = omega[thresholds[i]];
    return right - left;
}
double getMuRange(const double* mu, const std::vector<int>& thresholds, const int i) {
    const double left = i > 0 ? mu[thresholds[i-1]] : 0.0;
    const double right = mu[thresholds[i]];
    return right - left;
}

std::vector<int> otsuThreshold(const PnmImage& image, const int threshCount)
{
    // Omega calculations
    auto* omega = new double[INTENSITY_LAYER_NUMBER];
    memset(omega, 0.0,INTENSITY_LAYER_NUMBER*sizeof(double));
    calculatePrefOmegas(image, omega);

    // Mu calculations
    const double muT = calculateIntensitySum(image);
    auto* mu = new double[INTENSITY_LAYER_NUMBER];
    mu[0] = 0; 
    for (int i = 1; i < INTENSITY_LAYER_NUMBER; ++i)
    {
        mu[i] = mu[i-1] + i*omega[i];
    }

    // Filling initial thresholds
    std::vector<int> curThresholds(threshCount);
    for (int i = 0; i < threshCount; ++i)
    {
        curThresholds[i] = i;
    }

    // Calculation best thresholds
    double bestSigma = 0.0;
    std::vector<int> bestThresholds(threshCount);
    do
    {
        double sigma = 0.0;
        for (int i = 0; i < threshCount; ++i)
        {
            const double muRange = getMuRange(mu, curThresholds, i) / omega[curThresholds[i]];
            const double omegaRange = getOmegaRange(omega, curThresholds, i);
            sigma += (muT - muRange)*(muT - muRange)*omegaRange;
        }

        const double muRange = mu[INTENSITY_LAYER_NUMBER - 1] - mu[curThresholds[threshCount - 1]];
        sigma += (muT - muRange)*(muT - muRange)*(1.0 - omega[curThresholds[threshCount - 1]]);

        if (sigma > bestSigma)
        {
            for (int i = 0; i < threshCount; ++i)
            {
                bestThresholds[i] = curThresholds[i];
            }
            bestSigma = sigma;
        }
    } while (incThresholds(curThresholds));


    delete[] omega;
    delete[] mu;

    return bestThresholds;
}

int main(const int argc, char const* argv[])
{
    const std::string fileName = "woman";
    const std::string path = argc > 2 ? argv[2] : "images/" + fileName + ".pnm";
    try 
    {
        PnmImage image;

        image.loadFromFile(path);

        const std::vector<int> thresholds = otsuThreshold(image, 3);
        uint8_t* map = new uint8_t[INTENSITY_LAYER_NUMBER];
        int cur = 0;
        for (int i = 0; i < INTENSITY_LAYER_NUMBER; ++i)
        {
            if (cur == thresholds.size())
            {
                map[i] = INTENSITY_LAYER_NUMBER - 1;
            }
            else if (i >= thresholds[cur])
            {
                ++cur;
                --i;
            }
            else
            {
                map[i] = thresholds[cur];
            }
        }

        for (int x = 0; x < image.getXSize(); ++x)
        {
            for (int y = 0; y < image.getYSize(); ++y)
            {
                const unsigned int pixel = image.getPixel(x,y);
                image.setPixel(map[pixel], x, y);
            }
        }

        delete[] map;

        image.saveToFile("images/" + fileName + "_bin.pnm");
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