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

//Namespace container
namespace Inet {

	/*! @brief Platform-specific getter for errno */
	inline int LastError() {
		return errno;
	};

	/*!	@brief Default Constructor Implementation
	 *	@todo Need to initialize members that apply
	 */
	AbstractSocket::AbstractSocket() {
	}

	/*!	@brief Destructor */
	AbstractSocket::~AbstractSocket() {
	}

	/*!	@brief Move constructor
	 *	@param other Const Socket reference rvalue to move from
	 */
	AbstractSocket::AbstractSocket(AbstractSocket&& other) noexcept {
		//Call move assignment operator
		*this = std::move(other);
	}

	/*!	@brief Move assignment operator
	 *	@param other Const Socket reference rvalue to move from
	 *	@return A reference to this instance
	 */
	AbstractSocket& AbstractSocket::operator=(AbstractSocket&& other) noexcept {
		//Don't allow self-assignment
		if(this != &other) {
			//Call base class move
			//Move socket instance
			socket_ = std::move(other.socket_);
		}

		//Return self ref
		return *this;
	}

}; //Inet namespace
