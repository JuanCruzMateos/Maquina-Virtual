import re
import numpy as np

# colores para terminal


class Colors:
    RED = '\033[91m'
    YELLOW = '\033[93m'
    UNDERLINE = '\033[4m'
    NOUNDERLINE = '\033[24m'
    RESETCOLOR = '\033[39m'


# mnem : [codigo, nro operandos]
hashmap = {
    'MOV': [0, 2],
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
    'SLEN': [12, 2],
    'SMOV': [13, 2],
    'SCMP': [14, 2],
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
    'PUSH': [252, 1],
    'POP': [253, 1],
    'CALL': [254, 1],
    'RET': [4080, 0],
    'STOP': [4081, 0]
}

# registro : codigo
registros = {
    'DS': 0,
    'SS': 1,
    'ES': 2,
    'CS': 3,
    'HP': 4,
    'IP': 5,
    'SP': 6,
    'BP': 7,
    'CC': 8,
    'AC': 9,
    'AX': 10,
    'BX': 11,
    'CX': 12,
    'DX': 13,
    'EX': 14,
    'FX': 15
}

headers = {
    'DATA': 1024,
    'STACK': 1024,
    'EXTRA': 1024,
    'CODE': 1024
}

base = {
    "#": 10,
    "@": 8,
    "%": 16,
    "'": "ASCII"
}


strings = {
    # tag: [string, #mem]
}


# rotulos y constantes no string comparten la misma tabla; string se tratan aparte por simplicidad
saltos = {
    # rotulo: nroLinea
}

etiqueta = {
    # nroLinea: rotulo
}

comentarios = {
    # nroLinea(int): comentario(str)
}


tipos_errores = [
    "Error sintaxis: Mnemotico desconocido.",
    "No se encuentra rotulo.",
    "Cantidad de operandos erronea.",
    "Simbolo duplicado.",
    "No se encuentra simbolo.",
    "Valor inapropiado en directiva.",
    "Simbolo invalido."
]

errores = {
    # nroLinea: tipo (index tipos_errores)
}


def abrirAsmFile(nombreArchivo: str) -> list:
    """
    Devuelve una lista de str con las lineas del archivo.
    """
    f = open(nombreArchivo, "rt", encoding="utf-8")
    programa = f.read()
    programaEnLineas = programa.split("\n")
    f.close()
    return programaEnLineas


def conviertoLineasEnListas(programaEnLineas: list) -> list:
    """
    Devuelve una lista de listas, cada una con mnem y op en str.
    """
    nroLinea = 0
    programaEnListas = []
    for lineas in programaEnLineas:
        linea = lineas.replace(',', ' ').split()
        # lista vacia [] se evalua como False, la salteo
        if linea and linea[0][0] != ';':
            if linea[0][0] == '\\':
                procesarDirectiva(linea)
            elif len(linea) > 1 and linea[1].upper() == "EQU":
                guardarConstante(linea)
            else:
                linea = buscoRotuloYComentario(linea, nroLinea)
                quitarComas(linea)
                programaEnListas.append(linea)
                nroLinea += 1
    return programaEnListas


def buscoRotuloYComentario(linea: list, nroLinea: int) -> list:
    """
    Elimina comentarios y rotulos agregandolos a sus correspondientes dict.
    """
    # El rotulo solo pueden estar al inicio de una linea
    if re.search(":$", linea[0]) != None:
        rotulo = linea.pop(0)
        rotulo = rotulo.replace(':', '').upper()
        if rotulo in strings or rotulo in saltos:
            errores[nroLinea] = 3
        else:
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


def procesarDirectiva(linea: list):
    """
    Recive una lista con la linea,ej: ['\\ASM', 'DATA=10', 'EXTRA=3000', 'STACK=5000'].
    Analiza y guarda los valores de cada segmento en el diccionario headers.
    """
    linea = linea[1:]
    for directiva in linea:
        segmento, tam = directiva.split("=")
        headers[segmento.upper()] = int(tam)
        if int(tam) < 0 or int(tam) > 65535:
            errores[-2] = 5


def guardarConstante(linea: list):
    """
    Decodifica la constante a guardar ubicandola en saltos (misma tabla que rotulos -> evitar suplicados)
    o en strings.
    En esta etapa los string se guardan como strings, despues deben ser reemplazados por su
    correspondiente valor de mem dentro del CS cuando se termine de procesar el archivo por primera vez.
    -> valorConstStrings()
    """
    # TODO -> quitar comentario de la linea de EQU <-
    const = linea[0].upper()
    valor = ' '.join(linea[2:])
    # si duplicado -> error: no generar imagen
    if const in saltos or const in strings:
        # TODO como indicamos el numero de linea para el error si al ser CONST la salteamos ?
        errores[-1] = 3
    elif valor.isnumeric() or (valor[0] == '-' and valor[1:].isnumeric()):
        saltos[const] = int(valor)
    else:
        # print(valor)
        baseOp = valor[0]
        if valor == '" "':
            strings[const] = [" ", -1]
        elif baseOp == '"' or baseOp[0] == "'":
            valorOp = valor.replace('"', '').replace("'", "")
            # print(valorOp)
            if len(valorOp) < 3:
                errores[-3] = 6  # simbolo invalido
            else:
                strings[const] = [valorOp, -1]
        elif baseOp in base and baseOp != "'":
            valorOp = int(valor[1:], base[baseOp])
            saltos[const] = valorOp
        else:
            errores[-3] = 6


def valorConstStrings(programaEnListas):
    """
    Determina las ubicacion en el DS de cada string.
    Actualiza el CS.
    """
    pos = len(programaEnListas) - 1
    for k, v in strings.items():
        strings[k][1] = pos + 1
        pos = pos + len(v[0]) + 1
    headers["CODE"] = pos + 1


def quitarComas(linea: list) -> list:
    # linea = [linea[i].replace(',', '') if ',' in linea[i] else linea[i] for i in range(len(linea))]
    for i in range(len(linea)):
        if ',' in linea[i]:
            linea[i] = linea[i].replace(',', '')


def generoListaFinal(programaEnListas):
    """
    Devuelve una lista de tuplas con la linea decodificada.
    """
    numLinea = 0
    programaFinal = []
    # reemplazo los string por sus valores adecuados dentro del CS.
    valorConstStrings(programaEnListas)
    for lista in programaEnListas:
        programaFinal.append(decodificoLinea(lista, numLinea))
        numLinea += 1
    return programaFinal


# TODO: a partir de linea 284 verifica si se da el caso de un operando indirecto con espacios irregulares
# i.e.: [  ax +    vector ] -> daria error por cant operandos erronea cuando es correcto; lo salvo
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
        valorOperandos = []
        tipoDeOperandos = []
        operandos = []
        # verificamos que la cantidad de operandos coincide con los encontrados
        if cantidadOperandosEncontrados != cantidadOperandosNecesarios:
            # pueden ser dos comillas separadas por un espacio
            if linea[-1] == "'" and linea[-2] == "'":
                operandos = [linea[1], "' '"]
            elif linea[-3] == "'" and linea[-2] == "'":
                operandos = ["' '", linea[4]]
            else:
                # mejorar -> espacios variables en indirectos
                if "[" in linea and "]" in linea:
                    ini = linea.index("[")
                    fin = linea.index("]")
                    aux = ''.join(linea[ini:fin+1])
                    if ini == 1:
                        operandos = [aux, linea[-1]]
                    elif ini == 2:
                        operandos = [linea[1], aux]
                    else:
                        errores[numLinea] = 2
                        return numLinea, None, None, None, None, None, None, None
                else:
                    errores[numLinea] = 2
                    return numLinea, None, None, None, None, None, None, None
        else:
            operandos = linea[1:]
        for operando in operandos:
            opTipo, opVal = devuelveTipoOperandoYValorDecimal(operando, numLinea)
            valorOperandos.append(opVal)
            tipoDeOperandos.append(opTipo)
        return numLinea, mnemonico, codigoMnemonico, cantidadOperandosNecesarios, valorOperandos, tipoDeOperandos, operandos


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


def devuelveTipoOperandoYValorDecimal(operando: str, numLinea: int):
    # verificamos si el operando es un registro
    # devuelve tipo = 1 y valor del registro
    if operando.upper() in registros:
        return 1, registros[operando.upper()]
    # Si no es un registro, verificamos si es un operador directo o indirecto
    # Si es devuelve tipo = 2, y valor a reemplazar
    if re.search("\\[", operando):
        ini = operando.index("[")
        fin = operando.index("]")
        # nuevo
        aux = operando.replace('[', '').replace(']', '').upper()
        if aux in saltos:
            return 2, saltos[aux]
        # nuevo
        elif re.search("(?i)[A-H]|[X]|[B]|[P]|[S]", operando) == None or re.search("[%#@]", operando) != None:
            return 2, cambioBase(operando[ini+1:fin], numLinea)
        else:
            return 3, valorOperandoIndirecto(operando, numLinea)
    # Sino, es un valor inmediato
    # Devuelve tipo = 0 y el valor a reemplazar
    return 0, cambioBase(operando, numLinea)


def valorOperandoIndirecto(operando, numLinea):
    """
    Devuelve el valor del operando indirecto.
    """

    def analizarOp(unOperando: str, numlinea: int) -> tuple:
        reg = 0
        offset = 0
        if unOperando.isnumeric():
            offset = int(unOperando)
        elif unOperando.upper() in registros:
            reg = registros[unOperando.upper()]
        elif unOperando.upper() in saltos:
            offset = saltos[unOperando.upper()]
        else:
            errores[numlinea] = 1
        return reg, offset

    operando = operando.replace('[', '').replace(']', '')
    operando = ''.join(operando.split())
    index_plus_sign = operando.find("+")
    index_minus_sign = operando.find("-")
    if index_minus_sign == -1 and index_plus_sign == -1:
        reg, offset = analizarOp(operando, numLinea)
    else:
        if index_plus_sign != -1:
            operando = operando.split('+')
        else:
            operando = operando.split('-')
        operando = operando[0].upper(), operando[1].upper()
        # primer operando
        reg0, offset0 = analizarOp(operando[0], numLinea)
        # segundo operando
        reg1, offset1 = analizarOp(operando[1], numLinea)
        # junto
        reg = reg0 + reg1
        if index_minus_sign == -1:
            offset = offset0 + offset1
        else:
            offset = offset0 - offset1
    return (offset << 4) | reg


def cambioBase(operando, numLinea):
    # Si la primer posicion coincide con lo almacenados en bases
    # @, #, % y '
    if operando[0] in base:
        # obtengo la base
        baseOperando = base[operando[0]]
        # descarto el primer valor que indica la base
        operandoAux = operando[1:]
        # cuando son ASCII pueden tener una comilla mas, o ser solo una comilla '
        if baseOperando == "ASCII":
            if len(operandoAux) == 0:
                operandoAux = ' '
            else:
                operandoAux = operandoAux[:-1]
    # sino estan en la lista solo quedan dos opciones
    # opcion 1 --> que sea un valor decimal puro
    elif operando.isnumeric() or (operando[0] == '-' and operando[1:].isnumeric()):
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
        # puede ser un rotulo o una constrante
        if operando.upper() in saltos:
            valor = saltos[operando.upper()]
        # puede ser un string
        elif operando.upper() in strings:
            valor = strings[operando.upper()]
        else:
            valor = 0xFFF  # no se encuentra el rotulo -> reemplazo por valor
            errores[numLinea] = 1
    return valor


def generaValorCodificado(codMnemonico, cantidadOperandos, tipoOperandos, operandos, numLinea):
    if cantidadOperandos == 2:
        codigo = operacion2Parametros(
            codMnemonico, operandos[0], operandos[1], tipoOperandos[0], tipoOperandos[1], numLinea)
    elif cantidadOperandos == 1:
        codigo = operacion1Parametro(
            codMnemonico, operandos[0], tipoOperandos[0], numLinea)
    else:
        codigo = operacion0Parametros(codMnemonico)
    return codigo


def operacion2Parametros(codigoOperacion, operando1, operando2, tipoOperando1, tipoOperando2, numLinea):
    if type(operando2) == list:
        operando2 = operando2[1]
    if type(operando1) == list:
        operando1 = operando1[1]
    if operando1 & 0xFFF != operando1:
        print(Colors.YELLOW + Colors.UNDERLINE + "Warning" + Colors.NOUNDERLINE +
              ": truncado de operando en linea " + str(numLinea) + "." + Colors.RESETCOLOR)
    if operando2 & 0xFFF != operando2:
        print(Colors.YELLOW + Colors.UNDERLINE + "Warning" + Colors.NOUNDERLINE +
              ": truncado de operando en linea " + str(numLinea) + "." + Colors.RESETCOLOR)
    codigo = np.left_shift(codigoOperacion & 0x00F, 28, dtype=np.int64)
    tipoA = np.left_shift(tipoOperando1, 26, dtype=np.int64)
    a = np.left_shift(operando1 & 0xFFF, 12, dtype=np.int64)
    tipoB = np.left_shift(tipoOperando2 & 0x003, 24, dtype=np.int64)
    b = operando2 & 0xFFF
    codigoFull = codigo | tipoA | tipoB | a | b
    return codigoFull


def operacion1Parametro(codigoOperacion, operando1, tipoOperando1, numLinea):
    if type(operando1) == list:
        operando1 = operando1[1]
    if operando1 & 0xFFFF != operando1:
        print(Colors.YELLOW + Colors.UNDERLINE + "Warning:" + Colors.NOUNDERLINE +
              " truncado de operando en linea " + str(numLinea) + "." + Colors.RESETCOLOR)
    unos = 15 << 28
    codigo = (codigoOperacion & 0x00F) << 24
    tipoA = tipoOperando1 << 22
    if tipoOperando1 == 3:
        a = operando1 & 0x0FFF
    else:
        a = operando1 & 0xFFFF
    codigoFull = unos | codigo | tipoA | a
    return codigoFull


def operacion0Parametros(codigoOperacion):
    unos = np.left_shift(0xFF, 24, dtype=np.int64)
    codigo = np.left_shift(codigoOperacion & 0x00F, 20, dtype=np.int64)
    codigoFull = codigo | unos
    return codigoFull


# en una lina hay un rotulo duplicado muestro un -Err-
def generoListasDeStrings(codigos, programaFull):
    texto = []
    megaTexto = ""
    for i in range(len(codigos)):
        lineaEnHexa = "["+"{0:d}".format(programaFull[i][0]).zfill(4)+"]"
        codigoEnHexa = f"{(codigos[i] >> 24) & 0xFF:02X} {(codigos[i] >> 16) & 0xFF:02X} {(codigos[i] >> 8) & 0xFF:02X} {codigos[i] & 0xFF:02X} {' '*11}"
        if i in etiqueta.keys():
            lin = etiqueta[i]+":"
        elif i in errores and errores[i] == 3:
            lin = "-Err-"
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
            op2 = '{0: >8}'.format(op2)
            ope = op1 + ", " + op2
        elif programaFull[i][3] == 1:
            ope = '{0: >6}'.format(programaFull[i][6][0])
        else:
            ope = ""
        ope = '{0: <15}'.format(ope)
        if i in comentarios.keys():
            coment = ";" + comentarios[i]
        else:
            coment = ""
        lineaDeTexto = lineaEnHexa+" "+codigoEnHexa+" " + \
            lin+" "+mnemonico+" "+ope+" " + " " * 6 + coment
        megaTexto += lineaDeTexto + "\n"
        texto.append(lineaDeTexto)
    return texto, megaTexto


def agregarInfoHeaders(arr: list):
    """
    Agregar headers al arreglo antes de convertilo en numpy arr.
    """
    arr.insert(0, 0x4D563231)
    arr.insert(1, headers["DATA"])
    arr.insert(2, headers["STACK"])
    arr.insert(3, headers["EXTRA"])
    arr.insert(4, headers["CODE"])


def agregarStringsDS(arr: list):
    """
    Agregar strings al CS, un char por celda + '\0'.
    """
    lista_str = list(strings.values())
    lista_str.sort(key=lambda x: x[1])
    for string, _ in lista_str:
        for char in string:
            arr.append(ord(char))
        arr.append(0)
