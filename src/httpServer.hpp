#pragma once

#include "httpRemoteClient.hpp"
#include "httpRequestHandler.hpp"

namespace http
	{
		class Server ;

		template < class Client_t >
		RequestMessage parseRawMessageFrom( const http::RemoteClient < Client_t > & ) ;
	}

class http::Server
	{
		public:

			Server() ;

			template < class Client_t >
			void replyTo( const RemoteClient < Client_t > & ) ;

			ResponseHeader defaultResponseHeader ;

			// Callback functions for HTTP requests
			#define DECLARE_REQUEST_HANDLER_PTR(name) \
				IMPLEMENT_HTTP_REQUEST_HANDLER( ( * name ) ) = nullptr

			DECLARE_REQUEST_HANDLER_PTR( OPTIONS_requestHandler	) ;
			DECLARE_REQUEST_HANDLER_PTR( GET_requestHandler			) ;
			DECLARE_REQUEST_HANDLER_PTR( HEAD_requestHandler		) ;
			DECLARE_REQUEST_HANDLER_PTR( POST_requestHandler		) ;
			DECLARE_REQUEST_HANDLER_PTR( PUT_requestHandler			) ;
			DECLARE_REQUEST_HANDLER_PTR( DELETE_requestHandler	) ;
			DECLARE_REQUEST_HANDLER_PTR( TRACE_requestHandler		) ;
			DECLARE_REQUEST_HANDLER_PTR( CONNECT_requestHandler	) ;

			#undef DECLARE_REQUEST_HANDLER_PTR
	} ;

/******************
 * IMPLEMENTATION *
 ******************/

http::Server::Server()
	{
		// Set default header
		defaultResponseHeader.version			= "1.1"										;
		defaultResponseHeader.connection	= "close"									;
		defaultResponseHeader.server			= "Arduino libhttp/1.0.0"	;

		// Set default request handlers
		GET_requestHandler	= requestHandler::returnTestPage			;
		HEAD_requestHandler	= requestHandler::returnDefaultHeader	;
	}

template < class Client_t >
void http::Server::replyTo( const RemoteClient < Client_t > & client )
	{
		if( ! client.available() ) return ;

		RequestMessage inboundMessage = parseRawMessageFrom( client ) ;
		ResponseMessage responseMessage( defaultResponseHeader ) ;

		if( inboundMessage.parsingFailed )
			responseMessage.header.responseCode = Response::BAD_REQUEST ;
			else
				{
					// Extract request method from the http message
					const Request & requestMethod = inboundMessage.header.requestMethod ;

					// Select the appropriate function to handle the request
					auto handleRequest =
						requestMethod == Request::OPTIONS	? OPTIONS_requestHandler	:
						requestMethod == Request::GET			? GET_requestHandler			:
						requestMethod == Request::HEAD		? HEAD_requestHandler			:
						requestMethod == Request::POST		? POST_requestHandler			:
						requestMethod == Request::PUT			? PUT_requestHandler			:
						requestMethod == Request::DELETE	? DELETE_requestHandler		:
						requestMethod == Request::TRACE		? TRACE_requestHandler		:
						requestMethod == Request::CONNECT	? CONNECT_requestHandler	: nullptr ;

						if( handleRequest == nullptr )
							responseMessage.header.responseCode = Response::BAD_REQUEST ;
							else handleRequest( inboundMessage, responseMessage ) ;
				}

		client.write( responseMessage ) ;
		client.close() ;
	}

template < class Client_t >
http::RequestMessage http::parseRawMessageFrom( const http::RemoteClient < Client_t > & client )
	{
		RequestMessage requestMessage ;

		// Manually read the first byte to let getNextWord and accessNextField
		// work as expected
		char charBuffer = client.read() ;

		auto getNextWord = [ & ]() -> String
			{
				String word ;

				// Read from the buffer until a word-terminating char or field end is encoutered
				while( charBuffer != ' ' && charBuffer != '\n' )
					{
						word += charBuffer ;
						charBuffer = client.read() ;
					}

				// This is necessary to not get stuck on next call
				if( charBuffer == ' ' )
					charBuffer = client.read() ;

				return word ;
			} ;

		auto accessNextField = [ & ]() -> void
			{
				// Discard all chars until the next field
				while( charBuffer != '\n' )
					charBuffer = client.read() ;

				// Begin reading the new field, so getNextWord doesn't get stuck
				charBuffer = client.read() ;
			} ;

		// Parse first line
		// Parse request method
		{
			String requestMethod = getNextWord() ;

			#define ifMatchesThenAssign(x) requestMethod == ( Request_t ) x ? x

			requestMessage.header.requestMethod =
				ifMatchesThenAssign( Request::CONNECT	) :
				ifMatchesThenAssign( Request::DELETE	) :
				ifMatchesThenAssign( Request::GET			) :
				ifMatchesThenAssign( Request::HEAD		) :
				ifMatchesThenAssign( Request::OPTIONS	) :
				ifMatchesThenAssign( Request::POST		) :
				ifMatchesThenAssign( Request::PUT			) :
				ifMatchesThenAssign( Request::TRACE		) : Request::INVALID ;

			#undef ifMatches
		}

		requestMessage.header.requestTarget = getNextWord() ;
		requestMessage.header.version				= getNextWord() ;

		if( requestMessage.header.requestMethod == Request::INVALID )
			requestMessage.parsingFailed = true ;

		if( requestMessage.header.requestTarget == "" )
			requestMessage.parsingFailed = true ;

		// Parse header fields until the header-payload separator or end-of-message is encoutered
		while( charBuffer != '\r' && client.available() )
			{
				accessNextField() ;

				// Get the field name
				const String parsedTag = getNextWord() ;

				/* As HTTP/1.1 specification states, if a field starts with a space ignore it
				 *
				 * IMPLEMENTATION NOTE:
				 * Since getNextWord() stops extracting the word on every whitespace
				 * if the field begins with a whitespace the first word extracted
				 * for that field will be empty
				 */
				if( parsedTag == "" ) continue ;

				// Go through all the header fields to find one with a matching name
				// if no one is found the current field is ignored
				for( uint8_t n = 0 ; n < requestMessage.header.fieldN ; ++ n )
					{
						Field & field = * requestMessage.header.fieldArray[ n ] ;

						if( field.tag != parsedTag ) continue ;

						// Read the field value, appending eventual spaces discarded by getNextWord()
						for( String parsedValue = getNextWord() ; parsedValue != "" ; parsedValue = getNextWord() )
							field.value += parsedValue + ( charBuffer == ' ' ? " " : "" ) ;

						break ;
					}
			}

		// Read the payload
		accessNextField() ;

		/* IMPLEMENTATION NOTE :
		 * This method allows to correctly read the payload even when using
		 * keep-alive connections
		 */
		while( charBuffer != '\r' && client.available() )
			{
				const String nextWord = getNextWord() ;

				if( nextWord == "" ) accessNextField() ;
				else requestMessage.payload += nextWord + ( charBuffer == ' ' ? " " : "" ) ;
			}

		return requestMessage ;
}
