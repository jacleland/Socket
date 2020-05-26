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
#ifndef CLIENTSOCKET_H_INCLUDED
#define CLIENTSOCKET_H_INCLUDED

//System includes
#include <sys/types.h>
#include <sys/socket.h>

//Library includes
#include <string>

//Project includes
#include "ConnectionEndpoint.h"

//Namespace container
namespace Inet {

/*	@brief
 *	@author James.A.Cleland@gmail.com
 */
class ClientSocket : public ConnectionEndpoint {
public:
	/*!	@brief Default constructor */
	ClientSocket();

	/*!	@brief Default destructor */
	virtual ~ClientSocket();

	// No default copy constructor or assignment - use move instead
	ClientSocket(const ClientSocket &other) = delete;
	ClientSocket &operator=(const ClientSocket &other) = delete;

	/*!	@brief Move constructor
	 *	@param other Object to copy from
	 */
	ClientSocket(ClientSocket &&other) noexcept;

	/*!	@brief Move assignment operator
	 *	@param other object to assign from
	 *	@return Reference to this instance
	 */
	ClientSocket &operator=(ClientSocket &&other) noexcept;

	/*!	@brief Connect to the specified endpoint address
	 *	@param pAddr
	 */
	void connect(const Address& pAddr);

protected:
	/*!	@brief
	 *	@param
	 */
	void createSocket(const Address &pAddr);

	/*!	@brief
	 *	@param
	 */
	void destroySocket();
};

}; //Inet namespace

#endif //CLIENTSOCKET_H_INCLUDED
