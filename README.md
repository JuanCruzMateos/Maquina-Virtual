# Maquina Virtual - Arquitectura de Computadoras FI UNMdP

## Alumnos:
Sebastian Bengoa  
Noelia Echeverria  
Maria Camila Ezama  
Juan Cruz Mateos  

## Pasos a seguir para ejecutar el codigo.
Necesitan instalar 2 librerias de python (3.6 o superior)
1) numpy
2) re

Necesitan cambiar el path del arhivo que vayan a ejecutar
* Para ejecutar el codigo poner lo siguiente (si el archivo .asm esta en la carpeta del proyecto)
```
python Main.py Fibo.asm Fibo.bin
```
* Para ejecutar el codigo poner lo` siguiente (si el archivo .asm esta en otra carpeta del proyecto)
```
python Main.py c:\\poner el path aca\\Fibo.asm c:\\poner el path aca\\Fibo.bin
```

Para ejecutar MVX:
* Compilar: `gcc -o mvx.exe mvx.c func_mv.c`
* Ejecutar: `mvx.exe fibo.bin [-b] [-c] [-d]`


