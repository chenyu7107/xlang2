#pragma once

#include <string>
#include <unordered_map>
#include "value.h"
#include "runtime.h"
#include <assert.h>

namespace X 
{ 
namespace AST 
{
class Scope
{//variables scope support, for Module and Func/Class
protected:
	std::unordered_map <std::string, int> m_Vars;
public:
	Scope()
	{
	}
	inline int GetVarNum()
	{
		return (int)m_Vars.size();
	}
	inline std::vector<std::string> GetVarNames()
	{
		std::vector<std::string> names;
		for (auto& it : m_Vars)
		{
			names.push_back(it.first);
		}
		return names;
	}
	virtual Scope* GetParentScope()= 0;
	virtual int AddOrGet(std::string& name, bool bGetOnly)
	{//Always append,no remove, so new item's index is size of m_Vars;
		auto it = m_Vars.find(name);
		if (it != m_Vars.end())
		{
			return it->second;
		}
		else if (!bGetOnly)
		{
			int idx = (int)m_Vars.size();
			m_Vars.emplace(std::make_pair(name, idx));
			return idx;
		}
		else
		{
			return -1;
		}
	}
	inline virtual bool Get(Runtime* rt, void* pContext,
		std::string& name, AST::Value& v, LValue* lValue = nullptr)
	{
		int idx = AddOrGet(name, true);
		return (idx>=0)?rt->Get(this, pContext, idx, v, lValue):false;
	}
	inline virtual bool Set(Runtime* rt, void* pContext,
		int idx, AST::Value& v)
	{
		assert(idx != -1);
		return rt->Set(this, pContext, idx, v);
	}

	inline virtual bool Get(Runtime* rt, void* pContext,
		int idx, AST::Value& v, LValue* lValue = nullptr)
	{
		return rt->Get(this, pContext, idx, v, lValue);
	}
};
}
}