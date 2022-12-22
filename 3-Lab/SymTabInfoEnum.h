#pragma once
#include <string>
enum STT : char {
    NOTYPE = 0,
    OBJECT = 1,
    FUNC = 2,
    SECTION = 3,
    FILE_TYPE = 4,
    COMMON = 5,
    LOOS = 10,
    HIOS = 12,
    LOPROC = 13,
    HIPROC = 15,
};

inline std::string toStringSTT(STT type) {
    if (type == NOTYPE) {
        return "NOTYPE";
    } else if (type == SECTION) {
        return "SECTION";
    } else if (type == OBJECT) {
        return "OBJECT";
    } else if (type == FUNC) {
        return "FUNC";
    } else if (type == FILE_TYPE) {
        return "FILE";
    } else {
        return "STT '" + std::to_string(type) + "'not defined yet";
    }
}

enum STB : char {
    LOCAL = 0,
    GLOBAL = 1,
    WEAK = 2,
    // LOOS = 10,
    // HIOS = 12,
    // LOPROC = 13,
    // HIPROC = 15,
};

inline std::string toStringSTB(STB type) {
    if (type == LOCAL) {
        return "LOCAL";
    } else if (type == GLOBAL) {
        return "GLOBAL";
    } else if (type == WEAK) {
        return "WEAK";
    } else {
        return "STB '" + std::to_string(type) + "' not defined yet";
    }
}

enum STV : char {
    DEFAULT = 0,
    INTERNAL = 1,
    HIDDEN = 2,
    PROTECTED = 3,
};

inline std::string toStringSTV(STV type) {
    if (type == DEFAULT) {
        return "DEFAULT";
    } else if (type == INTERNAL) {
        return "INTERNAL";
    } else if (type == HIDDEN) {
        return "HIDDEN";
    } else if (type == PROTECTED) {
        return "PROTECTED";
    } else {
        return "STV '" + std::to_string(type) + "' not defined yet";
    }
}

enum SHN : int {
    UNDEF = 0,
    LORESERVE = 0xff00,
    ABS = 0xfff1,
    SHN_COMMON = 0xfff2,
    HIRESERVE = 0xffff,
};

inline std::string toStringSHN(SHN type) {
    if (type == UNDEF) {
        return "UNDEF";
    } else if (type == LORESERVE) {
        return "LORESERVE";
    } else if (type == ABS) {
        return "ABS";
    } else if (type == SHN_COMMON) {
        return "COMMON";
    } else if (type == HIRESERVE) {
        return "HIRESERVE";
    } else if (0xff00 <= type && type <= 0xff1f) {
        return "SPEC: " + std::to_string(type);
    } else {
        return std::to_string(type);
    }
}