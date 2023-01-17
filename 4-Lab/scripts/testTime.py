
import subprocess


def withoutChunkSize():
    print("\nStatic:")
    for i in range(1, 13):
        subprocess.run(["..\\omp4TestStatic.exe", str(i),
                        "..\images\\4k.pgm", ".\\4k_out.pgm"])
    print("\nDynamic:")
    for i in range(1, 13):
        subprocess.run(["..\\omp4TestDynamic.exe", str(i),
                        "..\images\\4k.pgm", ".\\4k_out.pgm"])


for _ in range(3):
    subprocess.run(["..\\omp4TestDynamic.exe", str(1),
                    "..\images\\4k.pgm", ".\\4k_out.pgm"])
