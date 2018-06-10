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
				IMPLEMENT_HTTP_REQUEST_HANDLER( returnDefaultHeader ) ; ///< Default HEAD handler
				IMPLEMENT_HTTP_REQUEST_HANDLER( returnTestPage			) ; ///< Default GET  handler
			}
	}

// ACTUAL IMPLEMENTATION BELOW

IMPLEMENT_HTTP_REQUEST_HANDLER( http::requestHandler::returnDefaultHeader )
	{
		// Do nothing, as the default response message has the default header and
		// empty payload
	}

IMPLEMENT_HTTP_REQUEST_HANDLER( http::requestHandler::returnTestPage )
	{
		const auto & target = requestMessage.header.requestTarget ;

		responseMessage.payload =
			" Hello from Arduino ! \n"
			"\n"
			" The request target was " + target + "\n"
			"\n"
			" This test page is offered by the testPage Handler\n"
			" part of the Arduino_HTTP/1.1 library by qub1750ul\n"
			"\n"
			" https://github.com/qub1750ul/Arduino_HTTP\n\n" ;
	}
