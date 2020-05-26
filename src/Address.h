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
 *  This program is distributed Addin the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

//Include once
#ifndef ADDRESS_H_INCLUDED
#define ADDRESS_H_INCLUDED

//System includes
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

//Library includes
#include <string>
#include <vector>
#include <memory>

//Namespace container
namespace Inet {

//Forward declaration of Address for pointer typedefs
class Address;

typedef std::unique_ptr<Address>			AddressUPtr_t;
typedef Address*											AddressPtr_t;

/*!	@brief Type definitions for Address types */
typedef struct sockaddr								SockAddr_t;
typedef SockAddr_t*										SockAddrPtr_t;

typedef struct sockaddr_in						SockAddrIn_t;
typedef SockAddrIn_t*									SockAddrInPtr_t;

typedef struct addrinfo								AddrInfo_t;
typedef AddrInfo_t*										AddrInfoPtr_t;

/*!	@brief Iterator-type class for encapsulating singly-linked addrinfo* data
 *	Encapsulates addrinfo data returned by getaddrinfo() as an iterable type
 *	as well as exposing a static factory interface for retrieving address data
 *	according to inet family, socket protocol, host and service types.
 *	The Addresses value returned is suitable for use in calls to
 *	Socket::bind() and Socket::connect()
 *  @author jcleland
 */
class Address {
	public:
		/*!	@brief Address family type definition */
		enum class Family : int {
			ANY = AF_UNSPEC,
			IPV4 = AF_INET,
			IPV6 = AF_INET6
		};

		/*! @brief Socket protocol type definition
		 *	@todo This should probably be moved to Socket, but not wanting
		 *	to create a circular depend relationship between Socket and Address
		 */
		enum class Protocol : int {
			ANY = 0,
			UDP = SOCK_DGRAM,
			TCP = SOCK_STREAM
		};

		/*!	@brief Return an address instance for a port (or service type) on the local machine
		 *	@param service The service (port or service string) to use for addresses
		 *	@return An address instance referencing one or more addresses for the specified host and service
		 */
		static AddressUPtr_t get(const char *service);

		/*!	@brief Return an address instance for a host and port (or service type)
		 *	@param host The hostname to use for addresses
		 *	@param service The service (port or service string) to use for addresses
		 *	@return An address instance referencing one or more addresses for the specified host and service
		 */
		static AddressUPtr_t get(const char *host, const char *service);

		/*!	@brief Return an address instance for service name or port
		 *	@param host The hostname to use for addresses
		 *	@param service The service (port or service string) to use for addresses
		 *	@param family The address family to use
		 *	@param protocol The protocol specification, either TCP (Stream) or UDP (Datagram)
		 */
		static AddressUPtr_t get(const char* host, const char* service,
														 Family family, Protocol protocol);

	public:
		/*! @brief Default constructor */
		Address();

		/*! @brief Construct instance using AddrInfo*
		 *	Construction by factory interface to take a pointer to addinfo
		 *	as retrieved and allocated by getaddrinfo. This value should be
		 *	deallocated using freeaddrinfo() upon destruction.
		 *	@param pInfo A pointer to AddrInfo data from getaddrinfo()
		 */
		Address(AddrInfo_t* pInfo);

		/*!	@brief Destructor */
		virtual ~Address();

		/*!	@brief Copy constructor - deleted */
		Address(const Address& other) = delete;

		/*!	@brief Move constructor
		 *	@param other The instance to copy from
		 */
		Address(Address&& other);

		/*!	@brief Assignment operator - deleted */
		Address& operator=(const Address& other) = delete;

		/*!	@brief Move assignment operator
		 *	@param other A const rvalue reference to move from
		 *	@return A reference to this object instance
		 */
		Address& operator=(Address&& other);

		/*!	@brief Conversion operator for casting Address to addrinfo*
		 *	@return A pointer to the current addrinfo pointer associated with this address
		 */
		operator const AddrInfoPtr_t () const {
			return pCurrent_;
		}

		/*!	@brief Type conversion to sockaddr*
		 * 	@return A SockAddrPtr_t representing the address encapsulated by this Address
		 */
		operator const SockAddrPtr_t () const {
			return (pCurrent_ != nullptr) ? pCurrent_->ai_addr : nullptr;
		}

		/*!	@brief Reset current address reference to the first returned address
		 */
		void first();

		/*!	@brief Returns true if the address will contain valid information after a call to next()
		 *	@return True if the address will be valid after calling next(), false if
		 * 	there are no more addresses in the linked list.
		 */
		bool hasNext() const;

		/*!	@brief Increments the current address info pointer to the next value
		 */
		void next();

	private:
		//Static members
		static std::string 	localhost_;			/*!< Static host def for server addresses */

		//Member data
		AddrInfoPtr_t		pAddrInfo_;	/*!<	Address info struct returned by getaddrinfo() */
		AddrInfoPtr_t		pCurrent_;		/*!<	The first address info for iteration */
}; //Address

}; //Inet namespace

#endif // ADDRESS_H_INCLUDED
