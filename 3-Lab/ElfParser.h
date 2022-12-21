#include <fstream>

#include "Structs/ElfHeader.h"
#include "Structs/ProgrammHeader.h"
class ElfParser {
   private:
    std::ifstream& file;

    ElfHeader elfHeader;

    ProgrammHeader* programmHeaders;

    void skip(int n);

   public:
    ElfParser(std::ifstream& f);
    ~ElfParser();

    void parse();
    uint32_t getSizeOfText();
    uint32_t getEntryPoint();
};
