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
#ifndef ABSTRACTSOCKET_H_INCLUDED
#define ABSTRACTSOCKET_H_INCLUDED

//System includes
//#include <unistd.h>
//#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

//Library includes
#include <string>

//Project includes
#include "Address.h"

//Included here for now to eliminate the need for modules consuming socket
// headers to always include the associated exception class definition.
#include "SocketException.h"

//Define constants
//TODO Replace this with constexpr within namespace
#ifndef INVALID_SOCKET
#define INVALID_SOCKET		0
#endif //INVALID_SOCKET

//Namespace container
namespace Inet {

/*! Type definitions for structs used by the socket API */
typedef int														socket_t;

typedef socklen_t											SockLen_t;
typedef SockLen_t*										SockLenPtr_t;

/*!	@brief Abstract base class for network sockets
 *	@author James.A.Cleland@gmail.com
 */
class AbstractSocket {
public:
	/*!	@brief Default constructor */
	AbstractSocket();

	/*!	@brief Default destructor */
	virtual ~AbstractSocket();

	// No default copy constructor or assignment - use move instead
	AbstractSocket(const AbstractSocket &other) = delete;
	AbstractSocket &operator=(const AbstractSocket &other) = delete;

	/*!	@brief Move constructor
	 *	@param other Object to copy from
	 */
	AbstractSocket(AbstractSocket &&other) noexcept;

	/*!	@brief Move assignment operator
	 *	@param other object to assign from
	 *	@return Reference to this instance
	 */
	AbstractSocket& operator=(AbstractSocket &&other) noexcept;

protected:
	/*!< Handle to the internal socket */
	socket_t				socket_ = INVALID_SOCKET;
};

}; //Inet namespace

#endif //ABSTRACTSOCKET_H_INCLUDED
