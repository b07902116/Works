.global fibonacci
.type fibonacci, %function

.align 2
# unsigned long long int fibonacci(int n);
fibonacci:  
    add t1, a0, 0			# t1 = n
    add t2, zero, 1			# t2 = i = 1
    add t3, zero, 0			# t3 = a = 0
    addi t4, zero, 1		# t4 = b = 1
    beq t1, zero, EXIT		# if n == 0, exit
    L1:
    	beq t2, t1, EXIT 	# if i == n, exit
    	add a0, t3, t4		# a0 = a + b
    	add t3, t4, 0		# a = b
    	add t4, a0, 0		# b = a0
    	addi t2, t2, 1		# i++
    	beq zero, zero, L1
    EXIT:
    	ret
