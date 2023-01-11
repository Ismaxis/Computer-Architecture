import pnmFuncs

images = []

# for i in range(1, 13):
#     images.append(pnmFuncs.imageFromBinaryFile(
#         f'../images/threads/out_{i}.pnm'))

# pnmFuncs.putTogether(images)


pnmFuncs.putTogether([
    # pnmFuncs.imageFromBinaryFile(
    #     "..\\Release\\4k_out.pgm"),
    pnmFuncs.imageFromBinaryFile(
        "..\\Release\\koval_4k.pgm")
])
