/**
 * @file
 * @brief Header file containing a log facility for the library
 */

#pragma once

#include <Arduino.h>

namespace http
	{
		class Log ;
		class LogView ;
	}

class http::Log
	{
		friend class http::LogView ;

		public:

			void operator << ( const String & newString )
				{
					if( ! enabled ) return ;

					if( addTimestamp ) buffer += String( millis() ) + "\t" ;

					buffer += newString + "\n" ;
				}

			bool enabled = false ;
			bool addTimestamp = true ;

		private:

			String buffer ;
	} ;

class http::LogView
	{
		public:

			LogView( http::Log & _log ) : log( _log ) {} ;

			operator String() const
				{
					return log.buffer ;
				}

			void clear()
				{
					log.buffer = "" ;
				}

			bool & enabled = log.enabled ;
			bool & addTimestamp = log.addTimestamp ;

		private:

			http::Log & log ;
	} ;
