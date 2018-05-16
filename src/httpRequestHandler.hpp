#pragma once

#include "httpMessage.hpp"

// Global macro
#define IMPLEMENT_HTTP_REQUEST_HANDLER(name) \
	void name ( \
			const http::RequestMessage & requestMessage , \
			http::ResponseMessage & responseMessage \
		)

namespace http
	{
		namespace requestHandler
			{
				IMPLEMENT_HTTP_REQUEST_HANDLER( GET_TEST_PAGE ) ;
			}
	}

// ACTUAL IMPLEMENTATION BELOW

IMPLEMENT_HTTP_REQUEST_HANDLER( http::requestHandler::GET_TEST_PAGE )
	{
		const auto & target = requestMessage.header.requestTarget ;

		responseMessage.payload =
			" Hello from Arduino ! \n"
			" Test GET Request Handler written by Giuseppe Masino\n"
			"\n"
			" The request target was " + target + "\n\n\n" ;
	}
