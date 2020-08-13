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
#ifndef NETMESSAGE_H_INCLUDED
#define NETMESSAGE_H_INCLUDED

//System includes
#include <sys/types.h>

//Library includes
#include <vector>
#include <string>

//Project includes

//Namespace container
namespace Inet {

/*!	@brief Message class for sending data over network sockets
 */
class NetStream {
public:
	/*! @brief Encoded data type markers for built-ins and primatives */
	enum Type : u_short {
		CHAR				= 'c',
		UCHAR				= 'C',
		INT16				= 'i',
		UINT16			= 'I',
		INT32				= 'l',
		UINT32			= 'L',
		INT64				= 'w',
		UINT64			= 'W',
		STRING			= 's',
		STREAM			= 'S',
		STREAMABLE	= 'M',
		UNKNOWN			= 'X'
	};

	///Typedefs local to class
	typedef std::vector<char>						Buffer_t;

public:
	/*!	@brief Class constructor */
	NetStream();

	/*!	@brief Construct with pre-allocated storage */
	explicit NetStream(const size_t bytes) { buffer_.reserve(bytes); }

	/*!	@brief Initialize the stream with raw data */
	explicit NetStream(const char *data, const size_t bytes);

	/*!	@brief Copy constructor, delete default */
	NetStream(const NetStream& other) = delete;

	/*!	@brief Move constructor */
	NetStream(NetStream&& other);

	/*!	@brief Assignment operator, delete default */
	NetStream& operator=(const NetStream& other) = delete;

	/*!	@brief Move assignment operator */
	NetStream& operator=(NetStream&& other);

	/*!	@brief Destructor */
	virtual ~NetStream();

	/*!	@brief Returns the size of the storage vector in bytes */
	inline u_int32_t size() const { return buffer_.size(); }

	/*!	@brief Returns a const reference to the internal data vector */
	inline const Buffer_t& data() const { return buffer_; }

	/*!	@brief Set raw data in the internal vector
	 *	@param data Pointer to data buffer to encode
	 *	@param	size The size of the data buffer
	 *	@return 0
	 */
	int32_t setRaw( u_char* data, u_int32_t size );

	/*!	@brief Add char value to this data stream
	 *	@param val Character value to append to the stream data
	 *	@return Reference to this message stream
	 */
	virtual NetStream& operator<<(const char& val);

	/*!	@brief Add unsigned char value to this data stream
	 *	@param val Unsigned character value to append to the stream data
	 *	@return Reference to this message stream
	 */
	virtual NetStream& operator<<(const u_char& val);

	/*!	@brief Append signed 16-bit value to stream data
	 *	@param val signed 16-bit value
	 *	@return A reference to this stream instance
	 */
 	virtual NetStream& operator<<(const int16_t& val);

	/*!	@brief Append unsigned 16-bit value to stream data
	 *	@param val unsigned 16-bit value
	 *	@return A reference to this stream instance
	 */
	virtual NetStream& operator<<(const u_int16_t& val);

	/*!	@brief Append signed int32 value to stream data
	 *	@param val Signed 32-bit integer value
	 *	@return A reference to this stream instance
	 */
	virtual NetStream& operator<<(const int32_t& val);

	/*!	@brief Append unsigned int32 value to stream data
	 *	@param val Unsigned 32-bit integer value
	 *	@return A reference to this stream instance
	 */
	virtual NetStream& operator<<(const u_int32_t& val);

	/*!	@brief Append signed int64 value to stream data
	 *	@param val Signed 64-bit integer value
	 *	@return A reference to this stream instance
	 */
	virtual NetStream& operator<<(const int64_t& val);

	/*!	@brief Append unsigned int64 value to stream data
	 *	@param val Unsigned 64-bit integer value
	 *	@return A reference to this stream instance
	 */
	virtual NetStream& operator<<(const u_int64_t& val);

	/*!	@brief Encode a std::string to stream data
	 *	@param val A const reference to std:string to encode
	 *	@return Reference to this stream instance
	 */
	virtual NetStream& operator<<(const std::string &val);

	/*!	@brief Write a buffer of char data to the stream
	 *	@param buf A pointer to the data to write
	 *	@param bytes The number of bytes to write from the pointer
	 *	@return A reference to this stream
	 */
	virtual NetStream& write(const char* buf, const size_t bytes);

	/*!	@brief Add char value to this data stream
	 *	@param val Character value to append to the stream data
	 *	@return Reference to this message stream
	 */
	virtual NetStream& operator>>(char& val);

	/*!	@brief Read and remove unsigned character value from this data stream
	 *	@param val Unsigned character value to read from stream data
	 *	@return Reference to this message stream
	 */
	virtual NetStream& operator>>(u_char& val);

	/*!	@brief Read and remove signed 16-bit value from this data stream
	 *	@param val Reference to signed 16-bit value to read from stream data
	 *	@return Reference to this message stream
	 */
	virtual NetStream& operator>>(int16_t& val);

	/*!	@brief Read and remove unsigned 16-bit value from this data stream
	 *	@param val Reference to signed 16-bit value to read from stream data
	 *	@return Reference to this message stream
	 */
	virtual NetStream& operator>>(u_int16_t& val);

	/*!	@brief Read and remove unsigned character value from this data stream
	 *	@param val Unsigned character value to read from stream data
	 *	@return Reference to this message stream
	 */
	virtual NetStream& operator>>(int32_t& val);

	/*!	@brief Read and remove unsigned character value from this data stream
	 *	@param val Unsigned character value to read from stream data
	 *	@return Reference to this message stream
	 */
	virtual NetStream& operator>>(u_int32_t& val);

	/*!	@brief Read and remove unsigned character value from this data stream
	 *	@param val Unsigned character value to read from stream data
	 *	@return Reference to this message stream
	 */
	virtual NetStream& operator>>(int64_t& val);

	/*!	@brief Read and remove unsigned character value from this data stream
	 *	@param val Unsigned character value to read from stream data
	 *	@return Reference to this message stream
	 */
	virtual NetStream& operator>>(u_int64_t& val);

	/*!	@brief Decode a std::string from stream data
	 *	@param val A const reference to std:string to hold decoded string data
	 *	@return Reference to this stream instance
	 */
	virtual NetStream& operator>>(std::string &val);

private:
	/*!	@brief Generic parameterized byte-ordering function
	 *	@type input T& value to be converted to network byte order
	 *	@return Input value converted to network byte order
	 * TODO: Not currently being used
	 */
	template <typename T> static T hton_t(const T &input);

	/*!	@brief Convert 64-bit value to network byte order
	 *	@param val 64-bit value to be converted
	 *	@return The value provided, converted to network byte order
	 */
	u_int64_t htonll(u_int64_t val);

	/*!	@brief Convert 64-bit value from network to host byte order
	 *	@param val 64-bit value to be converted
	 *	@return The value provided, converted to host byte order
	 */
	u_int64_t ntohll(u_int64_t val);

private:
	Buffer_t		buffer_;
};
} //Namespace

#endif // NETMESSAGE_H_INCLUDED
