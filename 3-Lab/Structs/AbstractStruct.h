#pragma once

#include "typedef.h"

class AbstractStruct {
   protected:
    template <typename T>
    void read(T& place, int bytes, std::istream& f) {
        f.read((char*)(&place), bytes);
    }

   public:
    virtual void fill(std::istream& f) = 0;
};