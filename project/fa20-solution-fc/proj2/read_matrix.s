.globl read_matrix

.text
# ==============================================================================
# FUNCTION: Allocates memory and reads in a binary file as a matrix of integers
#
# FILE FORMAT:
#   The first 8 bytes are two 4 byte ints representing the # of rows and columns
#   in the matrix. Every 4 bytes afterwards is an element of the matrix in
#   row-major order.
# Arguments:
#   a0 (char*) is the pointer to string representing the filename
#   a1 (int*)  is a pointer to an integer, we will set it to the number of rows
#   a2 (int*)  is a pointer to an integer, we will set it to the number of columns
# Returns:
#   a0 (int*)  is the pointer to the matrix in memory
# Exceptions:
# - If malloc returns an error,
#   this function terminates the program with error code 88.
# - If you receive an fopen error or eof, 
#   this function terminates the program with error code 90.
# - If you receive an fread error or eof,
#   this function terminates the program with error code 91.
# - If you receive an fclose error or eof,
#   this function terminates the program with error code 92.
# ==============================================================================
read_matrix:
#prologue:
    addi sp, sp, -36
    sw s0, 0(sp)
    sw s1, 4(sp)
    sw s2, 8(sp)
    sw s3, 12(sp)
    sw s4, 16(sp)
    sw s5, 20(sp)
    sw s6, 24(sp)
    sw s7, 28(sp)
    sw ra, 32(sp)
    li s0, 4                    # s0: size of int
    mv s1, a1                   # s1: a1, pointer, the number of rows
    mv s2, a2			# s2: a1, pointer, the number of cols
    li s7, 0                    # counter
#open_file:
    mv a1, a0                   
    li a2, 0                    
    jal fopen
    li t0, -1
    beq a0, t0, fopen_failed
    mv s3, a0                   # s3: fopen return value
#read_file:    
    mv a1, a0
    mv a2, s1
    mv a3, s0
    jal fread
    bne a0, s0, fread_failed 
    
    mv a1, s3
    mv a2, s2
    mv a3, s0
    jal fread
    bne a0, s0, fread_failed 

    lw t0, 0(s1)
    lw t1, 0(s2)
    mul s4, t0, t1              # s4: rows * cols
#allocate_memory:
    slli t0, s4, 2
    mv a0, t0
    jal malloc
    beq a0, x0, malloc_failed
    mv s5, a0                   # s5: file pointer
    mv s6, a0                   # s6: file pointer
save_bytes_loop:
    mv a1, s3
    mv a2, s5
    mv a3, s0
    jal fread
    bne a0, s0, fread_failed
    addi s7, s7, 1
    beq s7, s4, loop_end
    add s5, s5, s0
    j save_bytes_loop
loop_end:
    mv a1, s3
    jal fclose
    li t0, -1
    beq a0, t0, fclose_failed
    
    mv a0, s6
#epilogue:
    lw s0, 0(sp)
    lw s1, 4(sp)
    lw s2, 8(sp)
    lw s3, 12(sp)
    lw s4, 16(sp)
    lw s5, 20(sp)
    lw s6, 24(sp)
    lw s7, 28(sp)
    lw ra, 32(sp)
    addi sp, sp, 36
    
    ret
malloc_failed:
    li a1 88
    j exit2
fopen_failed:
    li a1 90
    j exit2
fread_failed:
    li a1 91
    j exit2
fclose_failed:
    li a1 92
    j exit2
