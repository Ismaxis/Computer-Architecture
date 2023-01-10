
import subprocess
for i in range(1, 13):
    subprocess.run(["..\\Release\\4-Lab.exe", str(i),
                    "..\\itmo-comp-arch22-lab4-Ismaxis\\test_data\in.pgm", "..\\out.pgm"])
