import sys
import numpy as np
import utilidades as fn
from pprint import pprint


def main():
    salidaPorPantalla = True

    if len(sys.argv) < 3:
        print("ERROR: falta especificar nombre de archivo.")
        return None
    elif len(sys.argv) == 4:
        if sys.argv[-1] == "-o":
            salidaPorPantalla = False
        else:
            print("ERROR: flag invalido. Solo [-o] es sopotado.")
            return None
    
    pathCompleto = sys.argv[1]
    pathParaGuardar = sys.argv[2]

    # Abro el programa
    # obtengo una lista de str con cada linea del archivo
    lista_de_lineas_arch = fn.abrirAsmFile(pathCompleto)
    # pprint(lista_de_lineas_arch)
    # print()

    # separo por lineas
    # obtengo una lista de str solo con mnemotico y operandos
    lista_instr_operandos = fn.conviertoLineasEnListas(lista_de_lineas_arch)
    # pprint(lista_instr_operandos)
    # print()

    # genero programa con reemplazos de valores
    # obtengo una lista de tuplas con 
    programaDecodificado = fn.generoListaFinal(lista_instr_operandos)
    # pprint(programaDecodificado)
    # print()

    # genero codigo
    codigo = fn.generoCodigo(programaDecodificado)
    # pprint(codigo)
    # print()

    if salidaPorPantalla:
        _, megaTexto = fn.generoListasDeStrings(codigo, programaDecodificado)
        print(megaTexto)

    if not fn.errores:
        numpyCod = np.array(codigo)
        numpyCod = numpyCod.astype(np.int32)
        filename = pathParaGuardar
        fileobj = open(filename, mode='wb')
        numpyCod.tofile(fileobj)
        fileobj.close()
    else:
        for nroLinea, tipoError in fn.errores.items():
            print(f"Linea {nroLinea}: {fn.tipos_errores[tipoError]}")


if __name__ == "__main__":
    main()
