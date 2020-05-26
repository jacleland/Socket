/*!
 *  The latest source code can be downloaded at:
 *
 *  Copyright (c) 2020, James A. Cleland <jcleland at jamescleland dot com>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

//System includes
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>

//Library includes
#include <utility>
#include <exception>
#include <iostream>
#include <cstring>

//Project includes
#include "appcommon.h"
#include "Address.h"
#include "ServerSocket.h"

using namespace Inet;

//Forward decl funcs
void GetArgs(int argc, char **argv);

//Globals
uint16_t port					= 30100; //TODO: Fix this so client and server take similar types
bool blocking					= true;
bool oneshot 					= true;

/*!	@brief
 *	@return Non-zero return value on application error
 */
int main(int argc, char *argv[]) {
	//Declare locals for received bytes and message buffer size
	int32_t len = 0;

	try {
		//Process command line
		GetArgs(argc, argv);

		//Output message
		std::cout << "Listening on port " << port;
		if(!blocking) std::cout << " using non-blocking socket";
		std::cout << "." << std::endl;

		//Create the server socket and bind. Set listen backlog and socket opts as reqd
		ServerSocket *pSock = new ServerSocket();
		pSock->bind(port);
		pSock->listen(12);

		do {
			//Buffer stored using unique ptr and sizes
			std::unique_ptr<char[]> buffer;
			uint32_t buflen = 0;

			//Accept connection
			ConnectionEndpoint &&client = pSock->accept();

			//Receive messages from the client and echo them back until connection closed
			bool connected = true;
			do {
				//Receive client message data
				uint32_t chunks = 0;
				connected = ReceiveMessage(client, buffer, buflen, chunks);
				if(connected) {
					//Output message
					std::cout << "Message length: " << buflen << " bytes in " <<
					chunks << " chunk(s), echoing...";

					//Echo reply to client
					SendMessage(client, buffer, buflen);
					std::cout << " Done." << std::endl;
				}
			} while(connected); //Continue until socket close by client

			//Output message
			std::cout << "Peer disconnected." << std::endl;

			//Clean up the endpoint
			client.close();
		} while(!oneshot);

		pSock->close();

		delete pSock;
	}
	catch(SocketException &se) {
		std::cout << se.what() << " (" << se.code() << ")" << std::endl;
	}

	return 0;
}

/*!	@brief Process command line arguments
 * 	@param argc As passed to main()
 * 	@param argv As passed to main()
 */
void GetArgs(int argc, char **argv) {
	//Declare locals
	char c;

	//Iterate over arguments
	while((c = getopt(argc, argv, "p:nfh")) != -1) {
		switch(c) {
			//Get port to use for connect address
			case 'p':
				if(strlen(optarg) > 0) port = atol(optarg);
				break;

			//Set non-blocking socket
			case 'n':
				blocking = false;
				break;

			//Let's the server stay alive between clients - defaults to one-shot then close
			case 'f':
				oneshot = false;
				break;

			//Help or any other arg will land here
			//TODO: Implement exception throw/help text w/usage
			case 'h':
			default:
				std::cout << "Oops" << std::endl;
		}
	}
}

