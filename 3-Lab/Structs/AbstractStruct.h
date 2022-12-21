#pragma once
#include "typedef.h"

#ifndef Abstract_Struct
#define Abstract_Struct

class AbstractStruct {
   protected:
    template <typename T>
    void read(T& place, int bytes, std::ifstream& f) {
        f.read((char*)(&place), bytes);
    }
};
#endif