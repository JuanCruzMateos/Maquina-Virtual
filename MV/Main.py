import sys
import pathlib
import utilidades as fn
import numpy as np

if __name__ == "__main__":
    pass

path = "/mnt/c/Users/mateo/Documents/Ingenieria/Arquitectura/Maquina Virtual/ASM/"

try:
    pathCompleto = path+sys.argv[1]
except:
    pathCompleto = path + "fibo.asm"
    print("Error - falta nombre de arhivo")

# Abro el programa
programaEnteroEnLineas = fn.abrirAsmFile(pathCompleto)
# separo por lineas
programaEnteroEnListas = fn.conviertoLineasEnListas(programaEnteroEnLineas)
# quito lineas en blanco
programaEnteroEnListasSinLineasBlanco = fn.eliminoLineasVacias(
    programaEnteroEnListas)
# borro comentarios de linea entera
programaSinComentariosUnaLinea, nroLineas = fn.dropComentariosDeUnaLinea(
    programaEnteroEnListasSinLineasBlanco)
# quito rotulos y comentarios (estan almacenados en diccionarios, se accede por numero de linea)
programaSinRotuloSinComentarios = fn.generoListaSinComentariosNiRotulos(
    programaSinComentariosUnaLinea)
# armo una estructura con toda la informacion que necesito antes de codificar
programaFinal = fn.generoListaFinal(programaSinRotuloSinComentarios)

print("Programa Final")
for x in programaFinal:
    if x[1] == None:
        print("Error")
    else:
        print(x)
print("numLinea - mnemonico - codigoMnemonico - cantidadOperandosNecesarios - valorOperandos - tipoDeOperandos - operadores originales")


codigos = fn.generoCodigo(programaFinal)

if codigos != None:
    texto = fn.generoListasDeStrings(codigos, programaFinal)
    for linea in texto:
        print(linea)

    numpyCod = np.array(codigos)

    for x in numpyCod:
        print(x)

# esto es para grabar el arhivo. Por ahora lo comento
filename = "fibo.bin"
fileobj = open(filename, mode='wb')
numpyCod.tofile(fileobj)
fileobj.close
