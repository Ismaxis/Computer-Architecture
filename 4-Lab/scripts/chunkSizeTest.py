import subprocess


def chunkSize():
    chunks = [1, 2, 4, 8, 16]
    print("\nStatic:")
    for i in chunks:
        subprocess.run([".\\omp4TestStaticChunkSize.exe", "12",
                        ".\\images\\16k.pgm", ".\\16k_out.pgm", str(i)])
    print("\nDynamic:")
    for i in chunks:
        subprocess.run(["..\\omp4TestDynamicChunkSize.exe", "12",
                        ".\\images\\16k.pgm", ".\\16k_out.pgm", str(i)])


for _ in range(3):
    chunkSize()
