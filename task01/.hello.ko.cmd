cmd_/home/jason/eudyptula/task01/hello.ko := ld -r -m elf_x86_64  -z max-page-size=0x200000  --build-id=sha1  -T ./scripts/module-common.lds -o /home/jason/eudyptula/task01/hello.ko /home/jason/eudyptula/task01/hello.o /home/jason/eudyptula/task01/hello.mod.o;  true
