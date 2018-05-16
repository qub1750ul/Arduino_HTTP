#pragma once

#include "httpMessage.hpp"

namespace http
	{
		template < class Client_t >
			class RemoteClient ;
	}

template < class Client_t >
class http::RemoteClient
	{
		public:

			RemoteClient( const Client_t & newClient ) :
				client( newClient ) {} ;

			inline void write( const ResponseMessage & message ) const
				{
					client.write( String( message ).c_str() ) ;
				}

			inline char read() const
				{
					return client.read() ;
				}

			inline bool available() const
				{
					return client.available() ;
				}

			inline void close() const
				{
					client.stop() ;
				}

			inline RemoteClient operator = ( const Client_t & newClient )
				{
					client = newClient ;
					return * this ;
				}

		private:

			Client_t client ;
	} ;
