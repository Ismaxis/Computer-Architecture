import os
import re


def glob_re(pattern, strings):
    return filter(re.compile(pattern).match, strings)


filenames = glob_re(r'.*\.(h|cpp)', os.listdir())


with open("merged.txt", "wb") as outfile:
    for f in filenames:
        with open(f, "rb") as infile:
            outfile.write(str.encode('\n' + f + '\n'))
            outfile.write(infile.read())
            outfile.write(str.encode('\n'))
