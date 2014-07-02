/* plugin:   (StaticAnalysis) for x64dbg <http://www.x64dbg.com>
 * author:   tr4ceflow@gmail.com <http://blog.traceflow.com>
 * license:  GLPv3
 */
#pragma once
#include "ICommand.h"
#include <list>
#include <set>
#include <map>

struct _Call{
	std::list<duint> arguments;             // address of arguments
	duint RVA;                              // relative VA of call
	duint RelativeAddressValue;             // call "xxxx"
	bool isApiCall;


	void clear(){
		arguments.clear();
		RVA = 0;
		RelativeAddressValue = 0;
		isApiCall = false;
	}
	_Call(){
		isApiCall=false;
	}	


};



class S_IntermodularCalls : public ICommand
{

	struct _JumpFF{
		duint RVA;
		duint RelativeAddressValue;
		char label[MAX_LABEL_SIZE];

		bool operator==(const _JumpFF& rhs) const
		{
			return (RVA == rhs.RVA);
		}
		bool operator<(const _JumpFF& rhs) const
		{
			return (RVA < rhs.RVA);
		}
	};

	_Call mCurrent;
	std::set<_JumpFF> mApiJumps;
	std::list<_Call> mCalls;
	std::map<duint,std::string> mComments;

public:

	S_IntermodularCalls(AnalysisRunner *parent);

	void clear();
	void see(const DISASM* disasm);
	bool think();
	void unknownOpCode(const DISASM* disasm);
};

