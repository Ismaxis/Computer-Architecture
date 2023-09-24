import subprocess

print("signlethread")
for _ in range(3):
    subprocess.run([".\\omp4TestDynamic.exe", str(1), ".\\16k.pgm", ".\\16k_out.pgm"])

print("disabled")
for _ in range(3):
    subprocess.run([".\\omp4TestDynamic.exe", str(-1), ".\\16k.pgm", ".\\16k_out.pgm"])
