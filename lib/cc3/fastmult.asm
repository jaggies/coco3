*******************************************************************************
* FUNCTION int32_t fastmult(int16_t a, int16_t b)
*******************************************************************************
	SECTION	code
_fastmult   EXPORT
_fastmult   EQU *
    pshs    U
    leau    ,S
    ldd     6,u variable a
    muld    8,u variable b
    ldx     4,u return address
    stq     ,x
    leas    ,U
    puls    U,PC
* END FUNCTION
	ENDSECTION

