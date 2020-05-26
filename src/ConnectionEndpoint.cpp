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
#include "ConnectionEndpoint.h"

//Namespace container
namespace Inet {

/*!	@brief Constructor for ServerSocket taking connected socket and endpoint address
 *	@param sock
 *	@param addr
 */
ConnectionEndpoint::ConnectionEndpoint(socket_t sock, SockAddrInPtr_t pAddr, int addrLen) {
	this->socket_ = sock;
	memcpy(&peerAddress_, pAddr, addrLen);
}

ConnectionEndpoint::ConnectionEndpoint() {
}

ConnectionEndpoint::~ConnectionEndpoint() {
}

/*!	@brief Move constructor
 *	@param other Const Socket reference rvalue to move from
 */
ConnectionEndpoint::ConnectionEndpoint(ConnectionEndpoint&& other) noexcept {
	//Call move assignment operator
	*this = std::move(other);
}

/*!	@brief Move assignment operator
 *	@param other Const Socket reference rvalue to move from
 *	@return A reference to this instance
 */
ConnectionEndpoint& ConnectionEndpoint::operator=(ConnectionEndpoint&& other) noexcept {
	//Don't allow self-assignment
	if(this != &other) {
		//Call base class move
		AbstractSocket::operator=(std::move(other));
	}

	//Return self ref
	return *this;
}

/*!	@brief Send data to the connected endpoint
 *	@param buf A pointer to the buffer containing data to send
 *	@param len The number of bytes to send
 *	@return The nubmer of bytes successfully sent
 *	@throws On error sending data
 */
int ConnectionEndpoint::send(const char* buf, int len) {
	//Declare locals
	int bytes = 0;
	bytes = ::send(socket_, buf, len, 0);
	return bytes;
}

/*!	@brief
 *
 */
int ConnectionEndpoint::receive(char *buf, int len) {
	//Declare locals
	int bytes = 0;
	bytes = ::read(socket_, buf, len);

	//TODO: Handle non-blocking condition when no data present
	if(bytes == 0) {
		//Read 0 bytes on blocking socket means connection has been closed
		if(blocking_) {
			throw SocketException(-1, "Peer has closed connection");
		}
	}

	if(bytes < 0) {
		throw SocketException(errno, std::string("Recieve error: ") + gai_strerror(errno));
	}

	return bytes;
}

/*!	@brief
 *
 */
void ConnectionEndpoint::close() {
	if(socket_ != INVALID_SOCKET)
		::close(socket_);
	socket_ = INVALID_SOCKET;
}

/*!	@brief Checks to see if the socket is closed using select
 *	@return True if the socket has been closed by the peer, false if not
 */
bool ConnectionEndpoint::closed() {
	fd_set read_set;
	struct timeval timeout;

	timeout.tv_sec = 0; // Time out after a minute
	timeout.tv_usec = 1;

	FD_ZERO(&read_set);
	FD_SET(socket_, &read_set);

	return(select(socket_+1, &read_set, NULL, NULL, &timeout) < 0);
}

}; //Inet namespace
