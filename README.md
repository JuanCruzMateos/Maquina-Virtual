# Maquina Virtual - Arquitectura de Computadoras FI UNMdP

## Alumnos:
* Sebastian Bengoa  
* Noelia Echeverria  
* Maria Camila Ezama  
* Juan Cruz Mateos  

## Pasos a seguir para ejecutar el codigo.
* Requerimientos (traductor):
    - Python (3.6 o superior)
    - Numpy 

* Traductor:
```
python mvc.py fibo.asm fibo.bin [-o]
```

* Maquina Virtual - Compilar: 
```
gcc -o mvx.exe mvx.c funcmv.c hashtable.c
```
* Maquina Virtual - Ejecutar: 
```
mvx.exe fibo.bin [-b] [-c] [-d]
```