#pragma once
#include <fstream>
#include <string>
#include <vector>

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
            const std::string message = "Unknown File type: '" + type + "'";
            throw std::runtime_error(message);
        }

        std::string width;
        input >> width;

        std::string height;
        input >> height;

        int huesCount;
        std::string huesCountStr;
        input >> huesCountStr;
        try
        {
            sizeX = std::stoi(width);
            sizeY = std::stoi(height);
            huesCount = std::stoi(huesCountStr);
        }
        catch (std::invalid_argument& e)
        {
            const std::string message = "Width, Height and hues count should be numbers:\n\tWidth: " + width +
                "\n\tHeight: " + height +
                "\n\tHues Count: " + huesCountStr;
            throw std::runtime_error(message);
        }

        if (sizeX <= 0 || sizeY <= 0)
        {
            throw std::runtime_error("Width and Height should be positive:\n\tWidth: " + width +
                "\n\tHeight: " + height);
        }

        if (huesCount != 255)
        {
            throw std::runtime_error(
                "Invalid count of hues: '" + huesCountStr + "'\nExpected: '" + std::to_string(
                    INTENSITY_LAYER_COUNT - 1) + "'");
        }

        input.ignore(sizeof(char)); // \n

        storage.clear();
        storage.resize(sizeY);

        for (int y = 0; y < sizeY; ++y)
        {
            for (int x = 0; x < sizeX; ++x)
            {
                uint8_t uintBuff = 0;
                input.read((char*)&uintBuff, sizeof(uint8_t));
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
        const std::string headerStr = "P5\n" + std::to_string(sizeX) + " " + std::to_string(sizeY) + "\n255\n";
        output.write(headerStr.c_str(), headerStr.size());

        for (int y = 0; y < sizeY; ++y)
        {
            for (int x = 0; x < sizeX; ++x)
            {
                output.write((char*)&storage.at(y).at(x), sizeof(uint8_t));
            }
        }
    }

    void applyThresholds(const std::vector<int>& thresholds, const bool ompEnabled = true)
    {
        auto* classes = new uint8_t[INTENSITY_LAYER_COUNT];
        fillClassesArray(classes, thresholds);
#pragma omp parallel if (ompEnabled)
        {
#pragma omp for schedule(static)
            for (int y = 0; y < getYSize(); ++y)
            {
                for (int x = 0; x < getXSize(); ++x)
                {
                    setPixel(classes[getPixel(x, y)], x, y);
                }
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
