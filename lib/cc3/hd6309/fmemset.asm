	SECTION	code

_fmemset	EXPORT
_fmemset	EQU	*
	pshs	u
	leau	,s
	ldx		8,u		variable n
	tfr		x,w
	leax	7,u		variable c
	ldy		4,u		pointer s
	tfm		x,y+
	leas	,u
	puls	u,pc

	ENDSECTION

	END
