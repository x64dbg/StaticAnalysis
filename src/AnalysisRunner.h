/* plugin:   (StaticAnalysis) for x64dbg <http://www.x64dbg.com>
 * author:   tr4ceflow@gmail.com <http://blog.traceflow.com>
 * license:  GLPv3
 */
#pragma once
#include "pluginsdk/bridgemain.h"
/* plugin:   (StaticAnalysis) for x64dbg <http://www.x64dbg.com>
 * author:   tr4ceflow@gmail.com <http://blog.traceflow.com>
 * license:  GLPv3
 */
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
	duint mBaseAddress;
	duint mSize;

	S_IntermodularCalls *_Calls;
	ApiDB *mDb;


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
};

