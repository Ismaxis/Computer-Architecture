#pragma once

#include "typedef.h"

class AbstractStruct {
   protected:
    template <typename T>
    void read(T& place, int bytes, std::ifstream& f) {
        f.read((char*)(&place), bytes);
    }
};