#include <omp.h>

#include <fstream>
#include <iostream>

#include "PnmImage.h"

void openInFile(std::ifstream& input, const char* path)
{
    input.open(path, std::ios_base::binary);
    if (!input.is_open()) 
    {
        throw std::ios_base::failure("Can`t open input file");
    }
};

void openOutFile(std::ofstream& output, const char* path)
{
    output.open(path, std::ios_base::binary);
    if (!output.is_open()) 
    {
        throw std::ios_base::failure("Can`t open output file");
    }
}



int main(const int argc, char const* argv[])
{
    const std::string path = argc > 2 ? argv[2] : "images/image1.pnm";
    const int threshold = 1;
    try 
    {
        PnmImage image;

        image.loadFromFile(path);

        for (int x = 0; x < image.getX(); ++x)
        {
            for (int y = 0; y < image.getY(); ++y)
            {
                image.setPixel(image.getPixel(x,y) > threshold ? 255 : 0, x, y);
            }
        }

        image.saveToFile("images/image1_bin.pnm");
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