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
	.file	"decode.c"
	.text
	.align	2
	.global	huffman_decode
	.type	huffman_decode, %function
huffman_decode:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 24
	@ frame_needed = 0, uses_anonymous_args = 0
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
	ldr	r1, .L14
	sub	sp, sp, #28
	mov	r4, r0
	ldr	r0, .L14+4
	bl	fopen
	mov	r1, #0
	mov	r2, #2
	str	r0, [sp, #4]
	mov	r0, r4
	bl	fseek
	mov	r0, r4
	bl	ftell
	mov	fp, r0
	mov	r0, r4
	bl	rewind
	add	r0, fp, #1
	bl	malloc
	mov	r3, r4
	mov	r1, #1
	mov	r2, fp
	mov	r9, r0
	bl	fread
	mov	r5, r0
	mov	r0, r4
	bl	ferror
	subs	r4, r0, #0
	bne	.L13
	ldr	r1, .L14+8
	strb	r4, [r9, r5]
	bl	setlocale
	cmp	fp, #0
	ble	.L3
	mov	r8, r4
	ldr	sl, .L14+12
	b	.L8
.L7:
	mov	r3, r0
	ldr	r1, .L14+16
	ldr	r0, [sp, #4]
	mov	r2, #195
	bl	fprintf
	cmp	r8, fp
	bge	.L3
.L8:
	mov	r3, r9
	ldrb	r6, [r3, r8]!	@ zero_extendqisi2
	ldrb	r4, [r3, #3]	@ zero_extendqisi2
	ldrb	r7, [r3, #5]	@ zero_extendqisi2
	ldrb	ip, [r3, #1]	@ zero_extendqisi2
	ldrb	lr, [r3, #2]	@ zero_extendqisi2
	ldrb	r5, [r3, #4]	@ zero_extendqisi2
	mov	r2, #2
	mov	r3, #0
	add	r0, sp, #8
	add	r1, sp, #20
	strb	r4, [sp, #11]
	strb	r3, [sp, #14]
	strb	r6, [sp, #8]
	strb	ip, [sp, #9]
	strb	lr, [sp, #10]
	strb	r5, [sp, #12]
	strb	r7, [sp, #13]
	bl	strtol
	ldr	r3, [sp, #20]
	ldrb	r4, [r3, #0]	@ zero_extendqisi2
	cmp	r4, #0
	mov	r2, r0
	bne	.L11
	ldrb	r0, [sl, r0, asl #1]	@ zero_extendqisi2
	cmp	r0, #38
	movne	r3, r2, asl #1
	addne	r3, sl, r3
	ldrneb	r2, [r3, #1]	@ zero_extendqisi2
	addne	r8, r8, r2
	bne	.L7
	mov	r3, r9
	add	r2, r8, #6
	ldrb	ip, [r3, r2]!	@ zero_extendqisi2
	ldrb	lr, [r3, #1]	@ zero_extendqisi2
	mov	r2, #2
	add	r0, sp, #8
	add	r1, sp, #20
	strb	ip, [sp, #14]
	strb	lr, [sp, #15]
	strb	r4, [sp, #16]
	bl	strtol
	ldr	r3, [sp, #20]
	ldrb	r2, [r3, #0]	@ zero_extendqisi2
	cmp	r2, #0
	bne	.L11
	mov	r3, r0, asl #2
	mov	r2, r0, asl #2
	add	r3, sl, r3
	add	r2, sl, r2
	ldrh	r1, [r3, #130]
	ldrh	r0, [r2, #128]
	add	r8, r8, r1
	mov	r3, r0
	ldr	r1, .L14+16
	ldr	r0, [sp, #4]
	mov	r2, #195
	bl	fprintf
	cmp	r8, fp
	blt	.L8
.L3:
	ldr	r0, [sp, #4]
	bl	fclose
	add	sp, sp, #28
	ldmfd	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
	bx	lr
.L11:
	ldr	r2, .L14+20
	ldr	r0, .L14+24
	ldr	r3, [r2, #0]
	mov	r1, #1
	mov	r2, #52
	bl	fwrite
	mov	r0, #1
	bl	exit
.L13:
	ldr	r2, .L14+20
	ldr	r0, .L14+28
	ldr	r3, [r2, #0]
	mov	r1, #1
	mov	r2, #18
	bl	fwrite
	mov	r0, #1
	bl	exit
.L15:
	.align	2
.L14:
	.word	.LC1
	.word	.LC0
	.word	.LC3
	.word	.LANCHOR0
	.word	.LC5
	.word	stderr
	.word	.LC4
	.word	.LC2
	.size	huffman_decode, .-huffman_decode
	.align	2
	.global	build_lookup_tables
	.type	build_lookup_tables, %function
build_lookup_tables:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 288
	@ frame_needed = 0, uses_anonymous_args = 0
	stmfd	sp!, {r4, r5, r6, r7, r8, r9, sl, lr}
	sub	sp, sp, #288
	add	r5, sp, #8
	sub	r5, r5, #1
	mov	r2, r0
	mov	r1, #256
	mov	r7, r0
	mov	r0, r5
	bl	fgets
	mov	r0, r5
	ldr	r1, .L32
	add	r2, sp, #280
	add	r3, sp, #276
	bl	__isoc99_sscanf
	add	r8, sp, #284
	add	r6, sp, #260
	add	r8, r8, #2
	add	r6, r6, #3
	add	sl, sp, #272
	mov	r9, #1
.L30:
	mov	r0, r5
	mov	r1, #256
	mov	r2, r7
	bl	fgets
	cmp	r0, #0
	beq	.L31
.L23:
	ldr	r1, .L32+4
	mov	r2, r8
	mov	r3, r6
	mov	r0, r5
	bl	__isoc99_sscanf
	mov	r0, r6
	bl	strlen
	and	r4, r0, #255
	cmp	r4, #6
	bhi	.L18
	mov	r1, sl
	mov	r2, #2
	mov	r0, r6
	bl	strtol
	rsb	r2, r4, #6
	mov	r3, r9, asl r2
	and	r0, r0, #255
	mov	r0, r0, asl r2
	sub	r3, r3, #1
	ldrb	lr, [sp, #286]	@ zero_extendqisi2
	ldr	r1, .L32+8
	eor	ip, r3, #63
	and	r0, r0, #255
	mov	r2, #0
.L20:
	eor	r3, r2, r0
	tst	r3, ip
	add	r2, r2, #1
	streqb	lr, [r1, #0]
	streqb	r4, [r1, #1]
	cmp	r2, #64
	add	r1, r1, #2
	bne	.L20
	mov	r0, r5
	mov	r1, #256
	mov	r2, r7
	bl	fgets
	cmp	r0, #0
	bne	.L23
.L31:
	ldr	r2, .L32+12
	add	r0, r0, #38
	add	r1, r2, #128
.L25:
	ldrb	r3, [r2, #0]	@ zero_extendqisi2
	cmp	r3, #0
	streqb	r0, [r2, #-1]
	add	r2, r2, #2
	cmp	r2, r1
	bne	.L25
	add	sp, sp, #288
	ldmfd	sp!, {r4, r5, r6, r7, r8, r9, sl, lr}
	bx	lr
.L18:
	mov	r1, sl
	mov	r2, #2
	mov	r0, r6
	bl	strtol
	mov	r0, r0, asl #16
	rsb	r3, r4, #8
	mov	r0, r0, lsr #16
	mov	r0, r0, asl r3
	mov	r3, r9, asl r3
	add	r2, sp, #256
	mov	r0, r0, asl #16
	sub	r3, r3, #1
	ldr	r1, .L32+16
	ldrh	lr, [r2, #30]
	eor	ip, r3, #255
	mov	r0, r0, lsr #16
	mov	r2, #0
.L22:
	eor	r3, r2, r0
	tst	r3, ip
	add	r2, r2, #1
	streqh	lr, [r1, #0]	@ movhi
	streqh	r4, [r1, #2]	@ movhi
	cmp	r2, #256
	add	r1, r1, #4
	bne	.L22
	b	.L30
.L33:
	.align	2
.L32:
	.word	.LC6
	.word	.LC7
	.word	.LANCHOR0
	.word	.LANCHOR0+1
	.word	.LANCHOR0+128
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
	beq	.L35
	ldr	r2, .L42
	ldr	r0, .L42+4
	ldr	r3, [r2, #0]
	mov	r1, #1
	mov	r2, #70
	bl	fwrite
	mov	r0, #1
.L36:
	ldmfd	sp!, {r4, r5, r6, lr}
	bx	lr
.L35:
	ldr	r0, [r1, #4]
	ldr	r1, .L42+8
	bl	fopen
	subs	r4, r0, #0
	beq	.L40
	bl	build_lookup_tables
	mov	r0, r4
	bl	fclose
	ldr	r0, [r5, #8]
	ldr	r1, .L42+12
	bl	fopen
	subs	r4, r0, #0
	beq	.L41
	bl	huffman_decode
	mov	r0, r4
	bl	fclose
	mov	r0, #0
	b	.L36
.L40:
	ldr	r0, .L42+16
	bl	perror
	mvn	r0, #0
	b	.L36
.L41:
	ldr	r0, .L42+20
	bl	perror
	mvn	r0, #0
	b	.L36
.L43:
	.align	2
.L42:
	.word	stderr
	.word	.LC8
	.word	.LC9
	.word	.LC11
	.word	.LC10
	.word	.LC12
	.size	main, .-main
	.global	LUT
	.global	extended_LUT
	.section	.rodata.str1.4,"aMS",%progbits,1
	.align	2
.LC0:
	.ascii	"original/decoded_output.txt\000"
.LC1:
	.ascii	"w\000"
	.space	2
.LC2:
	.ascii	"Error reading file\000"
	.space	1
.LC3:
	.ascii	"\000"
	.space	3
.LC4:
	.ascii	"Code passed was not null-terminated. Exiting progra"
	.ascii	"m\000"
	.space	3
.LC5:
	.ascii	"%c%c\000"
	.space	3
.LC6:
	.ascii	"%d,%d\000"
	.space	2
.LC7:
	.ascii	"%d,%s\000"
	.space	2
.LC8:
	.ascii	"Argument Error: Expected ./decode input.txt.lut enc"
	.ascii	"oded_input.txt.huf\012\000"
	.space	1
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
