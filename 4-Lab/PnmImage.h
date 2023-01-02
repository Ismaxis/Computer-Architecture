#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

class PnmImage
{
private:
    int sizeX = 0;
    int sizeY = 0;
    std::vector<std::vector<char>> storage;
public:

    int getXSize() const {
        return sizeX;
    }
    int getYSize() const {
        return sizeY;
    }

    char getPixel(const int x, const int y) const
    {
        return storage.at(y).at(x);    
    }
    void setPixel(const char value, const int x, const int y)
    {
        storage.at(y).at(x) = value;    
    }

    void loadFromFile(const std::string& path)
    {
        std::ifstream input;
        input.open(path, std::ios::binary);
        if (!input.is_open()) 
        {
            throw std::ios_base::failure("Can`t open input file");
        } 
        std::string type;
        input >> type;
        if (type != "P5")
        {
            throw std::runtime_error("Unknown File type: '" + type + "'");
        }
        char buff;
        input.read(&buff, 1);
        input.read(&buff, 1);
        while(buff != ' ')
        {
            sizeX *= 10;
            sizeX += buff - '0';
            input.read(&buff, 1);
        }
        input.read(&buff, 1);
        while(buff != '\n')
        {
            sizeY *= 10;
            sizeY += buff - '0';
            input.read(&buff, 1);
        }

        int huesCount = 0;
        input.read(&buff, 1);
        while(buff != '\n')
        {
            huesCount *= 10;
            huesCount += buff - '0';
            input.read(&buff, 1);
        }
        if (huesCount != 255)
        {
            throw std::runtime_error("Invalid count of hues: '" + std::to_string(huesCount) + "'");
        }

        storage.clear();
        storage.resize(sizeY);
        for (int y = 0; y < sizeY; ++y)
        {
            for (int x = 0; x < sizeX; ++x)
            {
                input.read(&buff, 1);
                storage.at(y).push_back(buff);
            }
        }
    }

    void saveToFile(const std::string& path) const
    {
        std::ofstream output;
        output.open(path, std::ios::binary);
        if (!output.is_open()) 
        {
            throw std::ios_base::failure("Can`t open output file");
        }
        output.write("P5\n", 3);
        
        output.write(std::to_string(sizeX).c_str(), ceil(log10(sizeX)));
        output.write(" ", 1);
        output.write(std::to_string(sizeY).c_str(), ceil(log10(sizeY)));
        output.write("\n", 1);
        output.write("255\n", 4);

        for (int y = 0; y < sizeY; ++y)
        {
            for (int x = 0; x < sizeX; ++x)
            {
                output.write(&storage.at(y).at(x), sizeof(char));
            }
        }
    }
};

