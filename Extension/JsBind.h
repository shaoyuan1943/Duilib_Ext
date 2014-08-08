#pragma once

#include "wke.h"
using namespace wke;

typedef struct _Cast
{
	_Cast(jsExecState e, jsValue val)
	{
		this->es = es;
		this->v = val;
	}
	_Cast()
	{}
	jsExecState es;
	jsValue v;
}Cast;

typedef struct _Functor
{
	_Functor(const char* name, jsNativeFunction fc, unsigned int count)
	{
		this->funcName = name;
		this->fn = fc;
		this->argCount = count;
	}
	_Functor()
	{}
	const char* funcName;
	jsNativeFunction fn; 
	unsigned int argCount;
}Functor;

typedef struct _Param
{
	_Param(jsExecState e, unsigned int idx)
	{
		this->es = e;
		this->argIdx = idx;
	}
	_Param()
	{}
	jsExecState es;
	unsigned int argIdx;
}Param;

typedef struct _JsCall
{
	_JsCall(jsExecState e, jsValue fn, jsValue* args, unsigned int count)
	{
		this->es = e;
		this->func = fn;
		this->args = args;
		this->argCount = count;
	}
	_JsCall()
	{}
	jsExecState es;
	jsValue func;
	jsValue* args;
	unsigned int argCount;
}JsCall;

typedef struct _JsObject
{
	_JsObject(jsExecState e, const char* name)
	{
		this->es = e;
		this->objectName = name;
	}
	_JsObject()
	{}
	jsExecState es;
	const char* objectName;
}JsObject;

class JsBind
{
public:
	// webkit init
	static void WebBorwserInit();
	static void WebBorwserShutDown();

	// to c++ type
	static const wchar_t* 	ToString(const Cast& cast);
	static int 				ToInt(const Cast& cast);
	static bool 			ToBool(const Cast& cast);
	static double 			ToDouble(const Cast& cast);
	static float 			ToFloat(const Cast& cast);

	// to js type
	static jsValue 			ToJsInt(int n);
	static jsValue 			ToJsFloat(float f);
	static jsValue 			ToJsDouble(double d);
	static jsValue 			ToJsString(jsExecState es, const wchar_t* str);

	// js invoke c++ function
	static void 			JsBindFunction(const Functor& func); 
	static jsType 			ArgType(const Param& pm);
	static jsValue 			Arg(const Param& pm);
	static jsValue 			Undefined();

	// get js object or a global js function
	static jsValue			GetJsGlobalObjectByName(const JsObject& object);
	static jsValue			GetJsGlobalObject(jsExecState es);

	// c++ invoke js function, not support js object
	static jsValue			CallJsGlobalFunction(const JsCall& call);
};

class CallParam
{
public:
	CallParam(unsigned int argsCount)
	{
		param = nullptr;
		param = new jsValue[argsCount];
		pos = 0;
	}
	~CallParam()
	{
		do 
		{
			if (param)
			{
				delete [] param;
				pos = 0;
			}
		} while (0);
	}
	void PushBack(int i)
	{
		jsValue v = JsBind::ToJsInt(i);
		param[pos] = v; 
		++pos;
	}

	void PushBack(float f)
	{
		jsValue v = JsBind::ToJsFloat(f);
		param[pos] = v;
		++pos;
	}

	void PushBack(double d)
	{
		jsValue v = JsBind::ToJsDouble(d);
		param[pos] = v;
		++pos;
	}

	void PushBack(wstring str, jsExecState es)
	{
		assert(es);
		jsValue v = JsBind::ToJsString(es, str.c_str());
		param[pos] = v;
		++pos;
	}

	void PushBack(const wchar_t* str, jsExecState es)
	{
		assert(es);
		jsValue v = JsBind::ToJsString(es, str);
		param[pos] = v;
		++pos;
	}

	jsValue* Args()
	{
		return param;
	}

	unsigned int ArgsCount()
	{
		return pos + 1;
	}

private:
	jsValue* param;
	unsigned int pos;
};