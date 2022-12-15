#include <iostream>

#include "Type.h"

class InstProps {
   private:
    std::string mnemonic;
    Type type;

   public:
    InstProps() = default;
    InstProps(std::string mnemonic, Type type) {
        this->mnemonic = mnemonic;
        this->type = type;
    }

    std::string getMnemonic() const {
        return mnemonic;
    }

    Type getType() const {
        return type;
    }
};
