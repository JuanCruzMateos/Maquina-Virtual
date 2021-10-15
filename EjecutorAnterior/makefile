# make:
# 	gcc -g -Wall -Wconversion -Wtype-limits -pedantic -Werror -o mvx.exe mvx.c func_mv.c

make:
	gcc -o mvx.exe mvx.c func_mv.c

run:
	./mvx.exe ../ASM/Ejercicio2_b.bin

clean:
	rm ./mvx.exe

valcheck:
	valgrind --leak-check=full --track-origins=yes --show-reachable=yes ./mvx.exe
