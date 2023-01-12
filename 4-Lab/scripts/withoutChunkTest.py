import subprocess


def withoutChunkSize():
    print("\nStatic:")
    for i in range(1, 13):
        subprocess.run([".\\omp4TestStatic.exe", str(i),
                        ".\\16k.pgm", ".\\6k_out.pgm"])
    print("\nDynamic:")
    for i in range(1, 13):
        subprocess.run([".\\omp4TestDynamic.exe", str(i),
                        ".\\16k.pgm", ".\\16k_out.pgm"])


for _ in range(3):
    withoutChunkSize()
