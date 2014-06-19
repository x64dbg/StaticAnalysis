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

void AnalysisRunner::run()
{
	unsigned char* data = new unsigned char[mSize];
	if(!DbgMemRead(mBaseAddress, data, mSize))
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
	disasm.EIP=(UIntPtr)data;
	disasm.VirtualAddr=(UInt64)baseaddr;
	duint i=0;
	while(i<size)
	{
		int len=Disasm(&disasm);
		if(len!=UNKNOWN_OPCODE)
		{
			see(&disasm);
		}
		else{
			unknownOpCode(&disasm);
			len=1;
		}
		disasm.EIP+=len;
		disasm.VirtualAddr+=len;
		i+=len;
	}
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
