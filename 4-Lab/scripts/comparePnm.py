import pnmFuncs

images = []

# for i in range(1, 13):
#     images.append(pnmFuncs.imageFromBinaryFile(
#         f'../images/threads/out_{i}.pnm'))

# pnmFuncs.putTogether(images)


pnmFuncs.putTogether([
    pnmFuncs.imageFromBinaryFile(
        "..\\Release\\out.pgm"),
    pnmFuncs.imageFromBinaryFile(
        "..\\Release\\out2.pgm"),
    pnmFuncs.imageFromBinaryFile(
        "..\\Release\\out3.pgm"),
    pnmFuncs.imageFromBinaryFile(
        "..\\Debug\\out.pgm")
])
