/**
 * @file
 * @brief Header file containing HTTP header related structures and functions
 */

#pragma once

#include <Arduino.h>

namespace http
	{
		enum class Request ;
		enum class Response ;
		class Request_t ;
		class Response_t ;
		struct Field ;
		struct ResponseHeader ;
		struct RequestHeader ;
	}

/**
 * Enum class representing a HTTP request method
 * It is intended to be used only as an assignment parameter for http::Request_t
 */
enum class http::Request
	{
		INVALID = 0 , // Internally used value
		OPTIONS ,
		GET			,
		HEAD		,
		POST		,
		PUT			,
		DELETE	,
		TRACE		,
		CONNECT
	} ;

/**
 * Enum class representing a HTTP response code
 * It is intended to be used only as an assignment parameter for http::Request_t
 */
enum class http::Response
	{
		// Informational
		CONTINUE = 0 ,
		SWITCHING_PROTOCOLS ,
		PROCESSING ,

		// Successful
		OK ,
		CREATED ,
		ACCEPTED ,
		UNOFFICIAL_INFO ,
		NO_CONTENT ,
		RESET_CONTENT ,
		PARTIAL_CONTENT ,
		MULTI_STATUS ,

		// Redirection
		MULTIPLE_CHOICES ,
		MOVED_PERMANENTLY ,
		FOUND ,
		SEE_OTHER ,
		NOT_MODIFIED ,
		USE_PROXY ,
		TEMPORARY_REDIRECT ,
		PERMANENT_REDIRECT ,

		// Client error
		BAD_REQUEST ,
		UNAUTHORIZED ,
		PAYMENT_REQUIRED ,
		FORBIDDEN ,
		NOT_FOUND ,
		METHOD_NOT_ALLOWED ,
		NOT_ACCEPTABLE ,
		PROXY_AUTH_REQUIRED ,
		REQUEST_TIMEOUT ,
		CONFLICT ,
		GONE ,
		LENGHT_REQUIRED ,
		PRECONDITION_FAILED ,
		ENTITY_TOO_LARGE ,
		URI_TOO_LONG ,
		UNSUPPORTED_MEDIA_TYPE ,
		REQUESTED_RANGE_NOT_SATISFIABLE ,
		EXPECTATION_FAILED ,
		I_AM_A_TEAPOT ,

		// Server Error
		ENHACE_YOUR_CALM ,
		UNPROCESSABLE_ENTITY ,
		UPGRADE_REQUIRED ,
		RETRY_WITH ,
		UNAVAILABLE_FOR_LEGAL_REASONS
	} ;

/**
 * Wrapper class for http::Request
 * it provides a strong type for representing a HTTP request method
 * in addition to conversion operators to String and to enum type
 */
class http::Request_t
	{
		public:

			Request_t( const Request & request ) :
				requestId( request ) {} ;

			operator String() const ;
			operator Request() const ;
			Request operator = ( const Request & ) ;

		private:

			Request requestId = Request::INVALID ;
	} ;

/**
 * Wrapper class for http::Response
 * it provides a strong type for representing a HTTP response code
 * in addition to conversion operators to String and to enum type
 */
class http::Response_t
	{
		public:

			Response_t( const Response & response ) :
				responseId( response ) {} ;

			Response_t operator = ( const Response & newResponse )
				{
					responseId = newResponse ;
					return * this ;
				}

			operator String() const ;
			operator Response() const ;

		private:

			Response responseId ;
	} ;

/**
 * A http header field
 * This struct is intended to use only in http header classes
 *
 * The tag can only be defined at the time of the instantiation,
 * while the value can be assigned at any time by the assignment operator
 */
struct http::Field
	{
		explicit Field( const String & tag_ ) : tag( tag_ ) {}

		Field( const String & tag_, const String & value_ ) :
			tag( tag_ ), value( value_ ) {}

		Field operator = ( const String & newValue )
			{
				value = newValue ;
				return * this ;
			}

		const String tag ;
		String value ;
	} ;

/**
 * A struct representing a HTTP response header
 * It can be serialized by conversion to String
 */
struct http::ResponseHeader
	{
		ResponseHeader & operator = ( const ResponseHeader & ) ;

		String version ;
		Response_t responseCode = Response::OK ;

		Field pragma						{ "Pragma:"							} ;

		// Control data fields
		Field age								{ "Age:"								} ;
		Field cacheControl			{ "Cache-Control:"			} ;
		Field expires						{ "Expires:"						} ;
		Field date							{ "Date:"								} ;
		Field location					{ "Location:"						} ;
		Field retryAfter				{ "Retry-After:"				} ;
		Field vary							{ "Vary:"								} ;
		Field warning						{ "Warning:" 						} ;

		// Validator fields
		Field ETag							{ "ETag:"								} ;
		Field lastModified			{ "Last-Modified:"			} ;

		// Authentication Challenges fields
		Field WWWAuthenticate		{ "WWW-Authenticate:"		} ;
		Field proxyAuthenticate	{ "Proxy-Authenticate:"	} ;

		// Response Context fields
		Field acceptRanges			{ "Accept-Ranges:"			} ;
		Field allow							{ "Allow:"							} ;
		Field server						{ "Server:"							} ;

		Field connection				{ "Connection:"					} ;

		operator String() const ;

		/* Member that defines the size of fieldArray,
		 * that is equal to the number of fields available in the header.
		 * Also used in serialization procedures
		 */
		static constexpr uint8_t fieldN = 17 ;

		// This array allows to access header fields in a iterative way.
		// Used in serialization procedures
		const Field * const fieldArray[ fieldN ]
			{
				// pragma needs to be the first element of the array
				// to corretly execute the serialization
				& pragma ,
				& age , & cacheControl , & expires , & date , & location , & retryAfter ,
				& vary , & warning , & ETag , & lastModified , & WWWAuthenticate ,
				& proxyAuthenticate , & acceptRanges , & allow , & server , & connection
			} ;
	} ;

/**
 * A struct representing a HTTP request header
 * It can be serialized by conversion to String
 */
struct http::RequestHeader
	{
		RequestHeader & operator = ( const RequestHeader & ) ;

		Request_t requestMethod = Request::INVALID ;
		String requestTarget ;
		String version ;

		// Control fields
		Field connection					{ "Connection:"						} ;
		Field cacheControl				{ "Cache-Control:"				} ;
		Field expect							{ "Expect:"								} ;
		Field host								{ "Host:"									} ;
		Field maxForwards					{ "Max-Forwards:"					} ;
		Field pragma							{ "Pragma:"								} ;
		Field range								{ "Range:"								} ;
		Field textEncodings				{ "TE:"										} ;

		// Conditional fields
		Field ifMatch							{ "If-Match:"							} ;
		Field ifNoneMatch					{ "If-None-Match:"				} ;
		Field ifModifiedSince			{ "If-Modified-Since:"		} ;
		Field ifUnmodifiedSince		{ "If-Unmodified-Since:"	} ;
		Field ifRange							{ "If-Range:"							} ;

		// Content negotiation fields
		Field acceptedTypes				{ "Accept:"								} ;
		Field acceptedCharset			{ "Accept-Charset:"				} ;
		Field acceptedEncoding		{ "Accept-Encoding:"			} ;
		Field acceptedLanguage		{ "Accept-Language:"			} ;

		// Authentication credentials fields
		Field authorization				{ "Authorization:"				} ;
		Field proxyAuth						{ "Proxy-Authorization:"	} ;

		// Context request fields
		Field from								{ "From:"									} ;
		Field referrer						{ "Referer:"							} ;
		Field userAgent						{ "User-Agent:"						} ;

		operator String() const ;

		/* Member that defines the size of fieldArray,
		 * that is equal to the number of fields available in the header.
		 * Also used in serialization procedures
		 */
		static constexpr uint8_t fieldN = 22 ;

		// This array allows to access header fields in a iterative way.
		// Used in serialization procedures
		Field * const fieldArray[ fieldN ]
			{
				// pragma needs to be the first element of the array
				// to corretly execute the serialization
				& pragma ,
				& connection , & cacheControl , & expect , & host ,
				& maxForwards , & range , & textEncodings , & ifMatch ,
				& ifNoneMatch , & ifModifiedSince , & ifUnmodifiedSince , & ifRange ,
				& acceptedTypes , & acceptedCharset , & acceptedEncoding ,
				& acceptedLanguage , & authorization , & proxyAuth , & from ,
				& referrer , & userAgent
			} ;
	} ;
