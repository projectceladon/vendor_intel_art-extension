%default {"instr":"","suff":""}
    movzbq  2(rPC), %rcx                    # ecx <- BB
    movzbq  3(rPC), %rax                    # eax <- CC
    vmovs${suff}   VREG_ADDRESS(%rcx), %xmm1       # %xmm1 <- 1st src
    v${instr}${suff} VREG_ADDRESS(%rax), %xmm1, %xmm0
    vmovs${suff}   %xmm0, VREG_ADDRESS(rINSTq)     # vAA <- %xmm0
    vpxor    %xmm0, %xmm0, %xmm0
    vmovs${suff}   %xmm0, VREG_REF_ADDRESS(rINSTq) # clear ref
    ADVANCE_PC_FETCH_AND_GOTO_NEXT 2
