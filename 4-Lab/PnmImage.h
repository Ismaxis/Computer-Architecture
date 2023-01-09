#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>

constexpr int INTENSITY_LAYER_COUNT = 256;

class PnmImage
{
private:
    int sizeX = 0;
    int sizeY = 0;
    std::vector<std::vector<uint8_t>> storage;
public:
    int getXSize() const
    {
        return sizeX;
    }

    int getYSize() const
    {
        return sizeY;
    }

    uint8_t getPixel(const int x, const int y) const
    {
        return storage.at(y).at(x);
    }

    void setPixel(const uint8_t value, const int x, const int y)
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
        while (buff != ' ')
        {
            sizeX *= 10;
            sizeX += buff - '0';
            input.read(&buff, 1);
        }
        input.read(&buff, 1);
        while (buff != '\n')
        {
            sizeY *= 10;
            sizeY += buff - '0';
            input.read(&buff, 1);
        }

        int huesCount = 0;
        input.read(&buff, 1);
        while (buff != '\n')
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
                uint8_t uintBuff = 0;
                input.read((char*)&uintBuff, 1);
                storage.at(y).push_back(uintBuff);
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
                output.write((char*)&storage.at(y).at(x), sizeof(uint8_t));
            }
        }
    }

    void applyThresholds(const std::vector<int>& thresholds)
    {
        auto* classes = new uint8_t[INTENSITY_LAYER_COUNT];
        fillClassesArray(classes, thresholds);
        for (int x = 0; x < getXSize(); ++x)
        {
            for (int y = 0; y < getYSize(); ++y)
            {
                setPixel(classes[getPixel(x, y)], x, y);
            }
        }
    }

private:
    static void fillClassesArray(uint8_t* classes, const std::vector<int>& thresholds)
    {
        for (int i = 0; i <= thresholds[0]; ++i)
        {
            classes[i] = 0;
        }

        for (int i = thresholds[0] + 1; i <= thresholds[1]; ++i)
        {
            classes[i] = 84;
        }

        for (int i = thresholds[1] + 1; i <= thresholds[2]; ++i)
        {
            classes[i] = 170;
        }

        for (int i = thresholds[2] + 1; i <= INTENSITY_LAYER_COUNT - 1; ++i)
        {
            classes[i] = 255;
        }
    }
};
