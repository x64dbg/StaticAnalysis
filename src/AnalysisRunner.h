/* plugin:   (StaticAnalysis) for x64dbg <http://www.x64dbg.com>
 * author:   tr4ceflow@gmail.com <http://blog.traceflow.com>
 * license:  GLPv3
 */
#pragma once
#include "pluginsdk/bridgemain.h"
#include "pluginmain.h"
#include "pluginsdk/BeaEngine.h"


#ifdef _WIN64
throw std::exception("The method or operation is not implemented.");
#else
#include "S_IntermodularCallsX86.h"
#endif // _WIN64


#include "ApiDB.h"

class AnalysisRunner
{
	std::map<UInt64,Instruction_t> mInstructionsBuffer;
	duint mBaseAddress;
	duint mSize;

	S_IntermodularCalls *_Calls;
	ApiDB *mApiDb;

	unsigned char* mCodeMemory;
	UIntPtr currentEIP;
	UInt64 currentVirtualAddr;

public:

	ApiDB* db();

	void setDB(ApiDB *api);


	AnalysisRunner(duint BaseAddress,duint Size);
	~AnalysisRunner(void);

	void run();
	void see(const DISASM *disasm);
	void unknownOpCode( const DISASM *disasm );
	void start();
	void clear( );
	void think( );
	void initialise();
	unsigned int disassembleInstruction(const duint bytesOffset, DISASM* disasm );
	void emulateStack( DISASM* disasm );
	void backup( DISASM* disasm, unsigned int len );
	int instruction(UInt64 va, Instruction_t* instr) const

};

