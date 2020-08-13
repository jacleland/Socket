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

#ifndef APPCOMMON_H_INCLUDED
#define APPCOMMON_H_INCLUDED

//System includes
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>

//Library includes
#include <utility>
#include <exception>
#include <map>

//Project includes
#include "Address.h"
#include "ConnectionEndpoint.h"
#include "AddressException.h"
#include "SocketException.h"
#include "NetStream.h"

//Defaults
#define HOSTNAME	"localhost"
#define PORT			"30100"
#define MSGSIZE		1024
#define MSGCOUNT	10

//Namespace container
namespace Inet {

//Typedefs
typedef std::unique_ptr<char[]>				BufferPtr_t;


/*!	@brief Send a message to the endpoint specified
 *	@param conn The ConnectionEndpoint to which the message will be sent
 *	@param buffer The buffer containing message data
 *	@param buflen The length of data in the buffer
 *	@return True if send is successful, false on failure
 */
bool SendMessage(ConnectionEndpoint& conn, BufferPtr_t& buffer, int32_t buflen) {
	//Declare local
	uint32_t netbytes = htonl(buflen);
	int32_t len = 0;

	try {
		len = conn.send((char*)&netbytes, sizeof(uint32_t));
		len = conn.send((char*)&buffer[0], buflen);
	}
	catch(SocketException& se) {
		return false;
	}
	return true;
}

/*!	@brief Recieve a message from the endpoint, adjusting buffer size and alloc as necessary
 *	@param conn The ConnectionEndpoint from which the message will be read
 *	@param buffer The buffer to receive the message data
 *	@param buflen The bytes currently allocated at buffer, output adjusted if realloc
 *	@return True if a message was read, false if the connection was closed
 */
bool ReceiveMessage(ConnectionEndpoint& conn, BufferPtr_t& buffer, uint32_t& buflen, uint32_t& chunks) {
	//Local data
	int32_t len = 0;
	uint32_t msglen = 0;

	try {
		//Get the buffer length from first 4 bytes and byte-swap network to host
		len = conn.receive((char*)&msglen, 4);
		if(len > 0) {
			msglen = ntohl(msglen);

			//Realloc auto ptr if required, first pass recvlen == >0, buflen == 0
			if(msglen != buflen) {
				//Reset buffer and allocate new array (should only on first client message)
				buffer.reset();
				buffer = std::unique_ptr<char[]>(new char[msglen]);
				buflen = msglen;
			}

			//Receive data
			uint32_t totalbytes = 0;
			while(totalbytes < msglen) {
				len = conn.receive((char*)&buffer[totalbytes], msglen - totalbytes);
				totalbytes += len;
				chunks++;
			}
		}
	}
	catch(SocketException &clientSe) {
		//Client has gone away?
		return false;
	}

	return true;
}

}; //Inet namespace

#endif //Include once
