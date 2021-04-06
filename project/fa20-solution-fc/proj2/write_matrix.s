.globl write_matrix

.text
# ==============================================================================
# FUNCTION: Writes a matrix of integers into a binary file
# FILE FORMAT:
#   The first 8 bytes of the file will be two 4 byte ints representing the
#   numbers of rows and columns respectively. Every 4 bytes thereafter is an
#   element of the matrix in row-major order.
# Arguments:
#   a0 (char*) is the pointer to string representing the filename
#   a1 (int*)  is the pointer to the start of the matrix in memory (containing what we want to write)
#   a2 (int)   is the number of rows in the matrix
#   a3 (int)   is the number of columns in the matrix
# Returns:
#   None
# Exceptions:
# - If you receive an fopen error or eof,
#   this function terminates the program with error code 93.
# - If you receive an fwrite error or eof,
#   this function terminates the program with error code 94.
# - If you receive an fclose error or eof,
#   this function terminates the program with error code 95.
# ==============================================================================
write_matrix:
#prologue:
	addi sp, sp, -28
    sw ra, 24(sp)
    sw s5, 20(sp)
    sw s4, 16(sp)
    sw s3, 12(sp)
    sw s2, 8(sp)
    sw s1, 4(sp)
    sw s0, 0(sp)
    mv s0, a2
    mv s1, a3
    mv s2, a1
    mul s3, a2, a3                
    li s4, 4                    # s4: size of int
#open_file:
    mv a1, a0                   
	li a2, 1                    # a2: 1, write permission                
	jal fopen
    li t0, -1
    beq a0, t0, fopen_failed
    mv s5, a0                   # s5: fopen return value
#write_file: 
#add pointer for the value of rows
	addi sp, sp, -4				
    sw s0, 0(sp)
# rows
    mv a1, s5					# a1: the file descriptor of the file we want to write to
    mv a2, sp					# a2: a pointer to a buffer containing what we want to write
    li a3, 1					# a3: the number of elements to write
    mv a4, s4					# a4: bytes of the size of each buffer element
    jal fwrite
    li t0, 1
    bne a0, t0, fwrite_failed
    addi sp, sp, 4
#add pointer for the value of cols 
	addi sp, sp, -4				
    sw s1, 0(sp)
# cols 
    mv a1, s5					# a1: the file descriptor of the file we want to write to
    mv a2, sp					# a2: a pointer to a buffer containing what we want to write
    li a3, 1					# a3: the number of elements to write
    mv a4, s4					# a4: bytes of the size of each buffer element
    jal fwrite
    li t0, 1
    bne a0, t0, fwrite_failed
    addi sp, sp, 4
# matrix    
	mv a1, s5
    mv a2, s2
    mv a3, s3
    mv a4, s4
    jal fwrite
    bne a0, s3, fwrite_failed 
#close file:
    mv a1, s5
    jal fclose
    li t0, -1
    beq a0, t0, fclose_failed
#epilogue:
    lw s0, 0(sp)
    lw s1, 4(sp)
    lw s2, 8(sp)
    lw s3, 12(sp)
    lw s4, 16(sp)
    lw s5, 20(sp)
    lw ra, 24(sp)
    addi sp, sp, 28
    
    ret
fopen_failed:
	li a1 93
    j exit2
fwrite_failed:
	li a1 94
    j exit2
fclose_failed:
	li a1 95
    j exit2