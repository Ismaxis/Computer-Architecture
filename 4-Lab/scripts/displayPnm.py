import pygame as pg

PIXEL_SIZE = 20

x = 0
y = 0
image = []
with open("../images/image1_mod.pnm", "rb") as file:
    file.readline()
    sizes = file.readline().split()
    x = int(sizes[0])
    y = int(sizes[1])
    file.readline()
    for i in range(y):
        line = file.readline().split()
        image.append([])
        for j in range(x):
            image[i].append(int(line[j]))

WIN_SIZE = (x*PIXEL_SIZE, y*PIXEL_SIZE)
win = pg.display.set_mode(WIN_SIZE)

for i in range(y):
    for j in range(x):
        pg.draw.rect(win, (image[i][j], image[i][j],
                     image[i][j]), (j*PIXEL_SIZE, i*PIXEL_SIZE, PIXEL_SIZE, PIXEL_SIZE))

pg.display.update()

while True:
    for event in pg.event.get():
        if event.type == pg.QUIT:
            quit()
