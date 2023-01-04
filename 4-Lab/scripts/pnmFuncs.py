import numpy as np


class PnmImage:
    def __init__(self, x, y) -> None:
        self.x = x
        self.y = y
        self.storage = []

    def getNpArray(self) -> np.array:
        return np.array(self.storage)


def imageFromFile(path: str) -> PnmImage:
    with open(path, "rb") as file:
        file.readline()
        sizes = file.readline().split()
        image = PnmImage(int(sizes[0]), int(sizes[1]))
        file.readline()
        for i in range(image.y):
            line = file.readline().split()
            image.storage.append([])
            for j in range(image.x):
                image.storage[i].append(int(line[j]))
        return image


def imageFromBinaryFile(path: str) -> PnmImage:
    with open(path, "rb") as file:
        file.read(3)
        byte = file.read(1)
        x = 0
        while (byte != b' '):
            x *= 10
            x += int(byte)
            byte = file.read(1)

        byte = file.read(1)
        y = 0
        while (byte != b'\n'):
            y *= 10
            y += int(byte)
            byte = file.read(1)

        file.read(4)  # 255\n
        image = PnmImage(x, y)
        for i in range(y):
            image.storage.append([])
            for j in range(x):
                r = file.read(1)
                image.storage[i].append(
                    int.from_bytes(r, "little", signed=False))

        return image
