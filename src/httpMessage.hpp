#pragma once

#include "httpHeader.hpp"

namespace http
	{
		struct RequestMessage ;
		struct ResponseMessage ;
	}

struct http::RequestMessage
	{
		RequestHeader header ;
		String payload ;
		bool parsingFailed = false ;
	} ;

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
