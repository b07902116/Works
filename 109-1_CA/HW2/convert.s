.global convert
.type matrix_mul, %function

.align 2
# int convert(char *);
convert:
    addi t1, a0, 0		# t1 = &a[0]
    addi t2, zero, '+'	# t2 = '+'
    addi t3, zero, '-'	# t3 = '-'
    addi t4, zero, '0'	# t4 = '0'
    addi t5, zero, '9'	# t5 = '9' + 1
    addi t5, t5, 1		
    addi a0, zero, 0	# a0 = 0

    lbu	a1, 0(t1)		# a1 = a[0]
    addi t1, t1, 1		# t1++
    beq	a1, t2, POS		# a1 == '+'
    beq a1, t3, NEG		# a1 == '-'
    bltu a1, t4, NAN	# a1 < '0'
    bgeu a1, t5, NAN	# a1 > '9'
    addi t1, t1, -1		# t1--
    beq zero, zero, POS
    POS:
    	addi a1, zero, 1	# a1 = 1
    	beq zero, zero, CVT
    NEG:
    	addi a1, zero, -1	# a1 = -1
    	beq zero, zero, CVT
    CVT:
    	lbu	a2, 0(t1)		# a2 = a[i]
    	addi t1, t1, 1		# t1 = &a[i++]
    	beq a2, zero, EXIT	# string end
	    bltu a2, t4, NAN	# a2 < '0'
	    bgeu a2, t5, NAN	# a2 > '9'
	    sub a2, a2, t4		# a2 -= '0'
	    add a0, a0, a2		# a0 += 2
	    lbu a2, 0(t1)		# if next char is '\0', then exit
	    beq a2, zero, EXIT
	    addi a3, zero, 10
	    mul a0, a0, a3		# a0 *= 10
	    beq zero, zero, CVT
    EXIT:
    	mul a0, a0, a1		# modify the sign
    	ret
    NAN:
    	addi a0, zero, -1
    	ret

