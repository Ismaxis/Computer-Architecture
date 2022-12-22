#pragma once

#include "typedef.h"

class AbstractStruct {
   protected:
    template <typename T>
    static void read(T& place, const int bytes, std::istream& f) {
        f.read((char*)(&place), bytes);
    }

   public:
    virtual ~AbstractStruct() = default;
    virtual void fill(std::istream& f) = 0;
};