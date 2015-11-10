#pragma once

#include "duktape\duktape.h"
#include <string>

#define PUSH_C_FUNCTION(fcn, nargs) \
	duk_push_c_function(ctx, fcn, nargs);\
	duk_put_prop_string(ctx, -2, #fcn);

class BaseAPI
{
public:
	BaseAPI(duk_context* ctx);
	~BaseAPI();

protected:
	int m_nStatus;
	std::string m_strError;
};