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

//System includes
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <errno.h>

//Library includes
#include <utility>
#include <exception>

//Project includes
#include "AbstractSocket.h"
#include "ClientSocket.h"
#include "SocketException.h"

//Namespace container
namespace Inet {

/*! @brief Platform-specific getter for errno */
inline int LastError() {
	return errno;
};

/*! @brief Default Constructor Implementation
 *  @todo Need to initialize members that apply
 */
ClientSocket::ClientSocket() {
}

/*! @brief Destructor */
ClientSocket::~ClientSocket() {
}

/*! @brief Move constructor
 *  @param other Const ClientSocket reference rvalue to move from
 */
ClientSocket::ClientSocket(ClientSocket &&other) noexcept {
	//Call move assignment operator
	*this = std::move(other);
}

/*! @brief Move assignment operator
 *  @param other Const ClientSocket reference rvalue to move from
 *  @return A reference to this instance
 */
ClientSocket &ClientSocket::operator=(ClientSocket &&other) noexcept {
	//Don't allow self-assignment
	if(this != &other) {
		//Call base class move
		ConnectionEndpoint::operator=(std::move(other));
	}

	//Return self ref
	return *this;
}

/*!	@brief Connect to the specified endpoint address
 *	@param pAddr
 */
void ClientSocket::connect(const Address &pAddr) {
	//Declare locals
	//int16_t retcode = 0;

	//Close open socket for now
	//TODO This shouldn't happend, need to error here probably.
	if(socket_ != INVALID_SOCKET) {
		::close(socket_);
		socket_ = INVALID_SOCKET;
	}

	//Create the socket to use for connect
	//socket_ = ::socket(aip->ai_family, aip->ai_socktype, aip->ai_protocol);
	createSocket(pAddr);

	if(socket_ == INVALID_SOCKET) {
		throw SocketException(socket_, "Error creating socket");
	}

	const AddrInfoPtr_t addrInfo = (const AddrInfoPtr_t)pAddr;
	if(::connect(socket_, addrInfo->ai_addr, addrInfo->ai_addrlen) != 0) {
		destroySocket();

		throw SocketException(errno, std::string("ClientSocket::connect() failed: ") + gai_strerror(errno));
	}
}

/*!	@brief
 *	@param
 */
void ClientSocket::createSocket(const Address &pAddr) {
	//Address conversion to addrinfo
	const AddrInfo_t *aip = (const AddrInfo_t *)(pAddr);

	//Close socket if open
	destroySocket();

	//Create the socket to use for connect
	socket_ = ::socket(aip->ai_family, aip->ai_socktype, aip->ai_protocol);
	if(socket_ == INVALID_SOCKET) {
		throw SocketException(socket_, "Error creating socket");
	}
}

/*!	@brief
 *	@param
 */
void ClientSocket::destroySocket() {
	if(socket_ != INVALID_SOCKET)
		::close(socket_);
	socket_ = INVALID_SOCKET;
}

}; //Inet namespace
