import re
import numpy as np

#       mnem : [codigo, nro operandos]
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

# registro : codigo
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

saltos = {
    # rotulo: nroLinea
}

etiqueta = {
    # nroLinea: rotulo
}

comentarios = {
    # nroLinea(int): comentario(str)
}

tipos_errores = ["Error sintaxis.", "No se encuentra rotulo.",
                 "Cantidad de operandos erronea."]

errores = {}

warnings = []

def abrirAsmFile(nombreArchivo: str) -> list:
    """Devuelve una lista de str con las lineas del archivo"""
    f = open(nombreArchivo, "r")
    programa = f.read()
    programaEnLineas = programa.split("\n")
    f.close()
    return programaEnLineas


def conviertoLineasEnListas(programaEnLineas: list) -> list:
    """Devuelve una lista de listas, cada una con mnem y op en str"""
    nroLinea = 0
    programaEnListas = []
    for lineas in programaEnLineas:
        linea = lineas.split()
        # lista vacia [] se evalua como False
        if linea and linea[0][0] != ';':
            linea = buscoRotuloYComentario(linea, nroLinea)
            quitarComas(linea)
            programaEnListas.append(linea)
            nroLinea += 1
    return programaEnListas


def quitarComas(linea: list) -> list:
    # linea = [linea[i].replace(',', '') if ',' in linea[i] else linea[i] for i in range(len(linea))]
    for i in range(len(linea)):
        if ',' in linea[i]:
            linea[i] = linea[i].replace(',', '')


def generoListaFinal(programaEnListas):
    """Devuelve una lista de tuplas con la linea decodificada"""
    numLinea = 0
    programaFinal = []
    for lista in programaEnListas:
        programaFinal.append(decodificoLinea(lista, numLinea))
        numLinea += 1
    return programaFinal


def generoCodigo(programaFinal):
    codigos = []
    for linea in programaFinal:
        numLinea = linea[0]
        if numLinea in errores.keys() and errores[numLinea] != 1:
            # error de mnemotico
            if errores[numLinea] == 0:
                codigo = 0xFFFFFFFF
            else:
                codigo = 0xEEEEEEEE
        else:
            codigo = generaValorCodificado(linea[2], linea[3], linea[5], linea[4], numLinea)
        codigos.append(codigo)
    return codigos


def buscoRotuloYComentario(linea: list, nroLinea: int) -> list:
    """Elimina comentarios y rotulos agregandolos a sus correspondientes dict"""
    # El rotulo solo pueden estar al inicio de una linea
    if re.search(":$", linea[0]) != None:
        rotulo = linea.pop(0)
        rotulo = rotulo.replace(':', '')
        saltos[rotulo] = nroLinea
        etiqueta[nroLinea] = rotulo
    # Ahora busco comentarios
    i = 0
    while i < len(linea) and re.search("^;", linea[i]) == None:
        i += 1
    # si hay un comentario, armo un string con todos los componentes de la
    # lista que estan luego de la palabra que tiene punto y coma
    if i < len(linea):
        linea[i] = linea[i].replace(';', '')
        comentario = " ".join(linea[i:])
        comentarios[nroLinea] = comentario
        linea = linea[:i]
    return linea


def decodificoLinea(linea: list, numLinea: int) -> tuple:
    # identificamos si el mnemonico existe
    mnemonico = linea[0].upper()
    if mnemonico not in hashmap:
        errores[numLinea] = 0
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
            errores[numLinea] = 2
            return numLinea, None, None, None, None, None, None, None
        else:
            # Si no hay error en la cantidad de parametros, paso a decodificarlos
            valorOperandos = []
            tipoDeOperandos = []
            operandos = []
            if cantidadOperandosNecesarios == 1 or cantidadOperandosNecesarios == 2:
                operandos = linea[1:]
            for operando in operandos:
                opTipo, opVal = devuelveTipoOperandoYValorDecimal(operando, numLinea)
                valorOperandos.append(opVal)
                tipoDeOperandos.append(opTipo)
            return numLinea, mnemonico, codigoMnemonico, cantidadOperandosNecesarios, valorOperandos, tipoDeOperandos, operandos



def devuelveTipoOperandoYValorDecimal(operando, numLinea):
    # verificamos si el operando es un registro
    # devuelve tipo = 1 y valor del registro
    if operando.upper() in registros:
        return 1, registros[operando.upper()]
    # Si no es un registro, verificamos si es un operador directo
    # Si es devuelve tipo = 2, y valor a reemplazar
    if re.search("\\[", operando):
        ini = operando.index("[")
        fin = operando.index("]")
        return 2, cambioBase(operando[ini+1:fin], numLinea)
    # Sino, es un valor inmediato
    # Devuelve tipo = 0 y el valor a reemplazar
    return 0, cambioBase(operando, numLinea)


def cambioBase(operando, numLinea):
    # Si la primer posicion coincide con lo almacenados en bases
    # @, #, % y '
    if operando[0] in base:
        # obtengo la base
        baseOperando = base[operando[0]]
        # descarto el primer valor
        operandoAux = operando[1:]
        # cuando son ASCII pueden tener una comilla mas -> baseOp != 16 ya que si es hex da true
        if baseOperando != 16 and not operandoAux.isnumeric():
            operandoAux = operandoAux[:-1]
    # sino estan en la lista solo quedan dos opciones
    # opcion 1 --> que sea un valor decimal puro
    elif operando.isnumeric():
        baseOperando = 10
        operandoAux = operando[:]
    # opcion 2 --> que sea una etiqueta
    else:
        baseOperando = 500  # es una etiqueta
    if baseOperando != 500:
        if baseOperando == "ASCII":
            valor = ord(operandoAux)
        else:
            valor = int(operandoAux, baseOperando)
    else:
        if operando.lower() in saltos:
            valor = saltos[operando.lower()]
        else:
            valor = 0xFFF  # no se encuentra el rotulo -> reemplazo por valor
            errores[numLinea] = 1
    return valor


def generaValorCodificado(codMnemonico, cantidadOperandos, tipoOperandos, operandos, numLinea):
    if cantidadOperandos == 2:
        codigo = operacion2Parametros(codMnemonico, operandos[0], operandos[1], tipoOperandos[0], tipoOperandos[1], numLinea)
    elif cantidadOperandos == 1:
        codigo = operacion1Parametro(codMnemonico, operandos[0], tipoOperandos[0], numLinea)
    else:
        codigo = operacion0Parametros(codMnemonico)
    return codigo


# Errores de truncamiento -> print

def operacion2Parametros(codigoOperacion, operando1, operando2, tipoOperando1, tipoOperando2, numLinea):
    if operando1 & 0xFFF != operando1:
        print("Warning... truncado de operando en linea " + str(numLinea + 1) + ".")
    if operando2 & 0xFFF != operando2:
        print("Warning... truncado de operando en linea " + str(numLinea + 1) + ".")
    codigo = np.left_shift(codigoOperacion & 0x00F, 28, dtype=np.int64)
    tipoA = np.left_shift(tipoOperando1, 26, dtype=np.int64)
    a = np.left_shift(operando1 & 0xFFF, 12, dtype=np.int64)
    tipoB = np.left_shift(tipoOperando2 & 0x003, 24, dtype=np.int64)
    b = operando2 & 0xFFF
    codigoFull = codigo | a | b | tipoA | tipoB
    return codigoFull


def operacion1Parametro(codigoOperacion, operando1, tipoOperando1, numLinea):
    if operando1 & 0xFFFF != operando1:
        print("Warning... truncado de operando en linea " + str(numLinea + 1) + ".")
    unos = 15 << 28
    codigo = (codigoOperacion & 0x00F) << 24
    tipoA = tipoOperando1 << 22
    a = operando1 & 0xFFFF
    codigoFull = codigo | a | tipoA | unos
    return codigoFull


def operacion0Parametros(codigoOperacion):
    unos = np.left_shift(0xFF, 24, dtype=np.int64)
    codigo = np.left_shift(codigoOperacion & 0x00F, 20, dtype=np.int64)
    codigoFull = codigo | unos
    return codigoFull


def generoListasDeStrings(codigos, programaFull):
    texto = []
    megaTexto = ""
    for i in range(len(codigos)):
        lineaEnHexa = "["+"{0:d}".format(programaFull[i][0]).zfill(4)+"]"
        codigoEnHexa = f"{(codigos[i] >> 24) & 0xFF:02X} {(codigos[i] >> 16) & 0xFF:02X} {(codigos[i] >> 8) & 0xFF:02X} {codigos[i] & 0xFF:02X} {' '*11}"
        if i in etiqueta.keys():
            lin = etiqueta[i]+":"
        else:
            lin = ""
        lin = '{0: <8}'.format(lin)
        mnemonico = programaFull[i][1] if programaFull[i][1] != None else "---"
        mnemonico = '{0: <8}'.format(mnemonico)
        # genero operadores
        if programaFull[i][3] == 2:
            op1 = programaFull[i][6][0]
            op2 = programaFull[i][6][1]
            op1 = '{0: >6}'.format(op1)
            op2 = '{0: >6}'.format(op2)
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
        lineaDeTexto = lineaEnHexa+" "+codigoEnHexa+" " + lin+" "+mnemonico+" "+ope+" " + " "+coment
        megaTexto += lineaDeTexto+"\n"
        texto.append(lineaDeTexto)
    return texto, megaTexto


