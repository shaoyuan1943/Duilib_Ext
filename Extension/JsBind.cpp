#include "stdafx.h"
#include "JsBind.h"

void JsBind::WebBorwserInit()
{
	wkeInit();
}

void JsBind::WebBorwserShutDown()
{
	wkeShutdown();
}

const wchar_t* JsBind::ToString(const Cast& cast)
{
	assert(cast.es);
	return jsToStringW(cast.es, cast.v);
}

int JsBind::ToInt(const Cast& cast)
{
	assert(cast.es);
	return jsToInt(cast.es, cast.v);
}

bool JsBind::ToBool(const Cast& cast)
{
	assert(cast.es);
	return jsToBoolean(cast.es, cast.v);
}

float JsBind::ToFloat(const Cast& cast)
{
	assert(cast.es);
	return jsToFloat(cast.es, cast.v);
}

double JsBind::ToDouble(const Cast& cast)
{
	assert(cast.es);
	return jsToDouble(cast.es, cast.v);
}


jsValue JsBind::ToJsInt(int n)
{
	return jsInt(n);
}

jsValue JsBind::ToJsFloat(float f)
{
	return jsFloat(f);
}

jsValue JsBind::ToJsDouble(double d)
{
	return jsDouble(d);
}

jsValue JsBind::ToJsString(jsExecState es, const wchar_t* str)
{
	assert(es);
	return jsStringW(es, str);
}

void JsBind::JsBindFunction(const Functor& func)
{
	assert(func.funcName);
	assert(func.fn);
	jsBindFunction(func.funcName, func.fn, func.argCount);
}

jsType JsBind::ArgType(const Param& pm)
{
	assert(pm.es);
	return jsArgType(pm.es, pm.argIdx);	
}

jsValue JsBind::Arg(const Param& pm)
{
	assert(pm.es);
	return jsArg(pm.es, pm.argIdx);
}

jsValue JsBind::Undefined()
{
	return jsUndefined();
}

jsValue JsBind::CallJsGlobalFunction(const JsCall& call)
{
	assert(call.es);
	assert(call.func);
	return jsCallGlobal(call.es, call.func, call.args, call.argCount);
}

jsValue JsBind::GetJsGlobalObjectByName(const JsObject& object)
{
	assert(object.es);
	assert(object.objectName);
	return jsGetGlobal(object.es, object.objectName);
}

jsValue JsBind::GetJsGlobalObject(jsExecState es)
{
	assert(es);
	return jsGlobalObject(es);
}