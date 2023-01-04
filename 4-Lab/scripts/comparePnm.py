import pnmFuncs

# woman = pnmFuncs.imageFromBinaryFile("../images/woman.pnm")
# woman_bin = pnmFuncs.imageFromBinaryFile("../images/woman_bin.pnm")
# woman_bin_new = pnmFuncs.imageFromBinaryFile("../images/woman_bin_new.pnm")
# drop = pnmFuncs.imageFromBinaryFile("../images/drop.pnm")
# drop_bin = pnmFuncs.imageFromBinaryFile("../images/drop_bin.pnm")
# drop_bin_new = pnmFuncs.imageFromBinaryFile("../images/drop_bin_new.pnm")
# drop_bin_new_3 = pnmFuncs.imageFromBinaryFile("../images/drop_bin_new_3.pnm")
imageA = pnmFuncs.imageFromBinaryFile(
    "..\itmo-comp-arch22-lab4-Ismaxis\\test_data\\in.pgm")
imageB = pnmFuncs.imageFromBinaryFile(
    "..\itmo-comp-arch22-lab4-Ismaxis\\test_data\\out.pgm")
imageC = pnmFuncs.imageFromBinaryFile(
    "..\itmo-comp-arch22-lab4-Ismaxis\\test_data\\out_new.pgm")

pnmFuncs.putTogether((imageA, imageB, imageC))
