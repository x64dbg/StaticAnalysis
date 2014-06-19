/* plugin:   (StaticAnalysis) for x64dbg <http://www.x64dbg.com>
 * author:   tr4ceflow@gmail.com <http://blog.traceflow.com>
 * license:  GLPv3
 */
#pragma once
#include "pluginsdk/bridgemain.h"
#include "pluginmain.h"
#include "pluginsdk/BeaEngine.h"

class AnalysisRunner;
class ICommand
{
public:
	ICommand(AnalysisRunner *parent);
	virtual ~ICommand(void);


protected:
	duint mBase;
	duint mSize;
	AnalysisRunner *mParent;

public:

	// initialization before any analysis
	void initialise(const duint Base,const duint Size);
	// clear all extracted informations
	virtual void clear() = 0;
	// each derived class will see each instruction only once (in a analysis step)
	virtual void see(const DISASM* disasm) = 0;
	// this methods process all gathered informations
	virtual bool think() = 0;

	void unknownOpCode(const DISASM* disasm);
};

