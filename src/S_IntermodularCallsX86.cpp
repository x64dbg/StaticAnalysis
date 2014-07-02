/* plugin:   (StaticAnalysis) for x64dbg <http://www.x64dbg.com>
 * author:   tr4ceflow@gmail.com <http://blog.traceflow.com>
 * license:  GLPv3
 */
#include "S_IntermodularCallsX86.h"
#include "AnalysisRunner.h"
#include "meta.h"

#define _isPush(disasm)  ((strcmp(disasm->Instruction.Mnemonic ,"push ") == 0) )
#define _isCall(disasm)  ((disasm->Instruction.Opcode == 0xE8) && (disasm->Instruction.BranchType) && (disasm->Instruction.BranchType!=RetType) && !(disasm->Argument1.ArgType &REGISTER_TYPE))


S_IntermodularCalls::S_IntermodularCalls(AnalysisRunner *parent) : ICommand(parent){

}

void S_IntermodularCalls::clear()
{
	mCurrent.clear();
	mApiJumps.clear();
	mCalls.clear();
	mComments.clear();
}

void S_IntermodularCalls::see(const  DISASM* disasm )
{
	if(_isPush(disasm))
	{
		// there is "push ..." --> remember
		mCurrent.arguments.push_back(disasm->VirtualAddr );
	}else {
		if(disasm->Instruction.Opcode == 0xFF)
		{
			// no part of api-call
			mCurrent.clear();
			//_plugin_logprintf("[StaticAnalysis:IntermodularCalls] found 0xFF : %x and mne (%s)\n", disasm->Instruction.Opcode,disasm->Instruction.Mnemonic);
			char labelText[MAX_LABEL_SIZE];
			bool hasLabel = DbgGetLabelAt(disasm->Argument1.Memory.Displacement, SEG_DEFAULT, labelText);

			if(hasLabel){
				// api calls have 0xFF-jumps as target addresses, we need to remember them
				_JumpFF j;
				j.RVA = disasm->VirtualAddr;
				j.RelativeAddressValue = disasm->Argument1.Memory.Displacement;
				strcpy(j.label,labelText);
				mApiJumps.insert(j);
			}
		}else {
			if(_isCall(disasm))
			{
				// set missing informations
				mCurrent.RVA = disasm->VirtualAddr ;
				mCurrent.RelativeAddressValue = disasm->Instruction.AddrValue ;
				// copy structure
				_Call t = mCurrent;
				mCalls.push_back(t);
				mCurrent.clear();

			}else
			{
				// something else, in particular no call
				mCurrent.clear();
			}
		}
	}
}

bool S_IntermodularCalls::think()
{
	_plugin_logprintf("[StaticAnalysis:IntermodularCalls] found %i calls\n", mCalls.size());

	unsigned int numberOfApiCalls = 0;
	std::list<_Call>::iterator c = mCalls.begin();

	// iterate all calls
	for (std::list<_Call>::iterator c = mCalls.begin();c != mCalls.end();c++)
	{
		bool is_api = false;
		_JumpFF findJump; 
		findJump.RVA = c->RelativeAddressValue;
		std::set<_JumpFF>::iterator j = mApiJumps.find(findJump);
		if(j != mApiJumps.end()){
			// having a label we can look in our database to get the arguments list
			APIFunction f; 
			f = mParent->db()->find(j->label);
			if(!f.invalid){
				// yeah we know everything about the api-call!
				std::string functionComment;
				functionComment = f.ReturnType + " " + f.Name + "(...)";
				DbgSetCommentAt(c->RVA ,functionComment.c_str());
				//_plugin_logprintf("[StaticAnalysis:IntermodularCalls] found %s at %X\n",functionComment , c->RVA);
				int pos=0;
				// set comments for the arguments
				if( (f.Arguments.size() > 0)  && (c->arguments.size() > 0) ){
					for(std::list<duint>::reverse_iterator a= c->arguments.rbegin();a!=c->arguments.rend();a++)
					{
						// more push" instructions than arguments
						if(pos >=f.Arguments.size())
							break;

						std::string ArgComment = f.arg(pos).Type + " "+ f.arg(pos).Name;
						DbgSetCommentAt((*a) ,ArgComment.c_str());
						pos++;
					}
				}
			}
			is_api = true;
			numberOfApiCalls++;
		}
	}

	_plugin_logprintf("[StaticAnalysis:IntermodularCalls] of which are %i intermodular calls\n", numberOfApiCalls);


	return true;
}

void S_IntermodularCalls::unknownOpCode(const  DISASM* disasm )
{
	// current instruction wasn't correctly disassembled, so assuming worst-case
	mCurrent.clear();
}

