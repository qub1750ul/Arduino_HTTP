#include "httpHeader.hpp"

const String requestMethodString[]
	{
		"INVALID" , // Internally used value
		"OPTIONS" ,
		"GET"			,
		"HEAD"		,
		"POST"		,
		"PUT"			,
		"DELETE"	,
		"TRACE"		,
		"CONNECT"
	} ;

const String responseString[]
	{
		"100 CONTINUE" ,
		"101 SWITCHING PROTOCOLS ",
		"102 PROCESSING ",

		"200 OK" ,
		"201 CREATED ",
		"202 ACCEPTED ",
		"203 UNOFFICIAL INFO ",
		"204 NO CONTENT ",
		"205 RESET CONTENT ",
		"206 PARTIAL CONTENT ",
		"207 MULTI STATUS ",

		"300 MULTIPLE CHOICES " ,
		"301 MOVED PERMANENTLY ",
		"302 FOUND ",
		"303 SEE OTHER ",
		"304 NOT MODIFIED ",
		"305 USE PROXY ",
		"306 TEMPORARY REDIRECT ",
		"307 PERMANENT REDIRECT ",

		"400 BAD REQUEST " ,
		"401 UNAUTHORIZED " ,
		"402 PAYMENT REQUIRED " ,
		"403 FORBIDDEN " ,
		"404 NOT FOUND " ,
		"405 METHOD NOT ALLOWED " ,
		"406 NOT ACCEPTABLE ",
		"407 PROXY AUTH REQUIRED ",
		"408 REQUEST TIMEOUT ",
		"409 CONFLICT ",
		"410 GONE ",
		"411 LENGHT REQUIRED ",
		"412 PRECONDITION FAILED ",
		"413 ENTITY TOO LARGE ",
		"414 URI TOO LONG ",
		"415 UNSUPPORTED MEDIA TYPE ",
		"416 REQUESTED RANGE NOT SATISFIABLE ",
		"417 EXPECTATION FAILED ",
		"418 I AM A TEAPOT ",

		"420 ENHACE YOUR CALM" ,
		"422 UNPROCESSABLE ENTITY" ,
		"426 UPGRADE REQUIRED" ,
		"429 RETRY WITH" ,
		"451 UNAVAILABLE FOR LEGAL REASONS"
	} ;

http::Request_t::operator String() const
	{
		return requestMethodString[ (uint8_t) requestId ] ;
	}

http::Request_t::operator Request() const
{
	return requestId ;
}

http::Request http::Request_t::operator = ( const Request & newRequest )
	{
		requestId = newRequest ;
		return * this ;
	}

http::Response_t::operator String() const
	{
		return responseString[ ( uint8_t ) responseId ] ;
	}

http::Response_t::operator Response() const
	{
		return responseId ;
	}

/**
 * Main serialization function for HTTP header classes
 * Here is where much of the job is done
 *
 * @param fieldN			numeber of fields available in the HTTP header
 * @param fieldArray	pointer to the fieldArray member of the HTTP header object
 */
String headerToString( const uint8_t & fieldN, const http::Field * const * fieldArray )
	{
		// String that holds the serialized header
		String header ;

		/* NOTE:
		 * pragma is a special field that is used to store all the actual pragma paramaters
		 * It is known to be the first element of fieldArray
		 * This reference is made for convenience
		 *
		 * As it needs to be processed in a different way than other fields,
		 * all loops that iterate over fields start at index 1 to not touch it
		 */
		const http::Field & pragma = * ( fieldArray[ 0 ] ) ;

		// Local function that serializes the header field at index n of fieldArray
		auto serializeField = [ & ]( const uint8_t n )
			{
				const http::Field & field = * ( fieldArray[ n ] ) ;

				return field.value.length() > 0 ?
					field.tag + " " + field.value + "\n" : "" ;
			} ;

		// Begin header serialization

		/* NOTE:
		 * The first row serialization method changes if this
		 * is for a response or a request, so delegate this
		 * to the proper function overloads.
		 *
		 * Instead, the other fields will be serialized here
		 *
		 * NOTE2:
		 * The serializeField function is used only in the following loop
		 * but in this way the whole thing is more expressive
		 */
		for( auto n = 1 ; n < fieldN ; ++ n )
			header += serializeField( n ) ;

		// Serialize and append pragma parameters to the header
		{
			uint8_t subStrCount = 0 ;
			uint8_t newStrCount = 0 ;

			/* NOTE: ( pragma field structure )
			 * The pragma field value is a concatenation of the values of
			 * all the actual pragma fields, each terminated with a \n
			 */

			// Count the \n terminated substrings and create an array that can hold them
			for( auto ptr = pragma.value.begin() ; ptr < pragma.value.end() ; ++ptr )
				if( * ptr == '\n' ) ++subStrCount ;

			String pragmaArray[ subStrCount ] ;

			// Extract the individual pragma field values
			for( auto ptr = pragma.value.begin() ; ptr < pragma.value.end() ; ++ptr )
				{
					pragmaArray[ newStrCount ] += *ptr ;
					if( *ptr == '\n' ) ++newStrCount ;
				}

			// Once extraction is complete, serialize them all
			for( newStrCount = 0 ; newStrCount < subStrCount ; ++newStrCount )
				header += pragma.tag + pragmaArray[ newStrCount ] ;
		}

		// Field serialization complete
		// Append header termination sequence
		header += "\r\n" ;

		return header ;
	}

http::ResponseHeader & http::ResponseHeader::operator = ( const ResponseHeader & other )
	{
		for( auto n = 0 ; n < fieldN ; ++ n )
			fieldArray[ n ]->value = other.fieldArray[ n ]->value ;

		return * this ;
	}

/// Serialization method for HTTP response header classes
http::ResponseHeader::operator String() const
	{
		String partialHeader = headerToString( fieldN, fieldArray ) ;
		String response = "HTTP/" + version + " " + ( String ) responseCode + "\n" ;

		return response + partialHeader ;
	}

/// Serialization method for HTTP request header classes
http::RequestHeader::operator String() const
	{
		String partialHeader = headerToString( fieldN, fieldArray ) ;
		String request =
			( String ) requestMethod + " " + requestTarget + " " + version + "\n" ;

		return request + partialHeader ;
	}

http::RequestHeader & http::RequestHeader::operator = ( const RequestHeader & other )
	{
		for( auto n = 0 ; n < fieldN ; ++ n )
			fieldArray[ n ]->value = other.fieldArray[ n ]->value ;

		return * this ;
	}
