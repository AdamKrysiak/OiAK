.data

infRound: .byte 0x7f, 0x08
nearestRound: .byte 0x7f, 0x00
minusInfRound: .byte 0x7f, 0x04

getPrec: .byte 0,0

.text

.type fpu_ctr, @function
.global fpu_ctr
fpu_ctr:
pushq %rbp
movq %rsp, %rbp
			#w rdi 1 arg
			#w rsi 2 arg
fstcw getPrec

cmp $1, %rdi
je caseEqual
jg caseBigger		#skok jesli dodatnie	(jump sign)
jl caseSmaller

caseBigger:
	#+inf
	fldcw infRound
	jmp exit

caseEqual:
	#nearest
	fldcw nearestRound
	jmp exit

caseSmaller:
	#-inf
	fldcw minusInfRound
	jmp exit

exit:
movq %rbp, %rsp
popq %rbp
		

ret
