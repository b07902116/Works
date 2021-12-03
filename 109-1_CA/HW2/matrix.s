.global matrix_mul
.type matrix_mul, %function

.align 2
# void matrix_mul(unsigned int A[][], unsigned int B[][], unsinged int C[][]);
matrix_mul:
    
    # insert code here
    # Green card here: https://www.cl.cam.ac.uk/teaching/1617/ECAD+Arch/files/docs/RISCVGreenCardv8-20151013.pdf
    # Matrix multiplication: https://en.wikipedia.org/wiki/Matrix_multiplication
    addi sp, sp, -88
    sd	s1, 0(sp)
    sd	s2, 8(sp)
    sd 	s3, 16(sp)
    sd	s4, 24(sp)
    sd	s5, 32(sp)
    sd 	s6, 40(sp)
    sd	s7, 48(sp)
    sd	s8, 56(sp)
    sd 	s9, 64(sp)
    sd	s10, 72(sp)
    sd	s11, 80(sp)
    addi a3, zero, 0	# a3 = i = 0
    addi a4, zero, 0	# a4 = j = 0
    addi a5, zero, 0	# a5 = k = 0
    addi a6, zero, 128	# a6 = SIZE = 128
    LOOPI:
    	addi a5, zero, 0	# k = 0
    	LOOPK:
	    	slli t5, a3, 7	# t5 = i * 128
	    	add t5, t5, a5	# t5 = t5 + k
	    	slli t5, t5, 1	# t5 *= 2
	    	add t5, a0, t5
	    	lhu	s1, 0(t5)
	    	lhu	s2, 2(t5)
	    	lhu	s3, 4(t5)
	    	lhu	s4, 6(t5)
	    	lhu	s5, 8(t5)
	    	lhu	s6, 10(t5)
	    	lhu	s7, 12(t5)
	    	lhu	s8, 14(t5)
    		addi a4, zero, 0	# j = 0
    		LOOPJ:
    			slli t5, a5, 7	# t5 = k * 128
    			add t5, t5, a4	# t5 += j
    			slli t5, t5, 1	# t5 *= 2
    			add t5, a1, t5	# t5 = addr of B[k][j]
    			add t6, a6, a6	# t6 = 256
    			lhu	s9, 0(t5)
    			add t5, t5, t6
		    	lhu	s10, 0(t5)
		    	add t5, t5, t6
		    	lhu	s11, 0(t5)
		    	add t5, t5, t6
		    	lhu	t0, 0(t5)
		    	add t5, t5, t6
		    	lhu	t1, 0(t5)
		    	add t5, t5, t6
		    	lhu	t2, 0(t5)
		    	add t5, t5, t6
		    	lhu	t3, 0(t5)
		    	add t5, t5, t6
		    	lhu	t4, 0(t5)
		    	mul s9, s9, s1
		    	mul s10, s10, s2
		    	mul s11, s11, s3
		    	mul t0, t0, s4
		    	mul t1, t1, s5
		    	mul t2, t2, s6
		    	mul t3, t3, s7
		    	mul t4, t4, s8
		    	add s9, s9, s10
		    	add s11, s11, t0
		    	add t1, t1, t2
		    	add t3, t3, t4
		    	add s9, s9, s11
		    	add t1, t1, t3
		    	add s9, s9, t1		# s9 = new add
		    	slli t5, a3, 7
		    	add t5, t5, a4
		    	slli t5, t5, 1
		    	add t5, a2, t5
		    	lh	a7, 0(t5)
		    	add a7, a7, s9
		    	andi a7, a7, 1023
		    	sh	a7, 0(t5)
		    	addi a4, a4, 1		# j++
		    	bne a4, a6, LOOPJ	# j < SIZE, goto LOOPJ
		    addi a5, a5, 8			# k += 8
		    bne a5, a6, LOOPK		# k < SIZE, goto LOOPK
		addi a3, a3, 1				# i++
		bne a3, a6, LOOPI			# i < SIZE, goto LOOPI
		beq zero, zero, EXIT


    EXIT:
	    ld	s1, 0(sp)
	    ld	s2, 8(sp)
	    ld 	s3, 16(sp)
	    ld	s4, 24(sp)
	    ld	s5, 32(sp)
	    ld 	s6, 40(sp)
	    ld	s7, 48(sp)
	    ld	s8, 56(sp)
	    ld 	s9, 64(sp)
	    ld	s10, 72(sp)
	    ld	s11, 80(sp)
        addi sp, sp, 88
    	ret
