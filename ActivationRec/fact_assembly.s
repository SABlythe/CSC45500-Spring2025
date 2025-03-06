factorial:
	# note: first parameter comes in through edi redister
	# note: goal is to store factorial into eax register (for return)

	pushq	%rbp		# back up calling func's activation record ptr

	movq	%rsp, %rbp	# make activation record point to top of stack

	# (int n)
	# int nm1, recResult, answer;
	subq	$16, %rsp	# allocate space for activation record (AR)

	# n <- paramter value
	movl	%edi, -4(%rbp)	# edi holds param, n is 1st int in act. record.

	# if (n==0)
	cmpl	$0, -4(%rbp)	# (0==n)? 
	jne	else_part	# false case goes to else portion of code

	# answer = 1;
	movl	$1, -16(%rbp)	# store 1 in answer (4th int in act. record)
	jmp	return_part

else_part:
        # nm1 = n-1;
	movl	-4(%rbp), %eax	# n is 1st int in act. record. 	
	subl	$1, %eax
	movl	%eax, -8(%rbp)	# nm1 is 2nd int in act. record

 	# recResult = factorial (nm1);
	movl	-8(%rbp), %edi	# nm1 is 2nd in act. record; edi is input param
	callq	factorial	# make recursive call (pushes onto stack)
	movl	%eax, -12(%rbp)	# eax holds return value; recResult is 3rd in AR

	# answer = n * recResult;
	movl	-4(%rbp), %eax	# n is 1st in AR
	imull	-12(%rbp), %eax # recResult is 3rd in AR
	movl	%eax, -16(%rbp)	# answer is 4th in AR

return_part:
	# return answer;  ####  really next 4 assembly statements
	movl	-16(%rbp), %eax	# place answer (4th in AR) into return register

	#   unallocate activation record (AR)
	addq	$16, %rsp	# move top of stack back 
	popq	%rbp		# restore caller's AR ...

	retq			# go back to caller (pops stack)
