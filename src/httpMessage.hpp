/**
 * @file
 * @brief Header file containing HTTP message related structures and functions
 */

#pragma once

#include "httpHeader.hpp"

namespace http
	{
		struct RequestMessage ;
		struct ResponseMessage ;
	}

/// Struct representing a whole HTTP request message
struct http::RequestMessage
	{
		RequestHeader header ;
		String payload ;
		bool parsingFailed = false ;
	} ;

/// Struct representing a whole HTTP response message
struct http::ResponseMessage
	{
		ResponseMessage(
				const ResponseHeader & header_ ,
				const String & payload_ = "" ,
				const bool & parsingFailed_ = false
			) :
			header( header_ ), payload( payload_ ), parsingFailed( parsingFailed_ )
			{} ;

		ResponseHeader header ;
		String payload ;
		bool parsingFailed ;

		operator String() const
			{
				return ( String ) header + payload + "\n" ;
			}
	} ;
