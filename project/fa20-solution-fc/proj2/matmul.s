.globl matmul

.text
# =======================================================
# FUNCTION: Matrix Multiplication of 2 integer matrices
# 	d = matmul(m0, m1)
# Arguments:
# 	a0 (int*)  is the pointer to the start of m0 
#	a1 (int)   is the # of rows (height) of m0
#	a2 (int)   is the # of columns (width) of m0
#	a3 (int*)  is the pointer to the start of m1
# 	a4 (int)   is the # of rows (height) of m1
#	a5 (int)   is the # of columns (width) of m1
#	a6 (int*)  is the pointer to the the start of d
# Returns:
#	None (void), sets d = matmul(m0, m1)
# Exceptions:
#   Make sure to check in top to bottom order!
#   - If the dimensions of m0 do not make sense,
#     this function terminates the program with exit code 72.
#   - If the dimensions of m1 do not make sense,
#     this function terminates the program with exit code 73.
#   - If the dimensions of m0 and m1 don't match,
#     this function terminates the program with exit code 74.
# =======================================================
matmul:
# error_checks  
    ble a1, x0, invalid_dimensions_m0
    ble a2, x0, invalid_dimensions_m0
    ble a3, x0, invalid_dimensions_m1
    ble a4, x0, invalid_dimensions_m1
    bne a2, a4, invalid_dimensions_match # c(r x q) = A(r x p) * B(p x q)
# prologue:
	addi sp, sp, -40
    sw s0, 0(sp)
    sw s1, 4(sp)
    sw s2, 8(sp)
    sw s3, 12(sp)
    sw s4, 16(sp)
    sw s5, 20(sp)
    sw s6, 24(sp)
    sw s7, 28(sp)
    sw s8, 32(sp)
    sw ra, 36(sp)
    mv s0, a0
    mv s1, a1
    mv s2, a2
    mv s3, a3
    mv s4, a3
    mv s5, a5
    mv s6, a6
    mv s7, x0                            # s7: i = 0 
    mv s8, x0                            # s8: j = 0
loop_start:
	mv a0, s0     						 # a0 = m0
    mv a1, s3     				         # a1 = m1
    mv a2, s2     						 # a2 = col of m0
    li a3, 1     						 # a3 = 1, m0 stride
    mv a4, s5     						 # a4 = col of m1, m1 stride
    
    jal dot     						 # get dot product
    
    sw a0, 0(s6)     				     # sum += result of dot product
    addi s6, s6, 4
    
    addi s8, s8, 1						 # j += 1
    beq s8, s5, inner_loop_end           # j = col of m1, stop inner loop
    
    addi s3, s3, 4						 # go to the next element of m1

    j loop_start
inner_loop_end:
	addi s7, s7, 1						 # i += 1					     
    beq s7, s1, outer_loop_end			 # i = row of m1, stop outer loop
    
    slli t0, s2, 2
    add s0, s0, t0                       # m0 += col of m0
    
    mv s3, s4                            # current m1 = original m1

    li s8, 0   						     # j = 0
    
    j loop_start
outer_loop_end:
# epilogue
	lw s0, 0(sp)
    lw s1, 4(sp)
    lw s2, 8(sp)
    lw s3, 12(sp)
    lw s4, 16(sp)
    lw s5, 20(sp)
    lw s6, 24(sp)
    lw s7, 28(sp)
    lw s8, 32(sp)
    lw ra, 36(sp)
    addi sp, sp, 40
    
    ret
invalid_dimensions_m0:
	li a1, 72
    j exit2
invalid_dimensions_m1:
	li a1, 73
    j exit2
invalid_dimensions_match:
	li a1, 74
    j exit2