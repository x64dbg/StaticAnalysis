/* plugin:   (StaticAnalysis) for x64dbg <http://www.x64dbg.com>
 * author:   tr4ceflow@gmail.com <http://blog.traceflow.com>
 * license:  GLPv3
 */
#include "AnalysisRunner.h"


AnalysisRunner::AnalysisRunner(duint BaseAddress,duint Size)
{
	mBaseAddress = BaseAddress;
	mSize = Size;

#ifdef _WIN64
	throw std::exception("The method or operation is not implemented.");
#else
	_Calls = new S_IntermodularCalls(this);
#endif // _WIN64

	initialise();
	clear();
}


AnalysisRunner::~AnalysisRunner(void)
{
}

void AnalysisRunner::start(){
	if(!mApiDb->ok())
		return;
	_plugin_logputs("[StaticAnalysis] analysis started ...");
	clear();
	run();
	think();
	_plugin_logputs("[StaticAnalysis] analysis finished ...");
}

unsigned int AnalysisRunner::disassembleInstruction(const duint bytesOffset,DISASM* disasm ){
	// prepare struct
	memset(&disasm, 0, sizeof(disasm));
	// test range
	if(currentEIP + bytesOffset > mSize)
		return UNKNOWN_OPCODE;
	// backup values
	duint baseaddr = mBaseAddress;
	duint size = mSize;

#ifdef _WIN64
	disasm.Archi=64;
#endif // _WIN64

	// prepare values in struct
	disasm->EIP=currentEIP + bytesOffset;
	disasm->VirtualAddr=currentVirtualAddr + bytesOffset;
	
	return (int) Disasm(disasm);
}

void AnalysisRunner::run()
{
	
	mCodeMemory = new unsigned char[mSize];
	if(!DbgMemRead(mBaseAddress, mCodeMemory, mSize))
	{
		//ERROR
		_plugin_logputs("[StaticAnalysis] could not read memory ...");
		return;
	}
	
	//loop over all instructions
	DISASM disasm;

	duint baseaddr = mBaseAddress;
	duint size = mSize;

	memset(&disasm, 0, sizeof(disasm));
#ifdef _WIN64
	disasm.Archi=64;
#endif // _WIN64
	currentEIP = (UIntPtr)mCodeMemory;
	disasm.EIP=currentEIP;
	currentVirtualAddr = (UInt64)baseaddr;
	disasm.VirtualAddr=currentVirtualAddr;
	duint i=0;
	while(i<size)
	{
		int len=Disasm(&disasm);
		if(len!=UNKNOWN_OPCODE)
		{
			emulateStack(&disasm);
			see(&disasm);
			backup(&disasm,len);
		}
		else{
			unknownOpCode(&disasm);
			len=1;
		}
		currentEIP += len;
		currentVirtualAddr += len;
		disasm.EIP=currentEIP;
		disasm.VirtualAddr=currentVirtualAddr;
		i+=len;
	}
	delete mCodeMemory;
}
void AnalysisRunner::clear(  )
{
	_Calls->clear();
}
void AnalysisRunner::think(  )
{
	_Calls->think();
}
void AnalysisRunner::see( const DISASM *disasm )
{
	_Calls->see(disasm);
}

void AnalysisRunner::unknownOpCode( const DISASM *disasm )
{
	_Calls->unknownOpCode(disasm);
}

ApiDB* AnalysisRunner::db()
{
	return mApiDb;
}

void AnalysisRunner::initialise()
{
	_Calls->initialise(mBaseAddress,mSize);
}

void AnalysisRunner::setDB( ApiDB *api )
{
	mApiDb = api;
}

void AnalysisRunner::emulateStack( DISASM* disasm )
{
	/* track all events:
	   - sub/add esp
	   - mov [esp+x], ???
	   - push/pop
	   - ret
	*/
	// 
	if ((disasm->Argument1.AccessMode == WRITE)
		&& (disasm->Argument1.ArgType & MEMORY_TYPE)
		&& (disasm->Argument1.Memory.BaseRegister & REG4)
		&& (disasm->Argument1.SegmentReg & SSReg )
		)  {
			int offset = disasm->Argument1.Memory.Displacement;
			duint addr = disasm->VirtualAddr;

	}
}

// remember instructions in buffer for further analysis
void AnalysisRunner::backup( DISASM* disasm , unsigned int len)
{
	mInstructionsBuffer.insert(std::pair<UInt64,DISASM>(disasm->VirtualAddr,Instruction_t(*disasm,len)));
}

// return instruction of address - if possible
int AnalysisRunner::instruction(UInt64 va, Instruction_t* instr) const{
	if(std::contains(mInstructionsBuffer,va)){
		*instr = mInstructionsBuffer.at(va);
		return instr->Length;
	}else{
		return UNKNOWN_OPCODE;
	}
}