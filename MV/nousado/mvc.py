import sys
from pprint import pprint


mnem_dos_op       = ["MOV", "ADD", "SUB", "SWAP", "MUL", "DIV", "CMP", "SHL", "SHR", "AND", "OR", "XOR"]
mnem_dos_op_code  = [i for i in range(12)]
mnem_un_op        = ["SYS", "JMP", "JZ", "JP", "JN", "JNZ", "JNP", "JNN", "LDL", "LDH", "RND", "NOT"]
mnem_un_op_code   = [240 + i for i in range(12)]
mnem_cero_op      = ["STOP"]
mnem_cero_op_code = [4081]


# mnemonico  = ["MOV", "ADD", "SUB", "SWAP", "MUL", "DIV", "CMP", "SHL", "SHR", "AND", "OR", "XOR", "SYS", "JMP", "JZ", "JP", "JN", "JNZ", "JNP", "JNN", "LDL", "LDH", "RND", "NOT", "STOP"]
# cod_mnemonico = [i for i in range(12)] + [240 + i for i in range(12)] + [4081]


def tokens(fname: str) -> list:
    """Devuelve una lista con listas de strings por cada linea del archivo"""
    with open(fname, "rt") as asmfile:
        # como seria usando compresion?
        # operaciones = [linea.strip().split() for linea in asmfile]
        inst = []
        for linea in asmfile:
            instruccion= linea.replace(',', ' , ').strip().split(';', 1)
            if len(instruccion) == 2:
                comment = instruccion[1]
            else:
                comment = " "
            instruccion = instruccion[0]
            print(instruccion, comment)
        return inst


def main():
    asmfile_name = sys.argv[1]
    instrucciones = tokens(asmfile_name)
    pprint(instrucciones)
    # obtener los labels
    # labels = {}
    # for i, instruccion in enumerate(instrucciones):
    #     inst = instruccion[0].upper()
    #     if inst not in mnem_cero_op + mnem_dos_op + mnem_un_op:
    #         labels[inst] = i
    # print(labels)
    # for instruccion in instrucciones:
    #     print(instruccion)


if __name__ == "__main__":
    main()
