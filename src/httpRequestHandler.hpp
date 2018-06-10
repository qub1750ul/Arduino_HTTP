/**
 * @file
 * @brief Header file containing default HTTP request handlers
 */

#pragma once

#include "httpMessage.hpp"

/**
 * Helper macro that simplifies the creation of a request handler
 *
 * @param name The name of the handler with eventual namespace scoping
 */
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
		// Do nothing, as the default response message already has
		// the default header and empty payload
	}

IMPLEMENT_HTTP_REQUEST_HANDLER( http::requestHandler::returnTestPage )
	{
		const auto & target = requestMessage.header.requestTarget ;

		responseMessage.payload =
			" Hello from Arduino ! \n"
			"\n"
			" The request target was " + target + "\n"
			"\n"
			" This test page is offered by the returnTestPage Handler\n"
			" part of the Arduino_HTTP/1.1 library by qub1750ul\n"
			"\n"
			" https://github.com/qub1750ul/Arduino_HTTP\n\n" ;
	}
