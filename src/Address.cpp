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
#include <string.h>

//Library includes
#include <memory>
#include <string>
#include <vector>

//Project includes
#include "AddressException.h"
#include "Address.h"

//Namespace container
namespace Inet {

/*! @brief Initialize class static data */
std::string Address::localhost_				= std::string("localhost");

/*!	@brief Return an address instance for a port (or service type) on the local machine
 *	@param service The service (port or service string) to use for addresses
 *	@return An address instance referencing one or more addresses for the specified host and service
 */
AddressUPtr_t Address::get(const char *service) {
	return Address::get(nullptr, service, Address::Family::IPV4, Address::Protocol::TCP);
}

/*!	@brief Return an address instance for a host and port (or service type)
 *	@param host The hostname to use for addresses
 *	@param service The service (port or service string) to use for addresses
 *	@return An address instance referencing one or more addresses for the specified host and service
 */
AddressUPtr_t Address::get(const char *host, const char *service) {
	return Address::get(host, service, Address::Family::IPV4, Address::Protocol::TCP);
}

/*!	@brief Return an address instance for service name or port
 *	@param host The host name for addresses to be retrieved
 *	@param service A service name or port to return address(es) for
 *	@param family The address family for addresses returned
 *	@param protocol The protocol specification for the address
 *	@return A pointer to an allocated Åddress instance (caller owned)
 */
AddressUPtr_t Address::get(const char* host, const char* service,
		Family family, Protocol protocol) {
	//Declare local
	int status = 0;
	AddrInfo_t hints;
	AddrInfo_t* pAddrInfo = nullptr;

	//Initialize hints
	memset(&hints, 0x00, sizeof(AddrInfo_t));
	hints.ai_family = 	(int)family;
	hints.ai_socktype = (int)protocol;
	hints.ai_flags = 0;
	hints.ai_canonname = NULL;
	hints.ai_addr = NULL;
	hints.ai_next = NULL;

	//Service param value for getaddrinfo is nullptr on port string len<0
	//const char* service = (service_.size() > 0) ? service_.c_str() : nullptr;

	//Call getaddrinfo
	if((status = getaddrinfo(host, service, &hints, &pAddrInfo)) != 0) {
		//Get error string and throw
		const char* errormsg = gai_strerror(status);
		std::string exmsg = "Call to getaddrinfo() failed: ";
		throw AddressException(status, exmsg + errormsg);
	}

	//Return a new Address instance initialized with the addr info pointer
	return std::make_unique<Address>(pAddrInfo);
}

/*!	@brief Default constructor */
Address::Address() : pAddrInfo_(nullptr), pCurrent_(nullptr) {
}

/*!	@brief Construct instance using AddrInfo_t*
 *	@param pinfo A pointer to AddrInfo_t data from getaddrinfo()
 */
Address::Address(AddrInfo_t* pInfo) :
	pAddrInfo_(pInfo), pCurrent_(pInfo) {
}

/*!	@brief Destructor */
Address::~Address() {
	//Free any resources allocated by getaddrinfo()
	if(pAddrInfo_ != nullptr)
		freeaddrinfo(pAddrInfo_);

	pCurrent_ = pAddrInfo_ = nullptr;
}

/*!	@brief Move constructor
 *	@param other Const Address reference rvalue to move from
 */
Address::Address(Address&& other) {
	//TODO Use move assignment operator, rather than duplicating this logic
	//Take rvalue attributes
	pAddrInfo_ = other.pAddrInfo_;
	pCurrent_ = other.pCurrent_;

	//We now own the allocated addrinfo struct,
	other.pAddrInfo_ = nullptr;
	other.pCurrent_ = nullptr;
}

/*!	@brief Move assignment operator
 *	@param other Const Address reference rvalue to move from
 *	@return A reference to this instance
 */
Address& Address::operator=(Address&& other) {
	//Handle self-assignment
	if(this != &other) {
		//Move other data to this instance
		pAddrInfo_ = other.pAddrInfo_;
		pCurrent_ = other.pCurrent_;
	}

	//assignment operator
	return *this;
}

/*!	@brief Resets the current address to the first value returned by getaddrinfo()
 */
void Address::first() {
	pCurrent_ = pAddrInfo_;
}

/*!	@brief Returns true if the address will contain valid information after a call to next()
 *	@return True if the address will be valid after calling next(), false if
 *	there are no more addresses in the linked list.
 */
bool Address::hasNext() const {
	return (pAddrInfo_->ai_next != nullptr) ? true : false;
}

/*!	@brief Increments the current address info pointer to the next value
 */
void Address::next() {
	if(pCurrent_) pCurrent_ = pCurrent_->ai_next; //Will set current = null if last
	else {
		//Current addrinfo.ai_next is null
		throw AddressException(-1, "Out of range");
	}
}

}; //Inet namespace

