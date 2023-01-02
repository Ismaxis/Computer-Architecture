#include <omp.h>

#include <iostream>

#include "PnmImage.h"

constexpr int INTENSITY_LAYER_NUMBER = 256;

void calculateHist(const PnmImage& image, int *hist) {
    for (int x = 0; x < image.getXSize(); ++x)
    {
        for (int y = 0; y < image.getYSize(); ++y)
        {
            ++hist[image.getPixel(x,y)];
        }
    }
}

int calculateIntensitySum(const PnmImage& image) {
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

int otsuThreshold(const PnmImage& image) {
    int* hist = new int[INTENSITY_LAYER_NUMBER];
    calculateHist(image, hist);

    int all_pixel_count = image.getXSize() * image.getYSize();
    int all_intensity_sum = calculateIntensitySum(image);

    int best_thresh = 0;
    double best_sigma = 0.0;

    int first_class_pixel_count = 0;
    int first_class_intensity_sum = 0;

    // Перебираем границу между классами
    // thresh < INTENSITY_LAYER_NUMBER - 1, т.к. при 255 в ноль уходит знаменатель внутри for
    for (int thresh = 0; thresh < INTENSITY_LAYER_NUMBER - 1; ++thresh) {
        first_class_pixel_count += hist[thresh];
        first_class_intensity_sum += thresh * hist[thresh];

        double first_class_prob = first_class_pixel_count / (double)all_pixel_count;
        double second_class_prob = 1.0 - first_class_prob;

        double first_class_mean = first_class_intensity_sum / (double)first_class_pixel_count;
        double second_class_mean = (all_intensity_sum - first_class_intensity_sum) / (double)(all_pixel_count - first_class_pixel_count);

        double mean_delta = first_class_mean - second_class_mean;

        double sigma = first_class_prob * second_class_prob * mean_delta * mean_delta;

        if (sigma > best_sigma) {
            best_sigma = sigma;
            best_thresh = thresh;
        }
    }

    return best_thresh;
}

int main(const int argc, char const* argv[])
{
    const std::string path = argc > 2 ? argv[2] : "images/drop.pnm";
    try 
    {
        PnmImage image;

        image.loadFromFile(path);

        const int threshold = otsuThreshold(image);
        //const int threshold = 100;

        for (int x = 0; x < image.getXSize(); ++x)
        {
            for (int y = 0; y < image.getYSize(); ++y)
            {
                unsigned int pixel = image.getPixel(x,y);
                image.setPixel(pixel > threshold ? (char)255 : 0, x, y);
            }
        }

        image.saveToFile("images/drop_bin.pnm");
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