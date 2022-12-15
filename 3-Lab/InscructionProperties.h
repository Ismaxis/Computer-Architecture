#include <iostream>

#include "Type.h"

using namespace std;
class InscructionProperties {
   private:
    string mnemonic;
    Type type;

   public:
    InscructionProperties(string mnemonic, Type type) {
        this->mnemonic = mnemonic;
        this->type = type;
    }

    string getMnemonic() const {
        return mnemonic;
    }

    Type getType() const {
        return type;
    }
};
