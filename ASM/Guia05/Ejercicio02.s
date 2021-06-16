	.file	"Ejercicio02.c"
 # GNU C17 (x86_64-posix-seh-rev0, Built by MinGW-W64 project) version 8.1.0 (x86_64-w64-mingw32)
 #	compiled by GNU C version 8.1.0, GMP version 6.1.2, MPFR version 4.0.1, MPC version 1.1.0, isl version isl-0.18-GMP

 # GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
 # options passed: 
 # -iprefix C:/Program Files/CodeBlocks/MinGW/bin/../lib/gcc/x86_64-w64-mingw32/8.1.0/
 # -D_REENTRANT Ejercicio02.c -mtune=core2 -march=nocona -Wall
 # -fverbose-asm
 # options enabled:  -faggressive-loop-optimizations
 # -fasynchronous-unwind-tables -fauto-inc-dec -fchkp-check-incomplete-type
 # -fchkp-check-read -fchkp-check-write -fchkp-instrument-calls
 # -fchkp-narrow-bounds -fchkp-optimize -fchkp-store-bounds
 # -fchkp-use-static-bounds -fchkp-use-static-const-bounds
 # -fchkp-use-wrappers -fcommon -fdelete-null-pointer-checks
 # -fdwarf2-cfi-asm -fearly-inlining -feliminate-unused-debug-types
 # -ffp-int-builtin-inexact -ffunction-cse -fgcse-lm -fgnu-runtime
 # -fgnu-unique -fident -finline-atomics -fira-hoist-pressure
 # -fira-share-save-slots -fira-share-spill-slots -fivopts
 # -fkeep-inline-dllexport -fkeep-static-consts -fleading-underscore
 # -flifetime-dse -flto-odr-type-merging -fmath-errno -fmerge-debug-strings
 # -fpeephole -fpic -fplt -fprefetch-loop-arrays -freg-struct-return
 # -fsched-critical-path-heuristic -fsched-dep-count-heuristic
 # -fsched-group-heuristic -fsched-interblock -fsched-last-insn-heuristic
 # -fsched-rank-heuristic -fsched-spec -fsched-spec-insn-heuristic
 # -fsched-stalled-insns-dep -fschedule-fusion -fsemantic-interposition
 # -fset-stack-executable -fshow-column -fshrink-wrap-separate
 # -fsigned-zeros -fsplit-ivs-in-unroller -fssa-backprop -fstdarg-opt
 # -fstrict-volatile-bitfields -fsync-libcalls -ftrapping-math
 # -ftree-cselim -ftree-forwprop -ftree-loop-if-convert -ftree-loop-im
 # -ftree-loop-ivcanon -ftree-loop-optimize -ftree-parallelize-loops=
 # -ftree-phiprop -ftree-reassoc -ftree-scev-cprop -funit-at-a-time
 # -funwind-tables -fverbose-asm -fzero-initialized-in-bss
 # -m128bit-long-double -m64 -m80387 -maccumulate-outgoing-args
 # -malign-double -malign-stringops -mcx16 -mfancy-math-387 -mfentry
 # -mfp-ret-in-387 -mfxsr -mieee-fp -mlong-double-80 -mmmx -mms-bitfields
 # -mno-sse4 -mpush-args -mred-zone -msse -msse2 -msse3 -mstack-arg-probe
 # -mstackrealign -mvzeroupper

	.text
	.globl	calculo
	.def	calculo;	.scl	2;	.type	32;	.endef
	.seh_proc	calculo
calculo:
	pushq	%rbp	 #
	.seh_pushreg	%rbp
	movq	%rsp, %rbp	 #,
	.seh_setframe	%rbp, 0
	.seh_endprologue
	movl	%ecx, 16(%rbp)	 # x, x
	movq	%rdx, 24(%rbp)	 # vec, vec
 # Ejercicio02.c:4:   vec[x] = vec[x-1] & vec[0];
	movl	16(%rbp), %eax	 # x, tmp97
	cltq
	salq	$2, %rax	 #, _2
	leaq	-4(%rax), %rdx	 #, _3
	movq	24(%rbp), %rax	 # vec, tmp98
	addq	%rdx, %rax	 # _3, _4
	movl	(%rax), %ecx	 # *_4, _5
 # Ejercicio02.c:4:   vec[x] = vec[x-1] & vec[0];
	movq	24(%rbp), %rax	 # vec, tmp99
	movl	(%rax), %edx	 # *vec_12(D), _6
 # Ejercicio02.c:4:   vec[x] = vec[x-1] & vec[0];
	movl	16(%rbp), %eax	 # x, tmp100
	cltq
	leaq	0(,%rax,4), %r8	 #, _8
	movq	24(%rbp), %rax	 # vec, tmp101
	addq	%r8, %rax	 # _8, _9
 # Ejercicio02.c:4:   vec[x] = vec[x-1] & vec[0];
	andl	%ecx, %edx	 # _5, _10
 # Ejercicio02.c:4:   vec[x] = vec[x-1] & vec[0];
	movl	%edx, (%rax)	 # _10, *_9
 # Ejercicio02.c:5: }
	nop	
	popq	%rbp	 #
	ret	
	.seh_endproc
	.def	__main;	.scl	2;	.type	32;	.endef
	.globl	main
	.def	main;	.scl	2;	.type	32;	.endef
	.seh_proc	main
main:
	pushq	%rbp	 #
	.seh_pushreg	%rbp
	movq	%rsp, %rbp	 #,
	.seh_setframe	%rbp, 0
	subq	$64, %rsp	 #,
	.seh_stackalloc	64
	.seh_endprologue
 # Ejercicio02.c:8: void main() {
	call	__main	 #
 # Ejercicio02.c:9:   int j=3, arreglo[5];
	movl	$3, -4(%rbp)	 #, j
 # Ejercicio02.c:10:   arreglo[0] = 1020;
	movl	$1020, -32(%rbp)	 #, arreglo
 # Ejercicio02.c:11:   arreglo[1] = arreglo[0] | 0x3FF;
	movl	-32(%rbp), %eax	 # arreglo, _1
 # Ejercicio02.c:11:   arreglo[1] = arreglo[0] | 0x3FF;
	orl	$1023, %eax	 #, _2
 # Ejercicio02.c:11:   arreglo[1] = arreglo[0] | 0x3FF;
	movl	%eax, -28(%rbp)	 # _2, arreglo
 # Ejercicio02.c:12:   calculo( j, arreglo);
	leaq	-32(%rbp), %rax	 #, tmp89
	movl	-4(%rbp), %ecx	 # j, tmp90
	movq	%rax, %rdx	 # tmp89,
	call	calculo	 #
 # Ejercicio02.c:13: }
	nop	
	addq	$64, %rsp	 #,
	popq	%rbp	 #
	ret	
	.seh_endproc
	.ident	"GCC: (x86_64-posix-seh-rev0, Built by MinGW-W64 project) 8.1.0"
