import numbers

registros = {
    'ax': 0x00010002,
    'bx': 0x00020001,
    'sp': 0x00030004
}

etiquetas = {
    'perro': 15,
    'gato': 14
}

# Strings para ir probando
myStr1 = "      [3     +   ax    ]"
myStr2 = "[gato - bx]"
myStr3 = "[perro-gato]"
myStr4 = "[perro]"
myStr5 = "[ax]"
myStr6 = "[ax    -      sp]"
# uso find en lugar de index porque index tira una excepcion si no encuentra el string

myStr = myStr6 # cambiar para probar 
myStr = myStr.replace(" ", "")
indexs_square_brackets_open = myStr.find("[")
indexs_square_brackets_close = myStr.find("]")
index_plus_sign = myStr.find("+")
index_minus_sign = myStr.find("-")
print("indexs_square_brackets_open=",indexs_square_brackets_open)
print("indexs_square_brackets_close=",indexs_square_brackets_close)
print("index_plus_sign=",index_plus_sign)
print("index_minus_sign=",index_minus_sign)


reg = []
valor = []


if (indexs_square_brackets_open!=-1 and indexs_square_brackets_close!=-1):
    myStr = myStr[indexs_square_brackets_open+1:indexs_square_brackets_close]
    if (index_plus_sign!=-1 or index_minus_sign!=-1):
        if index_plus_sign!=-1:
            myStr = myStr.split("+")
        else:
            myStr = myStr.split("-")
    else:
        aux = []
        aux.append(myStr)
        myStr = aux

if len(myStr)==1:
    if myStr[0] in etiquetas:
        valor.append(etiquetas[myStr[0]])
    elif myStr[0] in registros:
        reg.append(registros[myStr[0]])
    

if len(myStr)==2:
    
    if (myStr[0] in etiquetas) or (myStr[0] in registros):
        if myStr[0] in etiquetas:
            valor.append(etiquetas[myStr[0]])
        else:
            reg.append(registros[myStr[0]])
    elif isinstance(myStr[0], numbers.Number):
            valor.append(myStr[0])

    if (myStr[1] in etiquetas) or (myStr[1] in registros):
        if myStr[1] in etiquetas:
            valor.append(etiquetas[myStr[1]])
        else:
            reg.append(registros[myStr[1]])   
    elif isinstance(myStr[0], numbers.Number):
            valor.append(myStr[0])
    
print("myStr = ",myStr)
print("reg = ",reg)
print("valor = ",valor)