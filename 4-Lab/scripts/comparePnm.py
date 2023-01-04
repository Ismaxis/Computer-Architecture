import pygame as pg

import pnmFuncs

PIXEL_SIZE = 1

imageA = pnmFuncs.imageFromBinaryFile("../images/woman.pnm")
imageB = pnmFuncs.imageFromBinaryFile("../images/woman_bin.pnm")

WIN_SIZE = ((imageA.x + imageB.x)*PIXEL_SIZE,
            max(imageA.y, imageB.y)*PIXEL_SIZE)
win = pg.display.set_mode(WIN_SIZE)

for i in range(imageA.y):
    for j in range(imageA.x):
        pg.draw.rect(win, (imageA.storage[i][j], imageA.storage[i][j],
                     imageA.storage[i][j]), (j*PIXEL_SIZE, i*PIXEL_SIZE, PIXEL_SIZE, PIXEL_SIZE))

for i in range(imageB.y):
    for j in range(imageB.x):
        pg.draw.rect(win, (imageB.storage[i][j], imageB.storage[i][j],
                           imageB.storage[i][j]), ((imageA.x + j)*PIXEL_SIZE, i*PIXEL_SIZE, PIXEL_SIZE, PIXEL_SIZE))

pg.display.update()

while True:
    for event in pg.event.get():
        if event.type == pg.QUIT:
            quit()
