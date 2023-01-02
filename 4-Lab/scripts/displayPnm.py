import pygame as pg

PIXEL_SIZE = 2

x = 0
y = 0
image = []
with open("../images/drop.pnm", "rb") as file:
    print(file.read(3))
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

    for i in range(y):
        image.append([])
        for j in range(x):
            r = file.read(1)
            image[i].append(int.from_bytes(r, "little"))

WIN_SIZE = (x*PIXEL_SIZE, y*PIXEL_SIZE)
win = pg.display.set_mode(WIN_SIZE)

for i in range(y):
    for j in range(x):
        if (i == -1):
            pg.draw.rect(win, (255, 0, 0), (j*PIXEL_SIZE, i *
                         PIXEL_SIZE, PIXEL_SIZE, PIXEL_SIZE))
        else:
            pg.draw.rect(win, (image[i][j], image[i][j],
                               image[i][j]), (j*PIXEL_SIZE, i*PIXEL_SIZE, PIXEL_SIZE, PIXEL_SIZE))

pg.display.update()

while True:
    for event in pg.event.get():
        if event.type == pg.QUIT:
            quit()
