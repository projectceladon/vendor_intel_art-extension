%default {"instr":"","suff":""}
    movl    rINST, %ecx                     # ecx <- A+
    andl    $$0xf, %ecx                     # ecx <- A
    vmovs${suff} VREG_ADDRESS(%rcx), %xmm1        # %xmm1 <- 1st src
    sarl    $$4, rINST                      # rINST<- B
    v${instr}${suff}  VREG_ADDRESS(rINSTq), %xmm1,  %xmm0
    vmovs${suff} %xmm0, VREG_ADDRESS(%rcx)   # vAA<- %xmm0
    vpxor    %xmm0, %xmm0, %xmm0
    vmovs${suff} %xmm0, VREG_REF_ADDRESS(rINSTq)  # clear ref
    ADVANCE_PC_FETCH_AND_GOTO_NEXT 1
