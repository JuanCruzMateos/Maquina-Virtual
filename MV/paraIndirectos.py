import numbers

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
    'DATA' : 1024,
    'STACK': 1024,
    'EXTRA': 1024,
    'CODE' : 1024
}

base = {
    "#": 10,
    "@": 8,
    "%": 16,
    "'": "ASCII"
}

# TODO: nuevo
strings = {
    # tag: [string, #mem]
}

# TODO: ahora guarda ctes no string tambien
# rotulos y constantes no string comparten la misma tabla; string se tratan aparte por simplicidad
saltos = {
    'PERRO': 15,
    'GATO': 14
}

etiqueta = {
    # nroLinea: rotulo
}

comentarios = {
    # nroLinea(int): comentario(str)
}

# TODO: nuevos errores -> simbolo duplicado y no se encuentra simbolo
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


# Strings para ir probando
myStr1 = "      [3     +   ax    ]"
myStr2 = "[gato - bx]"
myStr3 = "[perro-gato]"
myStr4 = "[perro]"
myStr5 = "[ax]"
myStr6 = "[ax    -      sp]"
myStr7 = "[bp    -      2]"
myStr8 = "[bp    -      1]"
myStr9 = "[bp+3]"
pruebas = [myStr1, myStr2, myStr3, myStr4, myStr5, myStr6, myStr7, myStr8, myStr9]
pruebas0 = [myStr5, myStr7, myStr8, myStr9]

# uso find en lugar de index porque index tira una excepcion si no encuentra el string

# myStr = myStr6 # cambiar para probar 
# myStr = myStr.replace(" ", "")
# indexs_square_brackets_open = myStr.find("[")
# indexs_square_brackets_close = myStr.find("]")
# index_plus_sign = myStr.find("+")
# index_minus_sign = myStr.find("-")
# print("indexs_square_brackets_open=",indexs_square_brackets_open)
# print("indexs_square_brackets_close=",indexs_square_brackets_close)
# print("index_plus_sign=",index_plus_sign)
# print("index_minus_sign=",index_minus_sign)


# reg = []
# valor = []


# if (indexs_square_brackets_open!=-1 and indexs_square_brackets_close!=-1):
#     myStr = myStr[indexs_square_brackets_open+1:indexs_square_brackets_close]
#     if (index_plus_sign!=-1 or index_minus_sign!=-1):
#         if index_plus_sign!=-1:
#             myStr = myStr.split("+")
#         else:
#             myStr = myStr.split("-")
#     else:
#         aux = []
#         aux.append(myStr)
#         myStr = aux

# if len(myStr)==1:
#     if myStr[0] in etiquetas:
#         valor.append(etiquetas[myStr[0]])
#     elif myStr[0] in registros:
#         reg.append(registros[myStr[0]])
    

# if len(myStr)==2:
    
#     if (myStr[0] in etiquetas) or (myStr[0] in registros):
#         if myStr[0] in etiquetas:
#             valor.append(etiquetas[myStr[0]])
#         else:
#             reg.append(registros[myStr[0]])
#     elif isinstance(myStr[0], numbers.Number):
#             valor.append(myStr[0])

#     if (myStr[1] in etiquetas) or (myStr[1] in registros):
#         if myStr[1] in etiquetas:
#             valor.append(etiquetas[myStr[1]])
#         else:
#             reg.append(registros[myStr[1]])   
#     elif isinstance(myStr[0], numbers.Number):
#             valor.append(myStr[0])
    
# print("myStr = ",myStr)
# print("reg = ",reg)
# print("valor = ",valor)


def valorOperandoIndirecto(operando, numLinea):
    """
    Devuelve el valor del operando indirecto.
    """

    def analizarOp(unOperando: str) -> tuple:
        reg = 0
        offset = 0
        if unOperando.isnumeric():
            offset = int(unOperando)
        elif unOperando.upper() in registros:
            reg = registros[unOperando.upper()]
        elif unOperando.upper() in saltos:
            offset = saltos[unOperando.upper()]
        else:
            errores[numLinea] = 1
        return reg, offset

    operando = operando.replace('[', '').replace(']', '')
    operando = ''.join(operando.split())
    index_plus_sign  = operando.find("+")
    index_minus_sign = operando.find("-")
    if index_minus_sign == -1 and index_plus_sign == -1:
        reg, offset = analizarOp(operando)
    else:
        if index_plus_sign != -1:
            operando = operando.split('+')
        else:
            operando = operando.split('-')
        operando = operando[0].upper(), operando[1].upper()
        print(operando)
        # primer operando
        reg0, offset0 = analizarOp(operando[0])
        # segundo operando
        reg1, offset1 = analizarOp(operando[1])
        # junto
        reg = reg0 + reg1
        if index_minus_sign == -1:
            offset = offset0 + offset1
        else:
            offset = offset0 - offset1
    return (offset << 4) | reg

# TODO nuevo: interpreta valor del operando indirecto
def valorOperandoIndirectoAnt(operando, numLinea):
    """
    Devuelve el valor del operando indirecto.
    """
    # re.split('[\+-]', 'BX+100')
    # print("operando antes de split en valorOperandoIndirecto", operando)
    operando = ''.join(operando.split())
    operando = operando.replace('[','').replace(']','')
    # print("operando antes de split en valorOperandoIndirecto desoues de split" ,operando.upper())
    offset = 0
    reg = 0
    if len(operando) == 2:
        reg = registros[operando.upper()]
        offset = 0
    else:
        etiqueta = False
        positivo = False
        if "+" in operando:
            positivo = True
            operando = operando.split("+")
        elif "-" in operando:
            operando = operando.split("-")
        else:
            etiqueta = True

        if etiqueta:
            return saltos[operando.upper()]
        else:
            if positivo:
                if operando[0].upper() in registros:
                    reg = registros[operando[0].upper()]
                    if operando[1].upper() in saltos:
                        offset = saltos[operando[1].upper()]
                    else:
                        offset = int(operando[1])
                elif operando[0].upper() in saltos:
                    offset = saltos[operando[0].upper()] + int(operando[1])
            else:
                if operando[0].upper() in registros:
                    reg = registros[operando[0].upper()]
                    if operando[1].upper() in saltos:
                        offset = saltos[operando[1].upper()]
                    else:
                        offset = (-1 * int(operando[1]))
                elif operando[0].upper() in saltos:
                    offset = saltos[operando[0].upper()] - int(operando[1])           
    return (offset << 4) | reg
        # reg = registros[operando[:2].upper()]
        # if operando[3:].upper() in saltos:
        #     offset = saltos[operando[3:].upper()]
        #     if operando[2] == '-':
        #         offset *= -1
        # elif operando[3:].isnumeric():
        #     offset = int(operando[2:])
        # else:
        #     errores[numLinea] = 4
        #     return -1
        # print((offset << 4) | reg)

def test(pruebas):
    for op in pruebas:
        print(f"{op} -> {valorOperandoIndirecto(op, 1): 03X}")
        print(f"{op} -> {valorOperandoIndirectoAnt(op, 1): 03X}")
        print()

if __name__ == '__main__':
    test(pruebas0)