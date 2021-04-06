.globl dot

.text
# =======================================================
# FUNCTION: Dot product of 2 int vectors
# Arguments:
#   a0 (int*) is the pointer to the start of v0
#   a1 (int*) is the pointer to the start of v1
#   a2 (int)  is the length of the vectors
#   a3 (int)  is the stride of v0
#   a4 (int)  is the stride of v1
# Returns:
#   a0 (int)  is the dot product of v0 and v1
# Exceptions:
# - If the length of the vector is less than 1,
#   this function terminates the program with error code 75.
# - If the stride of either vector is less than 1,
#   this function terminates the program with error code 76.
# v0 = [1, 2, 3, 4, 5, 6, 7, 8, 9]
# v1 = [1, 2, 3, 4, 5, 6, 7, 8, 9]
# dot(v0, v1) = 1 * 1 + 2 * 2 + ... + 9 * 9 = 285
# =======================================================
dot:
    ble a2, x0, invalid_len    # if integer a2 <= 0 (aka a2 < 1), exit
    ble a3, x0, invalid_stride  # if the stride of v0 < 1, exit
    ble a4, x0, invalid_stride
    li t0, 0                   # t0: i = 0
    li t1, 0                   # t1: sum = 0
    slli t2, a3, 2             # transform v0 stride to offset a3
    slli t3, a4, 2             # transform v1 stride to offset a4
loop_start:
    lw t4, 0(a0)               # t4: v0[a0]
    lw t5, 0(a1)               # t5: v1[a1]
    mul t6, t4, t5             # t6 = t4 * t5
    add t1, t1, t6             # sum += v0[a0] * v1[a1]
    addi t0, t0, 1             # i++
    beq t0, a2, loop_end       # i == size, end
    add a0, a0, t2             # get the address of the next element
    add a1, a1, t3
    j loop_start
loop_end:
    add a0, t1, x0
    ret
invalid_len:
    li a1 75
    j exit2
invalid_stride:
    li a1 76
    j exit2
