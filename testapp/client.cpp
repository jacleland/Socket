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
#include <string.h>

//Library includes
#include <utility>
#include <exception>
#include <iostream>

//Project includes
#include "appcommon.h"
#include "ClientSocket.h"

using namespace Inet;

//Forward decl funcs
void GetArgs(int argc, char **argv);

//Globals
std::string hostname 	= HOSTNAME;
std::string port 			= PORT;
uint32_t msgSize 			= MSGSIZE;
uint16_t msgCount 		= MSGCOUNT;
bool blocking					= true;

/*!	@brief Echo client - sends and receives a message from echo server
 *	@param argc Command line argument count
 *	@param argv Pointer to array of command line arguments
 *	@return Non-zero return value on application error
 */
int main(int argc, char *argv[]) {
	//Declare locals for message buffer
	uint32_t recvbytes = 0;

	try {
		//Process command line
		GetArgs(argc, argv);

		//Output message
		std::cout << "Sending " << msgCount << " messages of " << msgSize << " bytes";
		if(!blocking) std::cout << " using non-blocking socket";
		std::cout << "." << std::endl;

		//Get host address for server
		std::unique_ptr<Address> upAddr = Address::get(hostname.c_str(), port.c_str());

		//Output message
		std::cout << "Connecting to host " << hostname << " on port " << port << "..." << std::endl;

		//Create a client socket instance and connect to first returned address
		ClientSocket *client = new ClientSocket();
		client->connect(*(upAddr.get()));

		//Build message buffers
		std::unique_ptr<char[]> buffer(new char[msgSize]);
		memset((char*)&buffer[0], 'A', msgSize);
		std::unique_ptr<char[]> recvbuf(new char[msgSize]);

		//Loop for send/receive
		for(int i = 0; i < msgCount; i++) {
			//Output message and send data to echo server
			std::cout << "Sending " << msgSize << "-byte message to server... ";
			SendMessage(*client, buffer, msgSize);

			//Chunk counter
			uint32_t chunks = 0;
			std::cout << "Receiving echo reply, ";
			ReceiveMessage(*client, recvbuf, recvbytes, chunks);

			//Output message
			std::cout << "read " << chunks << " chunk(s) of data: ";

			//Compare messages
			if(memcmp(&buffer[0], &recvbuf[0], msgSize) == 0)
				std::cout << "VALID" << std::endl;
			else
				std::cout << "INVALID" << std:: endl;
		}

		//Output message
		std::cout << "Done." << std::endl;

		//Close the client socket and delete
		client->close();
		delete client;
	}
	catch(AddressException &ae) {
		std::cout << ae.what() << " (" << ae.code() << ")" << std::endl;
	}
	catch(SocketException &se) {
		std::cout << se.what() << " (" << se.code() << ")" << std::endl;
	}

	return 0;
}

/*!	@brief Process command line arguments
 * 	@param argc As passed to main()
 * 	@param argv As passed to main()
 *	@return Reference to a const ArgMap_t containing args and values from the command line
 */
void GetArgs(int argc, char **argv) {
	//Declare locals
	char c;

	//Iterate over arguments
	while((c = getopt(argc, argv, "H:p:s:c:n")) != -1) {
		switch(c) {
			//Get hostname to use for connect address
			case 'H':
				if(strlen(optarg) > 0) hostname = optarg;
				break;

			//Get port to use for connect address
			case 'p':
				if(strlen(optarg) > 0) port = optarg;
				break;

			//Set the size of each message
			case 's':
				if(strlen(optarg) > 0) {
					msgSize = atol(optarg);
				}
				break;

			//Set number of messages to send
			case 'c':
				if(strlen(optarg) > 0) {
					msgCount = atoi(optarg);
				}
				break;

			//Set non-blocking socket
			case 'n':
				blocking = false;
				break;

			//Help or any other arg will land here
			//TODO: Implement exception throw/help text w/usage
			case 'h':
			default:
				std::cout << "Oops" << std::endl;
		}
	}
}
