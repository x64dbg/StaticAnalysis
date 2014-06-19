/* plugin:   (StaticAnalysis) for x64dbg <http://www.x64dbg.com>
 * author:   tr4ceflow@gmail.com <http://blog.traceflow.com>
 * license:  GLPv3
 */
#pragma once

#include "pluginsdk/bridgemain.h"
#include <vector>
#include <string>

struct APIArgument
{
	std::string Type;
	std::string Name;

	APIArgument(std::string t,std::string n){
		Type=t;
		Name = n;
	}

	APIArgument(){}
};

struct APIFunction
{
	std::string DLLName;
	std::string ReturnType;
	std::string Name;
	std::vector<APIArgument> Arguments;
	bool invalid;

	APIFunction(){
		invalid=false;
	}

	APIFunction(std::string dll,std::string ret,std::string name, std::vector<APIArgument> args){
		DLLName=dll;
		ReturnType=ret;
		Name=name;
		Arguments = args;
		invalid = false;
	}

	APIArgument arg(int i){
		return Arguments.at(i);
	}

};