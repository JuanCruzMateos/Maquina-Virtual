# Maquina Virtual - Arquitectura de Computadoras FI UNMdP

## Alumnos:
* Sebastian Bengoa  
* Noelia Echeverria  
* Maria Camila Ezama  
* Juan Cruz Mateos  

## Pasos a seguir para ejecutar el codigo.
* Requerimientos:
    - Necesitan instalar numpy 
    - Python (3.6 o superior)

* Traductor:
```
python main.py fibo.asm fibo.bin [-o]
```

* Maquina Virtual - Compilar: 
```
gcc -o mvx.exe mvx.c funcmv.c hastable.c
```
* Maquina Virtual - Ejecutar: 
```
mvx.exe fibo.bin [-b] [-c] [-d]
```