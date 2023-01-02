import pygame as pg


class PnmImage:
    def __init__(self, x, y) -> None:
        self.x = x
        self.y = y
        self.storage = []


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


PIXEL_SIZE = 20

imageA = imageFromFile("../images/image1.pnm")
imageB = imageFromFile("../images/image1_bin.pnm")

WIN_SIZE = (imageA.x*PIXEL_SIZE, (imageA.y + imageB.y)*PIXEL_SIZE)
win = pg.display.set_mode(WIN_SIZE)

for i in range(imageA.y):
    for j in range(imageA.x):
        pg.draw.rect(win, (imageA.storage[i][j], imageA.storage[i][j],
                     imageA.storage[i][j]), (j*PIXEL_SIZE, i*PIXEL_SIZE, PIXEL_SIZE, PIXEL_SIZE))

for i in range(imageB.y):
    for j in range(imageB.x):
        pg.draw.rect(win, (imageB.storage[i][j], imageB.storage[i][j],
                     imageB.storage[i][j]), (j*PIXEL_SIZE, (imageA.y + i)*PIXEL_SIZE, PIXEL_SIZE, PIXEL_SIZE))

pg.display.update()

while True:
    for event in pg.event.get():
        if event.type == pg.QUIT:
            quit()
