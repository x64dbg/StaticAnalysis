/* plugin:   (StaticAnalysis) for x64dbg <http://www.x64dbg.com>
 * author:   tr4ceflow@gmail.com <http://blog.traceflow.com>
 * license:  GLPv3
 */
#include "StackEmulator.h"



StackEmulator::StackEmulator( void) : mStackpointer(0)
{
	for (unsigned int i=0;i<MAX_STACKSIZE;i++)
		mStack[i]=0;
}


StackEmulator::~StackEmulator(void)
{
}

/*
 0x01234: mov [esp+C], eax
 --> modifyFrom(0xC,0x01234)
*/
void StackEmulator::modifyFrom(int relative_offset,duint addr){
	const unsigned int internal_pointer = pointerByOffset(-1*relative_offset);
	mStack[internal_pointer] = addr;
}
/*
 0x01234: pop eax
 --> popFrom(0x01234), because it is:

 mov eax, [esp]
 add esp, -1    ; one to the past

*/
void StackEmulator::popFrom( duint addr )
{
	moveStackpointer(+1);
}
/*
 0x01234: push eax
 --> pushFrom(0x01234), because it is:

 add esp, +1
 mov [esp], eax  ; one to the future
*/
void StackEmulator::pushFrom( duint addr )
{
	mStack[mStackpointer] = addr;
	moveStackpointer(-1);
}
/*
 0x01234: add esp, 0xA
 --> moveStackpointer(0xA)

 offset = offset to the past
*/
void StackEmulator::moveStackpointer( int offset )
{
	mStackpointer = pointerByOffset(-offset);
}

unsigned int StackEmulator::pointerByOffset(int offset){
	return (mStackpointer + ((offset+MAX_STACKSIZE) %MAX_STACKSIZE) + MAX_STACKSIZE) %MAX_STACKSIZE;
}

/* returns addr from last access
   
   0x155: mov [esp+8], eax

   lastAccessAt(0x8)  would be 0x155

*/
duint StackEmulator::lastAccessAt(int offset){
	int p = pointerByOffset(-offset);
	return mStack[p];
}

