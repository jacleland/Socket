
/*!
 *
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

//Include once
#ifndef CONNECTIONENDPOINT_H_INCLUDED
#define CONNECTIONENDPOINT_H_INCLUDED

//System includes
#include <sys/types.h>
#include <sys/socket.h>

//Library includes
#include <string>

//Project includes
#include "AbstractSocket.h"

//Namespace container
namespace Inet {

//Required for server socket friend relationship
class ServerSocket;

/*	@brief
 *	@author James.A.Cleland@gmail.com
 */
class ConnectionEndpoint : public AbstractSocket {
	friend ServerSocket;

protected:
	ConnectionEndpoint(socket_t sock, const SockAddrInPtr_t pAddr, int addrLen);

public:
	/*!	@brief Default constructor */
	ConnectionEndpoint();

	/*!	@brief Default destructor */
	virtual ~ConnectionEndpoint();

	//No default copy constructor or assignment - use move instead
	ConnectionEndpoint(const ConnectionEndpoint &other) = delete;
	ConnectionEndpoint &operator=(const ConnectionEndpoint &other) = delete;

	/*!	@brief Move constructor
	 *	@param other Object to copy from
	 */
	ConnectionEndpoint(ConnectionEndpoint &&other) noexcept;

	/*!	@brief Move assignment operator
	 *	@param other object to assign from
	 *	@return Reference to this instance
	 */
	ConnectionEndpoint& operator=(ConnectionEndpoint &&other) noexcept;

	/*!	@brief Send data to the connected peer
	 *	@todo This method is a placeholder, meant to behave in a manner
	 *				identical to the runtime library equivilent.
	 */
	virtual int send(const char* buf, int len);

	/*!	@brief Receive data from the connected peer
	 *	@todo This method is a placeholder, meant to behave in a manner
	 *				identical to the runtime library equivilent.
	 */
	virtual int receive(char *buf, int len);

	/*!	@brief Closes the connection
	 */
	virtual void close();

protected:
	/*!	@brief Checks to see if the socket is closed using select
	 *	@return True if the socket has been closed by the peer, false if not
	 */
	bool closed();

protected:
	SockAddrIn_t		peerAddress_;
	bool						blocking_ = true;
};

}; //Inet namespace

#endif //CONNECTIONENDPOINT_H_INCLUDED
