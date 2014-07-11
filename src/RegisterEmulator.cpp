#include "RegisterEmulator.h"



RegisterEmulator::RegisterEmulator()
{
}


RegisterEmulator::~RegisterEmulator()
{
}

void RegisterEmulator::emulate(const DISASM* BeaStruct)
{


	if ((BeaStruct->Argument1.AccessMode == WRITE) && ((BeaStruct->Argument1.ArgType & GENERAL_REG))){
		 
		if (BeaStruct->Argument1.ArgType & REG1){
			mRCX = BeaStruct->VirtualAddr;
		}
		else if (BeaStruct->Argument1.ArgType & REG2){
			mRDX = BeaStruct->VirtualAddr;
		}
		else if (BeaStruct->Argument1.ArgType & REG8){
			mR8 = BeaStruct->VirtualAddr;
		}
		else if (BeaStruct->Argument1.ArgType & REG9){
			mR9 = BeaStruct->VirtualAddr;
		}
	}
}

const duint RegisterEmulator::rcx() const
{
	return mRCX;
}

const duint RegisterEmulator::rdx() const
{
	return mRDX;
}

const duint RegisterEmulator::r8() const
{
	return mR8;
}

const duint RegisterEmulator::r9() const
{
	return mR9;
}
