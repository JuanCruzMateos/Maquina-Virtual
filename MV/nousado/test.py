import utilidades as fn
import ctypes as c
import numpy
from pprint import pprint

# asmfile = "../ASM/fibo.asm"

# arch = fn.abrirAsmFile(asmfile)
# pprint(arch)
# print()

# programa = fn.conviertoLineasEnListas(arch)
# pprint(programa)


f = open("prueba.bin", "wb")
for i in range(10):
    f.write(c.c_int32(i+1))
f.close()


binfile = "prueba.bin"

arch = open(binfile, "rb")
a = numpy.fromfile(arch, dtype=numpy.int32)
pprint(a)
arch.close()
