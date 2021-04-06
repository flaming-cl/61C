.globl classify

.text
classify:
    # =====================================
    # COMMAND LINE ARGUMENTS
    # =====================================
    # Args:
    #   a0 (int)    argc
    #   a1 (char**) argv
    #   a2 (int)    print_classification, if this is zero, 
    #               you should print the classification. Otherwise,
    #               this function should not print ANYTHING.
    # Returns:
    #   a0 (int)    Classification
    # Exceptions:
    # - If there are an incorrect number of command line args,
    #   this function terminates the program with exit code 89.
    # - If malloc fails, this function terminats the program with exit code 88.
    #
    # Usage:
    #   main.s <M0_PATH> <M1_PATH> <INPUT_PATH> <OUTPUT_PATH>
    
    # =====================================
    # ARGC CHECK & PROLOGUE
    # =====================================
    
    #argc check
    li t0, 5
    bne a0, t0, argc_invalid
    
    #prologue:
    addi sp, sp, -52
    sw s11, 48(sp)
    sw s10, 44(sp)
    sw s9, 40(sp)
    sw s8, 36(sp)
    sw s7, 32(sp)
    sw s6, 28(sp)
    sw s5, 24(sp)
    sw s4, 20(sp)
    sw s3, 16(sp)
    sw s2, 12(sp)
    sw s1, 8(sp)
    sw s0, 4(sp)
    sw ra, 0(sp)
    li s0, 4			      # s0: size of int	                    
    mv s1, a1                         # s1: argv
    mv s2, a2                         # s2: print classification flag
    
    # =====================================
    # LOAD MATRICES
    # =====================================
    
    # Load pretrained m0
    li a0, 8
    jal malloc
    beq a0, x0, malloc_failed
    mv s3, a0                          # s3: pointer - m0 rows & cols
    lw a0, 4(s1)                       # read_matrix a0: file name, argv[1]
    mv a1, s3			       # read_matrix a1: pointer for storing rows
    add a2, s3, s0		       # read_matrix a2: pointer for storing cols
    jal read_matrix
    mv s4, a0			       # s4: pointer - m0 matrix
    
    # Load pretrained m1
    li a0, 8
    jal malloc
    beq a0, x0, free_memory_and_exit   # if malloc fails, free previously allocated memory in heap
    mv s5, a0                          # s5: pointer - m1 rows & cols
    lw a0, 8(s1)                       # read_matrix a0: file name, argv[1]
    mv a1, s5			       # read_matrix a1: pointer for storing rows
    add a2, s5, s0		       # read_matrix a2: pointer for storing cols
    jal read_matrix
    mv s6, a0			       # s6: pointer - m1 matrix

    # Load input matrix
    li a0, 8
    jal malloc
    beq a0, x0, free_memory_and_exit
    mv s7, a0                          # s7: pointer - input rows & cols
    lw a0, 12(s1)                      # read_matrix a0: file name, argv[1]
    mv a1, s7			       # read_matrix a1: pointer for storing rows
    add a2, s7, s0		       # read_matrix a2: pointer for storing cols
    jal read_matrix
    mv s8, a0			       # s8: pointer - input matrix

    # =====================================
    # RUN LAYERS
    # =====================================
    
    # 1. LINEAR LAYER:    m0 * input
    lw t0, 0(s3)                      	# t0: rows of s9 (also rows of m0)
    lw t1, 4(s7)                      	# t1: cols of s9 (also rols of input)
    mul s9, t0, t1                      # s9: len of m0 * input
    slli a0, s9, 2
    jal malloc
    beq a0, x0, free_memory_and_exit
    mv s10, a0
    
    mv a0, s4
    lw a1, 0(s3)
    lw a2, 4(s3)
    mv a3, s8
    lw a4, 0(s7)
    lw a5, 4(s7)
    mv a6, s10                           # s10: pointer - array m0 * input
    jal matmul
    
    # 2. NONLINEAR LAYER: ReLU(m0 * input)
    mv a0, s10
    mv a1, s9
    jal relu                             # s10: ReLU(m0 * input)
    
    # 3. LINEAR LAYER:    m1 * ReLU(m0 * input)
    lw a0, 0(s5)                         # rows of m1
    lw t0, 4(s7)                         # cols of ReLU(m0 * input)
    mul a0, a0, t0
    slli a0, a0, 2						 
    jal malloc
    beq a0, x0, free_memory_and_exit
    mv s11, a0                           # s11: pointer - array m1 * ReLU(m0 * input)
    
    mv a0, s6
    lw a1, 0(s5)
    lw a2, 4(s5)
    mv a3, s10
    mv a4, s9
    lw a5, 4(s7)
    mv a6, s11
    jal matmul
    
    # =====================================
    # WRITE OUTPUT
    # =====================================
    
    # Write output matrix
    lw a0, 16(s1)
    mv a1, s11
    lw a2, 0(s5)
    lw a3, 4(s7)
    jal write_matrix
    
    # =====================================
    # CALCULATE CLASSIFICATION/LABEL
    # =====================================
    
    # Call argmax
    mv a0, s11
    lw a1, 0(s5)
    jal argmax
    mv s0, a0
    
    # Print classification
    bne s2, x0, end
    mv a1, s0
    jal print_int
    
    # Print newline afterwards for clarity
    li a1 '\n'
    jal print_char
    
end:
    # =====================================
    # FREE MEMORY & EPILOGUE & RETURN
    # =====================================
    mv a0, s0

    #free memory
    mv a0, s3
    jal free
    mv a0, s5
    jal free
    mv a0, s7
    jal free
    mv a0, s10
    jal free
    mv a0, s11
    jal free
    
    #epilogue
    lw ra, 0(sp)
    lw s0, 4(sp)
    lw s1, 8(sp)
    lw s2, 12(sp)
    lw s3, 16(sp)
    lw s4, 20(sp)
    lw s5, 24(sp)
    lw s6, 28(sp)
    lw s7, 32(sp)
    lw s8, 36(sp)
    lw s9, 40(sp)
    lw s10, 44(sp)
    lw s11, 48(sp)
    addi sp, sp, 52
    
    ret
free_memory_and_exit:
    beq s3, x0, malloc_failed
    mv a0, s3
    jal free
    
    beq s5, x0, malloc_failed
    mv a0, s5
    jal free
    
    beq s7, x0, malloc_failed
    mv a0, s7
    jal free
    
    beq s10, x0, malloc_failed
    mv a0, s10
    jal free
    
    beq s11, x0, malloc_failed
    mv a0, s1
    jal free
argc_invalid:
    li a1, 89
    j exit2
malloc_failed:
    li a1, 88
    j exit2
