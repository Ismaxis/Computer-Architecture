#pragma once
#include <fstream>
#include <string>
#include <vector>

class PnmImage
{
private:
    int sizeX = 0;
    int sizeY = 0;
    std::vector<std::vector<int>> storage;
public:

    int getXSize() const {
        return sizeX;
    }
    int getYSize() const {
        return sizeY;
    }

    int getPixel(const int x, const int y) const
    {
        return storage.at(y).at(x);    
    }
    void setPixel(const int value, const int x, const int y)
    {
        storage.at(y).at(x) = value;    
    }

    void loadFromFile(const std::string& path)
    {
        std::ifstream input;
        input.open(path);
        if (!input.is_open()) 
        {
            throw std::ios_base::failure("Can`t open input file");
        } 
        std::string buff;
        input >> buff;
        if (buff != "P5")
        {
            throw std::runtime_error("Unknown File type: '" + buff + "'");
        }
        input >> sizeX;
        input >> sizeY;
        
        int huesCount;
        input >> huesCount;
        if (huesCount != 255)
        {
            throw std::runtime_error("Invalid count of hues'" + buff + "'");
        }

        storage.clear();
        storage.resize(sizeY);
        for (int y = 0; y < sizeY; ++y)
        {
            for (int x = 0; x < sizeX; ++x)
            {
                int pixel;
                input >> pixel;
                storage.at(y).push_back(pixel);
            }
        }
    }

    void saveToFile(const std::string& path) const
    {
        std::ofstream output;
        output.open(path);
        if (!output.is_open()) 
        {
            throw std::ios_base::failure("Can`t open output file");
        }
        output << "P5\n";
        
        output << sizeX << ' ';
        output << sizeY << '\n';

        output << 255 << '\n';


        for (int y = 0; y < sizeY; ++y)
        {
            for (int x = 0; x < sizeX; ++x)
            {
                output << storage.at(y).at(x) << ' ';
            }
            output << '\n';
        }
    }
};

