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
	if(!mDb->ok())
		return;
	_plugin_logputs("[StaticAnalysis] analysis started ...");
	clear();
	run();
	think();
	_plugin_logputs("[StaticAnalysis] analysis finished ...");
}

unsigned int AnalysisRunner::getInstruction(const duint bytesOffset,DISASM* disasm ){


	memset(&disasm, 0, sizeof(disasm));

	if(currentEIP + bytesOffset > mSize)
		return UNKNOWN_OPCODE;

	duint baseaddr = mBaseAddress;
	duint size = mSize;


#ifdef _WIN64
	disasm.Archi=64;
#endif // _WIN64
	disasm->EIP=currentEIP + bytesOffset;
	disasm->VirtualAddr=currentVirtualAddr + bytesOffset;

	return (int) Disasm(disasm);

}

void AnalysisRunner::run()
{
	EIPdata = new unsigned char[mSize];
	if(!DbgMemRead(mBaseAddress, EIPdata, mSize))
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
	currentEIP = (UIntPtr)EIPdata;
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
	delete EIPdata;
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
	return mDb;
}

void AnalysisRunner::initialise()
{
	_Calls->initialise(mBaseAddress,mSize);
}

void AnalysisRunner::setDB( ApiDB *api )
{
	mDb = api;
}

void AnalysisRunner::emulateStack( DISASM* disasm )
{
	/* track all events:
	   - sub/add esp
	   - mov [esp+x], ???
	   - push/pop
	   - ret
	*/

	if ((disasm->Argument1.AccessMode == WRITE)
		&& (disasm->Argument1.ArgType & MEMORY_TYPE)
		&& (disasm->Argument1.Memory.BaseRegister & REG4))  {

	}
}
