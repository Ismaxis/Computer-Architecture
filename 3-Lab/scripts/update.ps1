./scripts/asm.ps1 ./resources/test.cpp ./resources/out.elf; if ($?) {./scripts/disasm.ps1 ./resources/out.elf}
