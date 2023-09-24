import pnmFuncs

images = []

for i in range(1, 13):
    images.append(pnmFuncs.imageFromBinaryFile(f"../images/threads/out_{i}.pnm"))

pnmFuncs.displayTogether(images)
