
import subprocess
for i in range(-1, 13):
    subprocess.run(["..\\Release\\4-Lab.exe", str(i),
                    "..\images\\4k.pgm", ".\\4k_out.pgm"])
