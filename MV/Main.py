import sys
import utilidades as fn
import numpy as np
# path = "C:\\Users\\Usuario\\Documents\\13 - INGENIERIA INFORMATICA\\Arquitectura de computadoras\\MV\\"
path = "/mnt/c/Users/mateo/Documents/Ingenieria/Arquitectura/Maquina Virtual/ASM/"

def main():
    # path = ""
    pathCorto = path+sys.argv[1]
    pathParaGuardarCorto = path+sys.argv[2]
    pathLargo = sys.argv[1]
    pathParaGuardarLargo = sys.argv[2]

    try:
        path1 = pathCorto
        path2 = pathParaGuardarCorto
        codigo = fn.ejecutoParser(path1, showText=True)
        exito = fn.guardoArchivoBin(path2, codigo)
    except:
        path1 = pathLargo
        path2 = pathParaGuardarLargo
        codigo = fn.ejecutoParser(path1, showText=True)
        exito = fn.guardoArchivoBin(path2, codigo)


if __name__ == "__main__":
    main()
