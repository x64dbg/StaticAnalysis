#pragma once

/* calling convention

   1. arg -> RCX (floating point: XMM0)
   2. arg -> RDX (floating point: XMM1)
   3. arg -> R8  (floating point: XMM2)
   4. arg -> R9  (floating point: XMM3)    

	additional arguments are pushed onto the stack (right to left)

*/


class S_IntermodularCalls
{
public:
	S_IntermodularCalls(void);
	~S_IntermodularCalls(void);
};

