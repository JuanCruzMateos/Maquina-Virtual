import sys
import numpy as np
import utilidades as fn


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
    programaEnteroEnLineas = fn.abrirAsmFile(pathCompleto)
    # separo por lineas
    programaEnteroEnListas = fn.conviertoLineasEnListas(programaEnteroEnLineas)
    # genero programa con reemplazos de valores
    programaDecodificado = fn.generoListaFinal(programaEnteroEnListas)
    # genero codigo
    codigo = fn.generoCodigo(programaDecodificado)

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


if __name__ == "__main__":
    main()
