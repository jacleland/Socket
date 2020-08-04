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

//System includes
#include <netinet/in.h>
#include <time.h>
#include <stdio.h>
#include <memory.h>

//Library includes
#include <memory>

//Project includes
#include "NetStream.h"
#include "StreamException.h"

//Namespace container
namespace Inet {

	/*!	@brief Default constructor */
	NetStream::NetStream() {}

	/*!	@brief Initialize the stream with raw data
	 *	@param data Pointer to buffer containing stream data
	 *	@param bytes the number of bytes to read from the data pointer
	 */
	NetStream::NetStream(const char *data, const size_t bytes) :
		buffer_(bytes)
	{
		memcpy((void*)&buffer_[0], (void*)data, bytes);
	}

	/*!	@brief Move constructor
	 *	@param other Object to move
	 */
	NetStream::NetStream(NetStream&& other) {
		if(this != &other) {
			buffer_ = std::move(other.buffer_);
		}
	}

	/*!	@brief Move assignment operator
	 *	@param other Object to move-assign from
	 *	@return A reference to this instance
	 */
	NetStream& NetStream::operator=(NetStream&& other) {
		//Handle self-assignment
		if(this != &other) {
			buffer_ = std::move(other.buffer_);
		}
		return *this;
	}

	/*!	@brief Destructor */
	NetStream::~NetStream() {}

	/*!	@brief
	 */
	int32_t NetStream::setRaw(u_char* data, u_int32_t size) {
		//Erase all elements in the stream first!
		if( buffer_.size() > 0 )
			buffer_.erase(buffer_.begin(), buffer_.end());

		//Pre-allocate the space in the vector.
		buffer_.resize( size );

		//Insert buffer into vector
		memcpy(&(*(buffer_.begin())), data, size);
		return 0;
	}

	/*!	@brief Add char value to this data stream
	 *	@param val Character value to append to the stream data
	 *	@return Reference to this stream instance
	 */
	NetStream& NetStream::operator<<(const char& val)	{
		buffer_.push_back( STREAM_TYPE_CHAR );
		buffer_.push_back( val );
		buffer_.push_back( STREAM_TYPE_CHAR );
		return *this;
	}

	/*!	@brief Add unsigned char value to this data stream
	 *	@param val Unsigned character value to append to the stream data
	 *	@return Reference to this stream instance
	 */
	NetStream& NetStream::operator<<(const u_char& val) {
		buffer_.push_back( STREAM_TYPE_UCHAR );
		buffer_.push_back( val );
		buffer_.push_back( STREAM_TYPE_UCHAR );
		return *this;
	}

	/*!	@brief Push signed 16-bit value on stream
	 *	@param val Signed 16-bit value
	 *	@return Reference to this stream instance
	 */
	NetStream&	NetStream::operator<<(const int16_t& val) {
		//Declare local
		int16_t netval;

		//Convert to Network Byte Order
		netval = htons(val);
		u_char* pIndex = (u_char*)&netval;

		//Push the type indicator, integer bytes, and closing type indicator
		buffer_.push_back( STREAM_TYPE_INT16 );
		for( u_int16_t count = 0; count < sizeof(int16_t); count++ ){
			buffer_.push_back(*pIndex);
			pIndex++;
		}
		buffer_.push_back( STREAM_TYPE_INT16 );

		//Return
		return *this;
	}

	/*!	@brief Append unsigned 16-bit value to stream data
	 *	@param val unsigned 16-bit value
	 *	@return A reference to this stream instance
	 */
	NetStream&	NetStream::operator<<(const u_int16_t& val) {
		//Declare local
		u_int16_t  netval;

		//Convert to Network Byte Order
		netval = htons(val);
		u_char* pIndex = (u_char*)&netval;

		//Push the type indicator
		buffer_.push_back( STREAM_TYPE_UINT16 );
		for(u_int16_t count = 0; count < sizeof(u_int16_t); count++) {
			buffer_.push_back(*pIndex);
			pIndex++;
		}
		buffer_.push_back( STREAM_TYPE_UINT16 );

		//Return
		return *this;
	}

	/*!	@brief Append signed int32 value to stream data
	 *	@param val Signed 32-bit integer value
	 *	@return A reference to this stream instance
	 */
	NetStream&	NetStream::operator<<(const int32_t& val) {
		//Declare local
		int32_t  netval;

		//Convert to NetworkByte Order
		netval = htonl(val);
		u_char* pIndex = (u_char*)&netval;

		//Push the type indicator
		buffer_.push_back( STREAM_TYPE_INT32 );
		for(u_int16_t count = 0; count < sizeof(int32_t); count++) {
			buffer_.push_back( *pIndex );
			pIndex++;
		}
		buffer_.push_back( STREAM_TYPE_INT32 );

		//Return
		return *this;
	}

	/*!	@brief Append unsigned int32 value to stream data
	 *	@param val Unsigned 32-bit integer value
	 *	@return A reference to this stream instance
	 */
	NetStream& NetStream::operator<<(const u_int32_t& val) {
		//Declare local
		u_int32_t   netval;

		//Convert to Network Byte Order
		netval = htonl(val);
		u_char* pIndex = (u_char*)&netval;

		//Push the type indicator
		buffer_.push_back( STREAM_TYPE_UINT32 );
		for(u_int16_t count = 0; count < sizeof(u_int32_t); count++) {
			buffer_.push_back( *pIndex );
			pIndex++;
		}
		buffer_.push_back( STREAM_TYPE_UINT32 );

		//Return
		return *this;
	}

	/*!	@brief Append signed int64 value to stream data
	 *	@param val Signed 64-bit integer value
	 *	@return A reference to this stream instance
	 */
	NetStream& NetStream::operator<<(const int64_t& val) {
		//Declare local
		int64_t   netval;

		//Convert to Network Byte Order
		netval = htonll(val);
		u_char* pIndex = (u_char*)&netval;

		//Push the type indicator
		buffer_.push_back( STREAM_TYPE_INT64 );
		for(u_int16_t count = 0; count < sizeof(int64_t); count++) {
			buffer_.push_back( *pIndex );
			pIndex++;
		}
		buffer_.push_back( STREAM_TYPE_INT64 );

		//Return
		return *this;
	}

	/*!	@brief Append unsigned int64 value to stream data
	 *	@param val Unsigned 64-bit integer value
	 *	@return A reference to this stream instance
	 */
	NetStream& NetStream::operator<<(const u_int64_t& val) {
		//Declare local
		u_int64_t   netval;

		//Convert to Network Byte Order
		netval = htonll(val);
		u_char* pIndex = (u_char*)&netval;

		//Push the type indicator
		buffer_.push_back( STREAM_TYPE_UINT64 );
		for(u_int16_t count = 0; count < sizeof(u_int64_t); count++) {
			buffer_.push_back( *pIndex );
			pIndex++;
		}
		buffer_.push_back( STREAM_TYPE_UINT64 );

		//Return
		return *this;
	}

	/*!	@brief Encode a std::string to stream data
	 *	@param val A const reference to std:string to encode
	 *	@return Reference to this stream instance
	 */
	NetStream& NetStream::operator<<(const std::string &val) {
		//Declare local
		size_t count;
		u_int32_t length;
		u_int32_t netlength;

		//Get the length of the string
		length = val.length();

		//Push the type indicator (tail)
		buffer_.push_back( STREAM_TYPE_STRING );

		//Push the string into the vector first
		for( count = 0; count < length; count++ ) {
			buffer_.push_back(val[(size_t)count]);
		}

		//Create a byte ordered length
		netlength = htonl(length);
		u_char* pIndex = (u_char*)&netlength;

		//Now push the length value into the vector
		for( count = 0; count < sizeof(u_int32_t); count++ ) {
			buffer_.push_back(*pIndex);
			pIndex++;
		}

		//Push the type indicator (head)
		buffer_.push_back( STREAM_TYPE_STRING );

		//Return
		return *this;
	}


	/*!	@brief Add char value to this data stream
	 *	@param val Character value to append to the stream data
	 *	@return Reference to this message stream
	 */
	NetStream& NetStream::operator>>(char& val) {
		//Declare local
		Buffer_t::iterator  itr;

		//point the iterator
		itr = buffer_.end()-1;

		//Validate the data
		if( *itr != STREAM_TYPE_CHAR || *(itr-(sizeof(char)+1)) != STREAM_TYPE_CHAR ) {
			throw StreamException( STREAM_ERR_INVALID_TYPE, STREAM_MSG_INVALID_TYPE);
		}

		//Extract this value from the vector
		itr--;                              //Move past the Type byte
		val = (char)*itr;                   //Get the value after the Type
		itr--;                              //Move to the end marker
		buffer_.erase( itr, buffer_.end() );//Delete this value

		//Return reference to ourselves
		return *this;
	}

	/*!	@brief Read and remove unsigned character value from this data stream
	 *	@param val Unsigned character value to read from stream data
	 *	@return Reference to this message stream
	 */
	NetStream& NetStream::operator>>(u_char& val)	{
		//Declare local
		Buffer_t::iterator  itr;

		//point the iterator
		itr = buffer_.end()-1;

		//Validate the data
		if( *itr != STREAM_TYPE_UCHAR || *(itr-(sizeof(u_char)+1)) != STREAM_TYPE_UCHAR ) {
			throw StreamException( STREAM_ERR_INVALID_TYPE, STREAM_MSG_INVALID_TYPE);
		}

		//Extract this value from the vector
		itr--;                              //Move past the Type byte
		val = (u_char)*itr;                 //Get the value after the Type
		itr--;                              //Move to the end marker
		buffer_.erase( itr, buffer_.end() );//Delete this value

		//Return reference to ourselves
		return *this;
	}

	/*!	@brief Remove a 16-bit signed integer value from the data stream
	 *	@param Signed 16-bit integer reference to receive the stream data
	 *	@return A reference to this data stream
	 */
	NetStream& NetStream::operator>>(int16_t& val) {
		//Declare local
		u_char*							pIndex = (u_char*)&val;
		Buffer_t::iterator	itr;

		//point the iterator
		itr = buffer_.end()-1;

		//Validate the data
		if(*itr != STREAM_TYPE_INT16 || *(itr-(sizeof(int16_t)+1)) != STREAM_TYPE_INT16) {
			throw StreamException(STREAM_ERR_INVALID_TYPE, STREAM_MSG_INVALID_TYPE);
		}

		//Remove low byte first then remove high byte
		itr--;                              //Move past the Type byte
		pIndex++;                           //Move to the second byte in the int
		*pIndex = *itr; pIndex--; itr--;    //Get the high byte
		*pIndex = *itr;                     //Get the low byte
		itr--;                              //Move to the end marker
		buffer_.erase( itr, buffer_.end() );//Delete this value

		//Byte order the value
		val = ntohs(val);

		//Return reference to ourselves
		return *this;
	}

	/*!	@brief Remove a 16-bit unsigned integer value from the data stream
	 *	@param Unsigned 16-bit integer reference to receive the stream data
	 *	@return A reference to this data stream
	 */
	NetStream& NetStream::operator>>(u_int16_t& val) {
		//Declare local
		u_char*							pIndex = (u_char*)&val;
		Buffer_t::iterator	itr;

		//point the iterator
		itr = buffer_.end()-1;

		//Validate the data
		if(*itr != STREAM_TYPE_UINT16 || *(itr-(sizeof(u_int16_t)+1)) != STREAM_TYPE_UINT16) {
			throw StreamException(STREAM_ERR_INVALID_TYPE, STREAM_MSG_INVALID_TYPE);
		}

		//Remove low byte first then remove high byte
		itr--;                              //Move past the Type byte
		pIndex++;                           //Move to the second byte in the int
		*pIndex = *itr; pIndex--; itr--;    //Get the high byte
		*pIndex = *itr;                     //Get the low byte
		itr--;                              //Move to the end marker
		buffer_.erase( itr, buffer_.end() );//Delete this value

		//Byte order the value
		val = ntohs(val);

		//Return reference to ourselves
		return *this;
	}

	/*!	@brief Remove a 32-bit signed integer value from the data stream
	 *	@param Signed 32-bit integer reference to receive the stream data
	 *	@return A reference to this data stream
	 */
	NetStream& NetStream::operator>>(int32_t& val) {
		//Declare local
		u_char*							pIndex = (u_char*)&val;
		Buffer_t::iterator	itr;

		//point the iterator
		itr = buffer_.end()-1;

		//Validate the data
		if(*itr != STREAM_TYPE_INT32 || *(itr-(sizeof(int32_t)+1)) != STREAM_TYPE_INT32) {
			throw StreamException(STREAM_ERR_INVALID_TYPE, STREAM_MSG_INVALID_TYPE);
		}

		//Remove low byte first then remove high byte
		itr--;                              //Move past the Type byte
		pIndex+=3;                          //Move to the fourth byte in the long
		for(u_int16_t count = 0; count < sizeof(int32_t); count++ ) {
			(*pIndex) = (*itr); itr--; pIndex--;
		}
		buffer_.erase( itr, buffer_.end() );//Delete this value

		//Byte order the value
		val = ntohl(val);

		//Return reference to ourselves
		return *this;
	}

	/*!	@brief Remove a 32-bit unsigned integer value from the data stream
	 *	@param Unsigned 32-bit integer reference to receive the stream data
	 *	@return A reference to this data stream
	 */
	NetStream& NetStream::operator>>(u_int32_t& val) {
		//Declare local
		u_char*							pIndex = (u_char*)&val;
		Buffer_t::iterator	itr;

		//point the iterator
		itr = buffer_.end()-1;

		//Validate the data
		if(*itr != STREAM_TYPE_UINT32 || *(itr-(sizeof(u_int32_t)+1)) != STREAM_TYPE_UINT32) {
			throw StreamException(STREAM_ERR_INVALID_TYPE, STREAM_MSG_INVALID_TYPE);
		}

		//Remove low byte first then remove high byte
		itr--;                              //Move past the Type byte
		pIndex+=3;                          //Move to the fourth byte in the long
		for(u_int16_t count = 0; count < sizeof(u_int32_t); count++ ) {
			(*pIndex) = (*itr); itr--; pIndex--;
		}
		buffer_.erase( itr, buffer_.end() );//Delete this value

		//Byte order the value
		val = ntohl(val);

		//Return reference to ourselves
		return *this;
	}

	/*!	@brief Remove a 64-bit signed integer value from the data stream
	 *	@param Signed 64-bit integer reference to receive the stream data
	 *	@return A reference to this data stream
	 */
	NetStream& NetStream::operator>>(int64_t& val) {
		//Declare local
		u_char*							pIndex = (u_char*)&val;
		Buffer_t::iterator	itr;

		//point the iterator
		itr = buffer_.end()-1;

		//Validate the data
		if(*itr != STREAM_TYPE_INT64 || *(itr-(sizeof(int64_t)+1)) != STREAM_TYPE_INT64) {
			throw StreamException(STREAM_ERR_INVALID_TYPE, STREAM_MSG_INVALID_TYPE);
		}

		//Remove low byte first then remove high byte
		itr--;                              //Move past the Type byte
		pIndex+=7;                          //Move to the fourth byte in the long
		for(u_int16_t count = 0; count < sizeof(int64_t); count++ ) {
			(*pIndex) = (*itr); itr--; pIndex--;
		}
		buffer_.erase( itr, buffer_.end() );//Delete this value

		//Byte order the value
		val = ntohll(val);

		//Return reference to ourselves
		return *this;
	}

	/*!	@brief Remove a 64-bit unsigned integer value from the data stream
	 *	@param Unsigned 64-bit integer reference to receive the stream data
	 *	@return A reference to this data stream
	 */
	NetStream& NetStream::operator>>(u_int64_t& val) {
		//Declare local
		u_char*							pIndex = (u_char*)&val;
		Buffer_t::iterator	itr;

		//point the iterator
		itr = buffer_.end()-1;

		//Validate the data
		if(*itr != STREAM_TYPE_UINT64 || *(itr-(sizeof(u_int64_t)+1)) != STREAM_TYPE_UINT64) {
			throw StreamException(STREAM_ERR_INVALID_TYPE, STREAM_MSG_INVALID_TYPE);
		}

		//Remove low byte first then remove high byte
		itr--;                              //Move past the Type byte
		pIndex+=7;                          //Move to the fourth byte in the long
		for(u_int16_t count = 0; count < sizeof(u_int64_t); count++ ) {
			(*pIndex) = (*itr); itr--; pIndex--;
		}
		buffer_.erase( itr, buffer_.end() );//Delete this value

		//Byte order the value
		val = ntohll(val);
		//val = hton_t<u_int64_t>(val);

		//Return reference to ourselves
		return *this;
	}

	/*!	@brief
	 */
	NetStream& NetStream::operator>>(std::string &val) {
		//Declare local
		Buffer_t::iterator itr;
		u_int32_t count;
		u_int32_t length;
		u_int8_t* pIndex = (u_int8_t*)&length;

		//point the iterator
		itr = buffer_.end()-1;

		//Validate the data
		if( *itr != STREAM_TYPE_STRING )
			throw StreamException( STREAM_ERR_INVALID_TYPE, STREAM_MSG_INVALID_TYPE);

		//Move past the Type
		itr--;

		//Get the length value from the vector and fix byte-ordering
		pIndex += 3;
		for(count = 0; count < sizeof(u_int32_t); count++) {
			*pIndex = (u_int8_t)(*itr);
			itr--; pIndex--;
		}
		length = ntohl(length);

		//Check the size against the pointer and check for type at the end
		if( *(itr-length) != STREAM_TYPE_STRING )
			throw StreamException( STREAM_ERR_INVALID_TYPE,STREAM_MSG_INVALID_TYPE);

		//Allocate buffer for string
		std::unique_ptr<u_char[]> pBuffer(new u_char[length+1]);
		memset(pBuffer.get(), 0x00, length + 1);

		//Get the string from the vector
		for(count = length; count > 0; count--) {
			*(pBuffer.get() + count - 1) = (*itr);
			itr--;
		}

		//Erase the vector contents
		buffer_.erase( itr, buffer_.end() );//Delete this value

		//Assign value
		val = std::string((char*)pBuffer.get());

		//Return reference to ourselves
		return *this;
	}

	/*!	@brief Generic parameterized byte-ordering function
	 *	@type input T& value to be converted to network byte order
	 *	@return Input value converted to network byte order
	 * TODO: Not currently being used
	 */
	template<typename T>
	T NetStream::hton_t(const T& input) {
		T output(0);
		const std::size_t size = sizeof(input);
		uint8_t *data = reinterpret_cast<uint8_t *>(&output);

		for (std::size_t i = 0; i < size; i++) {
			data[i] = input >> ((size - i - 1) * 8);
		}

		return output;
	}

	/*!	@brief Convert 64-bit value to network byte order
	 *	@param val 64-bit value to be converted
	 *	@return The value provided, converted to network byte order
	 */
	uint64_t NetStream::htonll(uint64_t value) {
		static const int num = 42;
		if (*reinterpret_cast<const char*>(&num) != num)
			return value;

		const uint32_t high_part = htonl(static_cast<uint32_t>(value >> 32));
		const uint32_t low_part = htonl(static_cast<uint32_t>(value & 0xFFFFFFFFLL));

		return (static_cast<uint64_t>(low_part) << 32) | high_part;
	}

	/*!	@brief Convert 64-bit value from network to host byte order
	 *	@param val 64-bit value to be converted
	 *	@return The value provided, converted to host byte order
	 */
	uint64_t NetStream::ntohll(uint64_t value) {
		static const int num = 42;
		if (*reinterpret_cast<const char*>(&num) != num)
			return value;

		const uint32_t high_part = ntohl(static_cast<uint32_t>(value >> 32));
		const uint32_t low_part = ntohl(static_cast<uint32_t>(value & 0xFFFFFFFFLL));

		return (static_cast<uint64_t>(low_part) << 32) | high_part;
	}
} //Inet namespace end
