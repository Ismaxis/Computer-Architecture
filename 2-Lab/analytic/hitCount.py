import numpy as np
import math

M = 64
N = 60
K = 32

CACHE_OFFSET_SIZE = 4
CACHE_SET_COUNT = 32
CACHE_SET_SIZE = int(math.log2(CACHE_SET_COUNT))
CACHE_LINE_SIZE = 16

aStart = 0
aIntSize = 1
aSize = M*K*aIntSize

bStart = aStart + aSize
bIntSize = 2
bSize = K*N*bIntSize

cStart = bStart + bSize
cIntSize = 4
cSize = M*N*cIntSize


class Cache:

    def __init__(self):
        self.tag_array = np.zeros((CACHE_SET_COUNT, 2), dtype=int)
        self.valid_array = np.zeros((CACHE_SET_COUNT, 2), dtype=bool)
        self.lru_array = np.zeros(CACHE_SET_COUNT, dtype=bool)
        self.reqCount = 0
        self.hitCount = 0
        self.missCount = 0

    def req(self, arrayNum: int, i: int, j: int):
        addr = Cache.getAddress(arrayNum, i, j)
        setNum = Cache.getSet(addr)
        tag = Cache.getTag(addr)
        self.checkHit(setNum, tag)

    def checkHit(self, setNum: int, tag):
        self.reqCount += 1
        for i in range(2):
            if (self.valid_array[setNum, i] and self.tag_array[setNum, i] == tag):
                self.hitCount += 1
                self.lru_array[setNum] = ~self.lru_array[setNum]
                return

        self.missCount += 1
        lru_index = int(self.lru_array[int(self.lru_array[setNum])])
        self.tag_array[setNum, lru_index] = tag
        self.valid_array[setNum, lru_index] = True

    @ staticmethod
    def getTag(address: int) -> int:
        return(address >> (CACHE_OFFSET_SIZE + CACHE_SET_SIZE))

    @ staticmethod
    def getSet(address: int) -> int:
        return (address >> CACHE_OFFSET_SIZE) % CACHE_SET_COUNT

    @ staticmethod
    def getAddress(arrayNum: int, i: int, j: int) -> int:
        if (arrayNum == 0):
            return aStart + aIntSize*(M*i + j)
        elif (arrayNum == 1):
            return bStart + bIntSize*(M*i + j)
        elif (arrayNum == 2):
            return cStart + cIntSize*(M*i + j)
        pass


def simulate(cache):
    for i in range(M):
        for j in range(N):
            for k in range(K):
                cache.req(0, i, k)
                cache.req(1, k, j)

            cache.req(2, i, j)


def main():
    cache = Cache()
    simulate(cache)

    if (cache.reqCount == cache.hitCount + cache.missCount):
        print("\nData validation" + bcolors.OKGREEN + " passed" + bcolors.ENDC)
    else:
        print("\nData validation" + bcolors.FAIL + " failed" + bcolors.ENDC)
    print(
        f'{bcolors.HEADER}{bcolors.BOLD}Requests{bcolors.ENDC}: {cache.reqCount}\n{bcolors.HEADER}{bcolors.BOLD}Hits{bcolors.ENDC}:     {cache.hitCount}')
    print(
        f'{bcolors.WARNING}{round(cache.hitCount/cache.reqCount,3)}%{bcolors.ENDC} of requsets is {bcolors.UNDERLINE}{bcolors.OKBLUE}HITS{bcolors.ENDC}\n')


if __name__ == '__main__':
    class bcolors:
        HEADER = '\033[95m'
        OKBLUE = '\033[94m'
        OKCYAN = '\033[96m'
        OKGREEN = '\033[92m'
        WARNING = '\033[93m'
        FAIL = '\033[91m'
        ENDC = '\033[0m'
        BOLD = '\033[1m'
        UNDERLINE = '\033[4m'
    main()
