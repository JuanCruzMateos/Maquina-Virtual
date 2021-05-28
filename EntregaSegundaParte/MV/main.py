import numpy as np
import utilidades as fn
from pprint import pprint


def main(argv):
    salidaPorPantalla = True

    if len(argv) < 3 or (len(argv) == 3 and argv[-1] == "-o"):
        print(fn.Colors.RED + "ERROR:" + fn.Colors.RESETCOLOR + " falta especificar nombre de archivo .bin")
        return None
    elif len(argv) == 4:
        if argv[-1] == "-o":
            salidaPorPantalla = False
        else:
            print(fn.Colors.RED + "ERROR:" + fn.Colors.RESETCOLOR + " flag invalido. Solo [-o] es admitido.")
            return None
    elif len(argv) > 4:
        print(fn.Colors.RED + "ERROR:" + fn.Colors.RESETCOLOR + " numero de parametros invalido.")
        return None

    # supongo que se respeta formato
    pathCompleto = argv[1]
    pathParaGuardar = argv[2]

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
        # TODO agregar headers in numpyCod
        fn.agregarInfoHeaders(codigo)
        # TODO agregar strings (van en DS)
        fn.agregarStringsDS(codigo)
        numpyCod = np.array(codigo)
        numpyCod = numpyCod.astype(np.int32)
        filename = pathParaGuardar
        fileobj = open(filename, mode='wb')
        numpyCod.tofile(fileobj)
        fileobj.close()
    else:
        print(fn.Colors.RED + fn.Colors.UNDERLINE + "Errores" + fn.Colors.NOUNDERLINE + ":")
        for nroLinea, tipoError in fn.errores.items():
            print(f" - Linea {nroLinea:>2d}: {fn.tipos_errores[tipoError]}")
        print(fn.Colors.RESETCOLOR)
    # pprint(fn.saltos)
    # pprint(fn.strings)

if __name__ == "__main__":
    import sys
    main(sys.argv)
