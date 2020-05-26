/*!
 *
 *	The latest source code can be downloaded at:
 *
 *	Copyright (c) 2020, James A. Cleland <jcleland at jamescleland dot com>
 *
 *	This program is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program.  If not, see <http://www.gnu.org/licenses/>.
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
#include "ServerSocket.h"
#include "SocketException.h"

//Namespace container
namespace Inet {

/*!	@brief Platform-specific getter for errno */
inline int LastError() {
	return errno;
};

/*!	@brief Default Constructor Implementation
 *	@todo Need to initialize members that apply
 */
ServerSocket::ServerSocket() {
}

/*!	@brief Destructor */
ServerSocket::~ServerSocket() {
}

/*!	@brief Move constructor
 *	@param other Const ServerSocket reference rvalue to move from
 */
ServerSocket::ServerSocket(ServerSocket &&other) noexcept {
	//Call move assignment operator
	*this = std::move(other);
}

/*!	@brief Move assignment operator
 *	@param other Const ServerSocket reference rvalue to move from
 *	@return A reference to this instance
 */
ServerSocket& ServerSocket::operator=(ServerSocket &&other) noexcept {
	//Don't allow self-assignment
	if(this != &other) {
		//Call base class move
		AbstractSocket::operator=(std::move(other));
	}

	//Return self ref
	return *this;
}


/*!	@brief
 *	@param
 */
void ServerSocket::bind(const uint16_t port) {
	//Create sockaddr struct to use for local inet socket address
	SockAddrIn_t saddr;

	//Create socket
	createSocket();

	//Initialize socket address structure for server socket bind,
	// sockaddr_in family is always AF_INET
	memset(&saddr, 0, sizeof(SockAddrIn_t));
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(port);
	if(::bind(socket_, (const SockAddrPtr_t)&saddr, sizeof(SockAddrIn_t)) != 0) {
		//Clean up socket and throw error
		destroySocket();
		throw SocketException(errno, std::string("Error binding socket to port"));
	}
}

/*!	@brief
 *	@param
 */
void ServerSocket::listen(const int backlog) {
	//TODO Validate socket
	//TODO Validate backlog
	//Set backlog for server socket
	if(::listen(socket_, backlog) < 0) {
		throw SocketException(errno, std::string("Error setting server socket backlog"));
	}
}

/*!	@brief
 *	@param
 */
ConnectionEndpoint ServerSocket::accept() {
	//Declare local
	SockAddrIn_t address;
	int addrLen = sizeof(SockAddr_t);
	socket_t client = INVALID_SOCKET;

	client = ::accept(socket_, (SockAddrPtr_t)&address, (SockLenPtr_t)&addrLen);
	if(client < 0) {
		throw SocketException(errno, std::string("Accept failed for server socket"));
	}

	//Create endpoint from client socket and address
	return std::move(ConnectionEndpoint(client, &address, addrLen));
}

/*!	@brief Close the server socket
 */
void ServerSocket::close() {
	//Close socket if open
	destroySocket();
}

/*!	@brief
 *	@param
 */
void ServerSocket::createSocket() {
	//Declare locals
	int opt = 1;
	int retcode = 0;

	//Close socket if open
	destroySocket();

	//Create socket
	socket_ = ::socket(AF_INET, SOCK_STREAM, 0);
	if(socket_ == INVALID_SOCKET) {
		throw SocketException(socket_, "Error creating socket");
	}

	//Set socket options
	//TODO Get socket options from new method or constructor
	retcode = ::setsockopt(socket_, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
		&opt, sizeof(opt));
	if(retcode < 0) {
		//Clean up socket and throw error
		destroySocket();
		throw SocketException(socket_, "Error setting socket options");
	}
}

/*!	@brief
 *	@param
 */
void ServerSocket::destroySocket() {
	if(socket_ != INVALID_SOCKET)
		::close(socket_);
	socket_ = INVALID_SOCKET;
}

}; //Inet namespace
