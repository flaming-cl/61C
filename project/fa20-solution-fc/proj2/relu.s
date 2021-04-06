.globl relu

.text
# ==============================================================================
# FUNCTION: Performs an inplace element-wise ReLU on an array of ints
# Arguments:
# 	a0 (int*) is the pointer to the array
#	a1 (int)  is the # of elements in the array
# Returns:
#	None
# Exceptions:
# - If the length of the vector is less than 1,
#   this function terminates the program with error code 78.
# ==============================================================================
relu:
    ble a1, x0, invalid_len
    li t0, 0
loop_start:
    lw t1, 0(a0) 			
    bge t1, x0, loop_continue
    sw x0, 0(a0)
loop_continue:
    addi t0, t0, 1
    beq t0, a1, loop_end
    addi a0, a0, 4
    j loop_start
loop_end:  
    ret
invalid_len:
    li a1, 78
    j exit2
