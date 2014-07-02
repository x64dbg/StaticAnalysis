/* plugin:   (StaticAnalysis) for x64dbg <http://www.x64dbg.com>
 * author:   tr4ceflow@gmail.com <http://blog.traceflow.com>
 * license:  GLPv3
 */
#include "S_IntermodularCallsX86.h"
#include "AnalysisRunner.h"
#include "meta.h"

/*#define _isPush(disasm)  (QString(disasm->Instruction.Mnemonic).trimmed().toLower() == "push")
#define _isCall(disasm)  ((disasm->Instruction.BranchType == CallType)  && !(disasm->Argument1.ArgType &REGISTER_TYPE))))*/

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
			
		}else {
			if(_isCall(disasm))
			{
				// set missing informations
				mCurrent.RVA = disasm->VirtualAddr ;
				mCurrent.RelativeAddressValue = disasm->Instruction.AddrValue ;


				// retrieve label from the instratuction at "addrValue"
				DISASM target_instr;
				int len = getInstruction(disasm->Instruction.AddrValue-disasm->VirtualAddr,&target_instr);

				if(len != UNKNOWN_OPCODE){
					// it is a jump to a dll
					if(target_instr.Instruction.Opcode == 0xFF){
						mCurrent.isApiCall = true;
						// prepare gathering labelText
						char labelText[MAX_LABEL_SIZE];
						bool hasLabel = DbgGetLabelAt(target_instr.Argument1.Memory.Displacement, SEG_DEFAULT, labelText);
						if(hasLabel){
							// we have a label from TitanEngine --> look up function header in database
							FunctionInfo_t f; 
							f = mParent->db()->find(labelText);
							if(!f.invalid){
								// found!
								// yeah we know everything about the api-call!
								std::string functionComment;
								functionComment = f.ReturnType + " " + f.Name + "(...)";
								DbgSetCommentAt(disasm->VirtualAddr ,functionComment.c_str());
								int pos=0;
								// set comments for the arguments
								if( (f.Arguments.size() > 0)  && (mCurrent.arguments.size() > 0) ){
									for(std::list<duint>::reverse_iterator a= mCurrent.arguments.rbegin();a!=mCurrent.arguments.rend();a++)
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

						}

					}

				}
				

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
		if(c->isApiCall)
			numberOfApiCalls++;
	}

	_plugin_logprintf("[StaticAnalysis:IntermodularCalls] of which are %i intermodular calls\n", numberOfApiCalls);


	return true;
}

void S_IntermodularCalls::unknownOpCode(const  DISASM* disasm )
{
	// current instruction wasn't correctly disassembled, so assuming worst-case
	mCurrent.clear();
}

