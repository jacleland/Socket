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
#ifndef SOCKET_H_INCLUDED
#define SOCKET_H_INCLUDED

//System includes
#include <sys/types.h>
#include <sys/socket.h>

//Library includes
#include <string>

//Project includes
#include "AbstractSocket.h"
#include "ClientSocket.h"

//Namespace container
namespace Inet {

/*	@brief
 *	@author James.A.Cleland@gmail.com
 */
class ServerSocket : public AbstractSocket {
public:
	/*!	@brief Default constructor */
	ServerSocket();

	/*!	@brief Default destructor */
	virtual ~ServerSocket();

	ServerSocket(const ServerSocket &other) = delete;
	ServerSocket &operator=(const ServerSocket &other) = delete;

	/*!	@brief Move constructor
		*	@param other Object to copy from
		*/
	ServerSocket(ServerSocket &&other) noexcept;


	/*!	@brief Move assignment operator
		*	@param other object to assign from
		*	@return Reference to this instance
		*/
	ServerSocket& operator=(ServerSocket &&other) noexcept;

	/*!	@brief
	 *	@param
	 */
	void bind(const uint16_t port);

	/*!	@brief
	 *	@param
	 */
	void listen(const int backlog);

	/*!	@brief
	 *	@param
	 */
	ConnectionEndpoint accept();

	/*!	@brief Close the server socket
	 */
	void close();

protected:
	/*!	@brief
	 *	@param
	 */
	void createSocket();

	/*!	@brief
	 *	@param
	 */
	void destroySocket();
};

}; //Inet namespace

#endif //SOCKET_H_INCLUDED
