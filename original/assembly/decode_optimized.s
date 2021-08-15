	.arch armv4t
	.eabi_attribute 27, 3
	.fpu neon
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 2
	.eabi_attribute 30, 2
	.eabi_attribute 18, 4
	.file	"decode_optimized.c"
	.text
	.align	2
	.global	huffman_decode
	.type	huffman_decode, %function
huffman_decode:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 0, uses_anonymous_args = 0
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
	ldr	r1, .L17
	sub	sp, sp, #20
	mov	r4, r0
	mov	r0, #0
	bl	setlocale
	ldr	r1, .L17+4
	ldr	r0, .L17+8
	bl	fopen
	mov	r1, #0
	mov	r2, #2
	mov	fp, r0
	mov	r0, r4
	bl	fseek
	mov	r0, r4
	bl	ftell
	mov	r0, r0, asl #16
	mov	r9, r0, lsr #16
	mov	r0, r4
	bl	rewind
	add	r0, r9, #1
	bl	malloc
	mov	r1, #1
	mov	r2, r9
	mov	r3, r4
	mov	r8, r0
	bl	fread
	mov	r5, r0
	mov	r0, r4
	bl	ferror
	cmp	r0, #0
	bne	.L16
	cmp	r9, #0
	strb	r0, [r8, r5]
	beq	.L3
	mov	r7, r0
	ldr	sl, .L17+12
	b	.L8
.L7:
	mov	r3, r0
	ldr	r1, .L17+16
	mov	r0, fp
	mov	r2, #195
	bl	fprintf
	cmp	r7, r9
	bcs	.L3
.L8:
	mov	r3, r8
	mov	r2, r8
	add	r0, r7, #2
	add	ip, r7, #4
	ldrb	r5, [r3, r0]!	@ zero_extendqisi2
	ldrb	r6, [r2, ip]!	@ zero_extendqisi2
	mov	r1, r8
	ldrb	lr, [r1, r7]!	@ zero_extendqisi2
	ldrb	r4, [r3, #1]	@ zero_extendqisi2
	ldrb	r3, [r2, #1]	@ zero_extendqisi2
	ldrb	ip, [r1, #1]	@ zero_extendqisi2
	mov	r0, sp
	strb	r3, [sp, #5]
	add	r1, sp, #12
	mov	r3, #0
	mov	r2, #2
	strb	r4, [sp, #3]
	strb	r3, [sp, #6]
	strb	lr, [sp, #0]
	strb	ip, [sp, #1]
	strb	r5, [sp, #2]
	strb	r6, [sp, #4]
	bl	strtol
	ldr	r3, [sp, #12]
	ldrb	r4, [r3, #0]	@ zero_extendqisi2
	cmp	r4, #0
	bne	.L9
	mov	r3, r0, asl #16
	mov	r3, r3, lsr #16
	ldrb	r0, [sl, r3, asl #1]	@ zero_extendqisi2
	cmp	r0, #38
	movne	r3, r3, asl #1
	addne	r3, sl, r3
	ldrneb	r2, [r3, #1]	@ zero_extendqisi2
	addne	r7, r7, r2
	bne	.L7
	mov	r3, r8
	add	r2, r7, #6
	ldrb	ip, [r3, r2]!	@ zero_extendqisi2
	ldrb	lr, [r3, #1]	@ zero_extendqisi2
	mov	r2, #2
	mov	r0, sp
	add	r1, sp, #12
	strb	ip, [sp, #6]
	strb	lr, [sp, #7]
	strb	r4, [sp, #8]
	bl	strtol
	ldr	r3, [sp, #12]
	ldrb	r2, [r3, #0]	@ zero_extendqisi2
	cmp	r2, #0
	bne	.L9
	mov	r3, r0, asl #16
	add	r2, sl, r3, lsr #14
	add	r3, sl, r3, lsr #14
	ldrh	r1, [r2, #130]
	ldrh	r0, [r3, #128]
	add	r7, r7, r1
	mov	r3, r0
	ldr	r1, .L17+16
	mov	r0, fp
	mov	r2, #195
	bl	fprintf
	cmp	r7, r9
	bcc	.L8
.L3:
	mov	r0, fp
	bl	fclose
	mov	r0, r8
	bl	free
	add	sp, sp, #20
	ldmfd	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
	bx	lr
.L9:
	ldr	r2, .L17+20
	ldr	r0, .L17+24
	ldr	r3, [r2, #0]
	mov	r1, #1
	mov	r2, #52
	bl	fwrite
	mov	r0, #1
	bl	exit
.L16:
	ldr	r2, .L17+20
	ldr	r0, .L17+28
	ldr	r3, [r2, #0]
	mov	r1, #1
	mov	r2, #29
	bl	fwrite
	mov	r0, #1
	bl	exit
.L18:
	.align	2
.L17:
	.word	.LC0
	.word	.LC2
	.word	.LC1
	.word	.LANCHOR0
	.word	.LC5
	.word	stderr
	.word	.LC4
	.word	.LC3
	.size	huffman_decode, .-huffman_decode
	.align	2
	.global	build_lookup_tables
	.type	build_lookup_tables, %function
build_lookup_tables:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 280
	@ frame_needed = 0, uses_anonymous_args = 0
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
	sub	sp, sp, #284
	add	r6, sp, #4
	sub	r6, r6, #1
	mov	r2, r0
	mov	r1, #256
	mov	r8, r0
	mov	r0, r6
	bl	fgets
	add	r2, sp, #276
	mov	r0, r6
	ldr	r1, .L38
	add	r2, r2, #2
	add	r3, sp, #276
	bl	sscanf
	add	sl, sp, #272
	add	r7, sp, #256
	add	sl, sl, #2
	add	r7, r7, #3
	add	r9, sp, #268
	mov	fp, #1
.L36:
	mov	r0, r6
	mov	r1, #256
	mov	r2, r8
	bl	fgets
	cmp	r0, #0
	beq	.L37
.L28:
	ldr	r1, .L38+4
	mov	r2, sl
	mov	r3, r7
	mov	r0, r6
	bl	sscanf
	mov	r0, r7
	bl	strlen
	and	r4, r0, #255
	cmp	r4, #6
	bhi	.L21
	mov	r1, r9
	mov	r2, #2
	mov	r0, r7
	bl	strtol
	rsb	r2, r4, #6
	and	r0, r0, #255
	mov	r3, fp, asl r2
	mov	r0, r0, asl r2
	sub	r3, r3, #1
	ldrb	r5, [sp, #274]	@ zero_extendqisi2
	ldr	ip, .L38+8
	eor	lr, r3, #63
	and	r0, r0, #255
	mov	r1, #0
.L24:
	add	r3, r1, #1
	eor	r2, r1, r0
	tst	r2, lr
	eor	r3, r0, r3
	streqb	r5, [ip, #-2]
	streqb	r4, [ip, #-1]
	add	r1, r1, #2
	tst	r3, lr
	streqb	r5, [ip, #0]
	streqb	r4, [ip, #1]
	cmp	r1, #64
	add	ip, ip, #4
	bne	.L24
	mov	r0, r6
	mov	r1, #256
	mov	r2, r8
	bl	fgets
	cmp	r0, #0
	bne	.L28
.L37:
	ldr	r2, .L38+12
	add	r0, r0, #38
	add	r1, r2, #128
.L31:
	ldrb	r3, [r2, #0]	@ zero_extendqisi2
	cmp	r3, #0
	ldrb	r3, [r2, #2]	@ zero_extendqisi2
	streqb	r0, [r2, #-1]
	cmp	r3, #0
	streqb	r0, [r2, #1]
	add	r2, r2, #4
	cmp	r2, r1
	bne	.L31
	add	sp, sp, #284
	ldmfd	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
	bx	lr
.L21:
	mov	r1, r9
	mov	r2, #2
	mov	r0, r7
	bl	strtol
	mov	r0, r0, asl #16
	rsb	r3, r4, #8
	mov	r0, r0, lsr #16
	mov	r0, r0, asl r3
	mov	r3, fp, asl r3
	add	r2, sp, #256
	mov	r0, r0, asl #16
	sub	r3, r3, #1
	ldrh	r5, [r2, #18]
	ldr	ip, .L38+16
	eor	lr, r3, #255
	mov	r0, r0, lsr #16
	mov	r1, #0
.L27:
	add	r3, r1, #1
	eor	r2, r1, r0
	tst	r2, lr
	eor	r3, r0, r3
	streqh	r5, [ip, #-4]	@ movhi
	streqh	r4, [ip, #-2]	@ movhi
	add	r1, r1, #2
	tst	r3, lr
	streqh	r5, [ip, #0]	@ movhi
	streqh	r4, [ip, #2]	@ movhi
	cmp	r1, #256
	add	ip, ip, #8
	bne	.L27
	b	.L36
.L39:
	.align	2
.L38:
	.word	.LC6
	.word	.LC7
	.word	.LANCHOR0+2
	.word	.LANCHOR0+1
	.word	.LANCHOR0+132
	.size	build_lookup_tables, .-build_lookup_tables
	.align	2
	.global	main
	.type	main, %function
main:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	cmp	r0, #3
	stmfd	sp!, {r4, r5, r6, lr}
	mov	r5, r1
	bne	.L46
	ldr	r0, [r1, #4]
	ldr	r1, .L49
	bl	fopen
	subs	r4, r0, #0
	beq	.L47
	bl	build_lookup_tables
	mov	r0, r4
	bl	fclose
	ldr	r0, [r5, #8]
	ldr	r1, .L49+4
	bl	fopen
	subs	r4, r0, #0
	beq	.L48
	bl	huffman_decode
	mov	r0, r4
	bl	fclose
	mov	r0, #0
.L42:
	ldmfd	sp!, {r4, r5, r6, lr}
	bx	lr
.L46:
	ldr	r2, .L49+8
	ldr	r0, .L49+12
	ldr	r3, [r2, #0]
	mov	r1, #1
	mov	r2, #60
	bl	fwrite
	mvn	r0, #0
	b	.L42
.L47:
	ldr	r0, .L49+16
	bl	perror
	mvn	r0, #0
	b	.L42
.L48:
	ldr	r0, .L49+20
	bl	perror
	mvn	r0, #0
	b	.L42
.L50:
	.align	2
.L49:
	.word	.LC9
	.word	.LC11
	.word	stderr
	.word	.LC8
	.word	.LC10
	.word	.LC12
	.size	main, .-main
	.global	LUT
	.global	extended_LUT
	.section	.rodata.str1.4,"aMS",%progbits,1
	.align	2
.LC0:
	.ascii	"\000"
	.space	3
.LC1:
	.ascii	"./decoded_output.txt\000"
	.space	3
.LC2:
	.ascii	"w\000"
	.space	2
.LC3:
	.ascii	"Error reading the input file.\000"
	.space	2
.LC4:
	.ascii	"Code passed was not null-terminated. Exiting progra"
	.ascii	"m\000"
	.space	3
.LC5:
	.ascii	"%c%c\000"
	.space	3
.LC6:
	.ascii	"%hu,%hu\000"
.LC7:
	.ascii	"%hu,%s\000"
	.space	1
.LC8:
	.ascii	"Argument Error: Expected ./decode LUT.txt encoded_i"
	.ascii	"nput.txt\012\000"
	.space	3
.LC9:
	.ascii	"r\000"
	.space	2
.LC10:
	.ascii	"Error opening LUT.txt file\000"
	.space	1
.LC11:
	.ascii	"rb\000"
	.space	1
.LC12:
	.ascii	"Error opening encoded_input.txt file\000"
	.bss
	.align	1
.LANCHOR0 = . + 0
	.type	LUT, %object
	.size	LUT, 128
LUT:
	.space	128
	.type	extended_LUT, %object
	.size	extended_LUT, 1024
extended_LUT:
	.space	1024
	.ident	"GCC: (Sourcery G++ Lite 2008q3-72) 4.3.2"
	.section	.note.GNU-stack,"",%progbits
