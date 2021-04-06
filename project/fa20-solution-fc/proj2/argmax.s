.globl argmax

.text
# =================================================================
# FUNCTION: Given a int vector, return the index of the largest
#	element. If there are multiple, return the one
#	with the smallest index.
# Arguments:
# 	a0 (int*) is the pointer to the start of the vector
#	a1 (int)  is the # of elements in the vector
# Returns:
#	a0 (int)  is the first index of the largest element
# Exceptions:
# - If the length of the vector is less than 1,
#   this function terminates the program with error code 77.
# =================================================================
argmax:
    ble a1, x0, invalid_len # length check, if size <= 0, exit
    li t0, 0                  # t0: counter i
    li t1, 0                  # t1: max_i
    li t2, 0                  # t2: max_value
loop_start:
    lw t3, 0(a0)              # t3: a[i]
    ble t3, t2, loop_continue # a[i] <= max_value, continue
    add t1, t0, x0            # max_i = i
    add t2, t3, x0            # max_value = a[i]
loop_continue:
    addi t0, t0, 1            # i++
    beq t0, a1, loop_end      # i == size, end
    addi a0, a0, 4
    j loop_start
loop_end:
    add a0, t1, x0
    ret
invalid_len:
    li a1 77
    j exit2
