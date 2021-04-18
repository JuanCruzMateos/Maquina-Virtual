import numpy as np
import re

hashmap = {'MOV': [0, 2],
           'ADD': [1, 2],
           'SUB': [2, 2],
           'SWAP': [3, 2],
           'MUL': [4, 2],
           'DIV': [5, 2],
           'CMP': [6, 2],
           'SHL': [7, 2],
           'SHR': [8, 2],
           'AND': [9, 2],
           'OR': [10, 2],
           'XOR': [11, 2],
           'SYS': [240, 1],
           'JMP': [241, 1],
           'JZ': [242, 1],
           'JP': [243, 1],
           'JN': [244, 1],
           'JNZ': [245, 1],
           'JNP': [246, 1],
           'JNN': [247, 1],
           'LDL': [248, 1],
           'LDH': [249, 1],
           'RND': [250, 1],
           'NOT': [251, 1],
           'STOP': [4081, 0]
}

registros = {'DS': 0,
             'IP': 5,
             'CC': 8,
             'AC': 9,
             'AX': 10,
             'BX': 11,
             'CX': 12,
             'DX': 13,
             'EX': 14,
             'FX': 15
}

base = {
    "#": 10,
    "@": 8,
    "%": 16,
    "'": "ASCII"
}

saltos = {}

etiqueta = {}

comentarios = {}

errores = {}


def abrirAsmFile(nombreArchivo: str) -> list:
    """Devuelve una lista de str con las lineas de los archivos"""
    f = open(nombreArchivo, "r")
    programaEntero = f.read()
    programaEnteroEnLineas = programaEntero.split("\n")
    # f.close()
    return programaEnteroEnLineas


def conviertoLineasEnListas(programaEnteroEnLineas: list) -> list:
    """Devuelve una lista de listas de string por cada linea"""
    programaEnteroEnListas = []
    for lineas in programaEnteroEnLineas:
        programaEnteroEnListas.append(lineas.split())
    return programaEnteroEnListas


def eliminoLineasVacias(programaEnteroEnListas):
    """Elimina lineas vacias"""
    programaEnteroEnListasClean = []
    for linea in programaEnteroEnListas:
        # if linea:
        #     programaEnteroEnListasClean.append(linea)
        if linea == []:
            continue
        else:
            programaEnteroEnListasClean.append(linea)
    return programaEnteroEnListasClean


def dropComentariosDeUnaLinea(programaEnteroEnListas):
    """Elimina lineas con solo comntarios"""
    programaEnteroEnListasClean = []
    nroLineas = 0
    for lista in programaEnteroEnListas:
        if lista[0][0] == ";":
            continue
        else:
            programaEnteroEnListasClean.append(lista)
            nroLineas += 1
    return programaEnteroEnListasClean, nroLineas


def generoListaSinComentariosNiRotulos(programaEnteroEnListas):

    nroLinea = 0
    listaAux = []
    for linea in programaEnteroEnListas:
        linea2 = buscoRotuloYComentario(linea, nroLinea)
        listaAux.append(linea2)
        nroLinea += 1
    return listaAux


def buscoRotuloYComentario(linea, nroLinea):
    # El rotulo solo pueden estar al inicio de una linea
    if re.search(":$", linea[0]) != None:
        rotulo = linea.pop(0)
        rotulo = rotulo.replace(':', '')
        saltos[rotulo] = nroLinea
        etiqueta[nroLinea] = rotulo
    # Ahora busco comentarios
    posComentario = 0
    hayComentario = False
    i = 0
    while i < len(linea) and not hayComentario:
        if re.search("^;", linea[i]) != None:
            posComentario = i
            hayComentario = True
        i += 1

    # si hay un comentario, armo un string con todos los componentes de la
    # lista que estan luego de la palabra que tiene punto y coma
    if hayComentario:
        linea[posComentario] = linea[posComentario].replace(';', '')
        comentario = " ".join(linea[posComentario:])
        comentarios[nroLinea] = comentario
        linea = linea[:posComentario]
    return linea


def decodificoLinea(linea, numLinea):
    # identificamos si el mnemonico existe
    mnemonico = linea[0].upper()
    if mnemonico not in hashmap:
        errores[numLinea] = "Linea " + \
            str(numLinea)+" con Error - mnemonico erroneo"
        return numLinea, None, None, None, None, None, None, None
    else:
        # Si el mnemonico existe, obtengo su codigo
        codigoMnemonico = hashmap[mnemonico][0]
        # Obtengo la cantidad de parametros asociados al mnemonico
        cantidadOperandosNecesarios = hashmap[mnemonico][1]
        # Obtengo la cantidad de parametros que efectivamente tengo
        cantidadOperandosEncontrados = len(linea)-1
        # verificamos que la cantidad de operandos coincide con los encontrados
        if cantidadOperandosEncontrados != cantidadOperandosNecesarios:
            errores[numLinea] = "Linea " + \
                str(numLinea)+" con Error - Cantidad de operandos incorrectos"
            return numLinea, None, None, None, None, None, None, None
        else:
            # Si no hay error en la cantidad de parametros, paso a decodificarlos
            valorOperandos = []
            tipoDeOperandos = []
            operandos = []
            if cantidadOperandosNecesarios == 1:
                operandos = linea[1:]
            elif cantidadOperandosNecesarios == 2:
                operandos = linea[1:]
                # cuando hay dos parametros, el primero tiene una coma, se la borro
                operandos[0] = operandos[0].replace(',', '')
            for operando in operandos:
                # print(operando)
                opTipo, opVal = devuelveTipoOperandoYValorDecimal(operando)
                valorOperandos.append(opVal)
                tipoDeOperandos.append(opTipo)
            return numLinea, mnemonico, codigoMnemonico, cantidadOperandosNecesarios, valorOperandos, tipoDeOperandos, operandos


def generoListaFinal(programaEnteroEnListas):
    numLinea = -1
    programaFinal = []
    for lista in programaEnteroEnListas:
        numLinea += 1
        programaFinal.append(decodificoLinea(lista, numLinea))
    return programaFinal


def devuelveTipoOperandoYValorDecimal(operando):
    # verificamos si el operando es un registro
    # devuelve tipo = 1 y valor del registro
    if operando.upper() in registros.keys():
        return 1, registros[operando.upper()]

    # Si no es un registro, verificamos si es un operador directo
    # Si es devuelve tipo = 2, y valor a reemplazar
    if re.search("\\[", operando):
        ini = operando.index("[")
        fin = operando.index("]")
        return 2, cambioBase(operando[ini+1:fin])
    # Sino, es un valor inmediato
    # Devuelve tipo = 0 y el valor a reemplazar
    return 0, cambioBase(operando)


def cambioBase(operando):
    # Si la primer posicion coincide con lo almacenados en bases
    # @, #, % y '
    if operando[0] in base.keys():
        # obtengo la base
        baseOperando = base[operando[0]]
        # descarto el primer valor
        operandoAux = operando[1:]
        # cuando son ASCII pueden tener una comilla mas
        if not operandoAux.isnumeric():
            operandoAux = operandoAux[:-1]

    # sino estan en la lista solo quedan dos opciones
    # opcion 1 --> que sea un valor decimal puro
    elif operando.isnumeric():
        baseOperando = 10
        operandoAux = operando[:]
    # opcion 2 --> que sea una etiqueta
    else:
        baseOperando = 500  # es una etiqueta

    if (baseOperando != 500):
        if baseOperando == "ASCII":
            valor = ord(operandoAux)
        else:
            valor = int(operandoAux, baseOperando)
    else:
        if operando.lower() in saltos:
            valor = saltos[operando]
        else:
            valor = None

    return valor


def generaValorCodificado(codMnemonico, cantidadOperandos, tipoOperandos, operandos):
    if cantidadOperandos == 2:
        codigo = operacion2Parametros(
            codMnemonico, operandos[0], operandos[1], tipoOperandos[0], tipoOperandos[1])
    elif cantidadOperandos == 1:
        codigo = operacion1Parametro(
            codMnemonico, operandos[0], tipoOperandos[0])
    else:
        codigo = operacion0Parametros(codMnemonico)
    return codigo


def operacion2Parametros(codigoOperacion, operando1, operando2, tipoOperando1, tipoOperando2):
    codigo = np.left_shift(codigoOperacion & 0x00F, 28, dtype=np.int64)
    tipoA = np.left_shift(tipoOperando1, 26, dtype=np.int64)
    a = np.left_shift(operando1 & 0xFFF, 12, dtype=np.int64)
    tipoB = np.left_shift(tipoOperando2 & 0x003, 24, dtype=np.int64)
    b = operando2 & 0xFFF
    # print("--------------------------------------------")
    # print("Valor maximo de los operandos ", 4096)
    # print("Valor operando 1 ", operando1&0xFFF)
    # print("Valor operando 2 ", operando2&0xFFF)
    # print("Valor maximo del codigo ", 16)
    # print("Valor del codigo ", codigoOperacion&0x00F)
    # print("--------------------------------------------")
    codigoFull = codigo | a | b | tipoA | tipoB
    return codigoFull


def operacion1Parametro(codigoOperacion, operando1, tipoOperando1):

    unos = np.left_shift(15, 28, dtype=np.int64)
    codigo = np.left_shift(codigoOperacion & 0x00F, 24, dtype=np.int64)
    tipoA = np.left_shift(tipoOperando1, 22, dtype=np.int64)
    a = operando1 & 0xFFFF
    # print("{0:x}".format(unos).zfill(9))
    # print("Valor operando ", operando1&0x00F)

    # print("Valor maximo del codigo ", 16)
    # print("Valor del codigo ", codigoOperacion&0x00F)
    codigoFull = codigo | a | tipoA | unos
    return codigoFull


def operacion0Parametros(codigoOperacion):
    unos = np.left_shift(0xFF, 24, dtype=np.int64)
    codigo = np.left_shift(codigoOperacion & 0x00F, 20, dtype=np.int64)
    # print("Valor maximo del codigo ", 16)
    # print("Valor del codigo ", codigoOperacion&0x00F)
    codigoFull = codigo | unos
    return codigoFull


def generoCodigo(programaFinal):
    if errores == {}:  # si no hay errores
        codigos = []
        for linea in programaFinal:
            codigo = generaValorCodificado(
                linea[2], linea[3], linea[5], linea[4])
            codigos.append(codigo)
        return codigos
    else:
        return None


def generoListasDeStrings(codigos, programaFull):
    texto = []
    for i in range(len(codigos)):

        lineaEnHexa = "["+"{0:X}".format(programaFull[i][0]).zfill(4)+"]"
        codigoEnHexa = "{0:X}".format(codigos[i]).zfill(8)
        if i in etiqueta.keys():
            lin = etiqueta[i]+":"
        else:
            lin = ""
        lin = '{0: <8}'.format(lin)
        mnemonico = programaFull[i][1]
        mnemonico = '{0: <8}'.format(mnemonico)

        # genero operadores
        if programaFull[i][3] == 2:
            op1 = programaFull[i][6][0]
            op2 = programaFull[i][6][1]
            op1 = '{0: <6}'.format(op1)
            op2 = '{0: <6}'.format(op2)
            ope = op1+", " + op2
        elif programaFull[i][3] == 1:
            ope = programaFull[i][6][0]
        else:
            ope = ""

        ope = '{0: <15}'.format(ope)

        if i in comentarios.keys():
            coment = ";"+comentarios[i]
        else:
            coment = ""

        lineaDeTexto = lineaEnHexa+" "+codigoEnHexa+" " + \
            lin+" "+mnemonico+" "+ope+" " + " "+coment

        texto.append(lineaDeTexto)
    return texto
