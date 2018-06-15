/*
 * CustomWebServer
 *
 * Example of using the Arduino_HTTP library to make a simple web server
 * that communicates with the network over Ethernet and shows a custom
 * web page
 */

#include <Ethernet.h>
#include <httpServer.hpp>

// Set the MAC address for the Ethernet Shield
byte macAddress[ 6 ] { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED } ;

// Create a EthernetServer to manage the Ethernet connection
// Set it to work on port 80, so that web browsers can communicate with it
EthernetServer ethernetServer( 80 ) ;

// Create a HTTP Server with the default parameters
http::Server httpServer ;

/* Web servers work by responding to requests made by clients
 * When a request is received the server has to pick a way to handle it
 * Within the library these ways are associated to special handler functions
 *
 * Create a request handler called showMyWebPage
 */
IMPLEMENT_HTTP_REQUEST_HANDLER( showMyWebPage )
	{
		// There are a number of variables that can be modified within a request handler
		// The responseMessage.payload variable defines the content of the resulting web page
		responseMessage.payload = "Hi, this is a custom web page" ;
	}

void setup()
	{
		// Set the Ethernet Shield to get an automatic IP address from the network
		Ethernet.begin( macAddress ) ;

		// Show the obtained address on the serial monitor
		Serial.begin( 9600 ) ;
		Serial.println( Ethernet.localIP() ) ;

		// To visualize a web page a browser has to make a request of type GET to the server
		// So set the HTTP server to use the showMyWebPage handler to reply to GET requests
		httpServer.GET_requestHandler = showMyWebPage ;
	}

void loop()
	{
		// If a request has been made from a remote client over the Ethernet connection
		// create a RemoteClient object that represents it
		http::RemoteClient < EthernetClient > remoteClient = ethernetServer.available() ;

		// Then pass the object to the server
		httpServer.replyTo( remoteClient ) ;

		/* At this point a response has been generated and a web page will be
		 * showed to the client
		 *
		 * To view the web page type the IP address of the ethernet shield in
		 * a browser on any PC connected the same network
		 */
	}
