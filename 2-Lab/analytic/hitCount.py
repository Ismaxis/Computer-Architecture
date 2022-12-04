import numpy as np
import math

M = 64
N = 60
K = 32

CACHE_OFFSET_SIZE = 4
CACHE_SET_COUNT = 32
CACHE_SET_SIZE = int(math.log2(CACHE_SET_COUNT))  # 5
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

    def req(self, addr: int):
        setNum = Cache.getSet(addr)
        tag = Cache.getTag(addr)
        return self.checkHit(setNum, tag)

    def checkHit(self, setNum: int, tag: int):
        self.reqCount += 1
        for i in range(2):
            if (self.valid_array[setNum, i] and self.tag_array[setNum, i] == tag):
                self.hit(setNum, i)
                return True
        else:
            self.miss(setNum, tag)
            return False

    def hit(self, setNum: int, i: int):
        self.hitCount += 1
        self.lru_array[setNum] = (i == 0)

    def miss(self, setNum: int, tag: int):
        lru_index = int(self.lru_array[setNum])
        self.tag_array[setNum, lru_index] = tag
        self.valid_array[setNum, lru_index] = True
        self.lru_array[setNum] = not self.lru_array[setNum]

    @ staticmethod
    def getTag(address: int) -> int:
        return (address >> (CACHE_OFFSET_SIZE + CACHE_SET_SIZE))

    @ staticmethod
    def getSet(address: int) -> int:
        return (address >> CACHE_OFFSET_SIZE) % CACHE_SET_COUNT


def simulate(cache):
    pa = aStart
    pc = cStart
    for i in range(M):
        for j in range(N):
            pb = bStart
            for k in range(K):
                cache.req(pa + k*aIntSize)  # a
                cache.req(pb + j*bIntSize)  # b
                pb += N*bIntSize
            cache.req(pc + j*cIntSize)  # c
        pa += K*aIntSize
        pc += N*cIntSize


def validate(reqCount: int, hitCount: int):
    with open("hit_stat.dump", "r") as hit_stat:
        givenReqCount = int(hit_stat.readline())
        givenHitCount = int(hit_stat.readline())

        if (givenReqCount != reqCount or givenHitCount != hitCount):
            raise Exception(
                f'''{cl.OKBLUE}Results missmatch{cl.ENDC}
{cl.OKGREEN}Analytic{cl.ENDC}:   reqs {reqCount}, hits {hitCount}, rate {round(hitCount/reqCount, 6)}
{cl.WARNING}Simulation{cl.ENDC}: reqs {givenReqCount}, hits {givenHitCount}, rate {round(givenHitCount/givenReqCount, 6)}\n''')


def main():
    cache = Cache()
    simulate(cache)

    try:
        validate(cache.reqCount, cache.hitCount)
    except Exception as e:
        print("\nData validation" + cl.FAIL + " failed" + cl.ENDC)
        print(e)
    else:
        print("\nData validation" + cl.OKGREEN + " passed" + cl.ENDC)
        print(
            f'{cl.HEADER}{cl.BOLD}Requests{cl.ENDC}: {cache.reqCount}\n{cl.HEADER}{cl.BOLD}Hits{cl.ENDC}:     {cache.hitCount}')
        # print(
        #     f'{cl.WARNING}{round(cache.hitCount/cache.reqCount, 6)}{cl.ENDC} of requsets is {cl.UNDERLINE}{cl.OKBLUE}HITS{cl.ENDC}\n')
        print(f'{cl.UNDERLINE}{cl.OKBLUE}HIT RATE{cl.ENDC} is {cl.WARNING}{round(cache.hitCount/cache.reqCount, 6)}{cl.ENDC}\n')


if __name__ == '__main__':
    class cl:
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
