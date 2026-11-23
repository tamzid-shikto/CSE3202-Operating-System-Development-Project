/* ctxsw(char **old_sp, char **new_sp) */
    .text
    .globl ctxsw
ctxsw:
    pushl %ebp
    pushfl
    pushal
    movl 44(%esp), %eax
    movl %esp, (%eax)
    movl 48(%esp), %eax
    movl (%eax), %esp
    popal
    popfl
    popl %ebp
    ret

    .section .note.GNU-stack,"",@progbits