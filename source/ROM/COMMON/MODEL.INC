#ifdef __A251__
$IF ROMHUGE         ; SET(ROMHUGE)
    POSTFIX LIT '?'
    RETURN  LIT 'ERET'
    JUMP    LIT 'EJMP'
    MCALL   LIT 'ECALL'
    Prefix  LIT '?'
    Model   LIT 'FAR'
    #ifdef AB1520B
    PRSeg   LIT 'ECODE_LAREA'
    #else
    PRSeg   LIT 'ECODE'
    #endif
$ELSEIF ROMLARGE    ; SET(ROMLARGE)
    POSTFIX LIT '_'
    RETURN  LIT 'RET'
    JUMP    LIT 'LJMP'
    MCALL   LIT 'LCALL'
    Prefix  LIT ''
    Model   LIT 'NEAR'
    PRSeg   LIT 'CODE'
$ELSE
__ERROR__ "UNDEFINED CASE"
$ENDIF
#else
__ERROR__ "UNDEFINED CASE"
#endif
