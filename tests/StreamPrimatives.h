/**
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
#ifndef STREAMTESTS_H_INCLUDED
#define STREAMTESTS_H_INCLUDED

//CxxTest includes
#include <cxxtest/TestSuite.h>

//Standard library includes
#include <string>
#include <vector>
#include <memory>

//Include library headers
#include "NetStream.h"
#include "StreamException.h"

//Include shared test config header
#include "TestCommon.h"

//Using Inet namespace
using namespace Inet;

/*!
 * Unit tests for the Inet::Address class
 * @author jcleland
 */
class StreamPrimatives : public CxxTest::TestSuite {
public:
	/*!	@brief Test encode/decode of signed char */
	void test_char(void) {
		NetStream stream;
		char c = 0b01010101;

		try {
			stream << c;
			stream >> c;
			TS_ASSERT(c == 0b01010101);
		}
		catch(const StreamException &se) {
			TS_FAIL(se.what());
		}
	}

	/*!	@brief Test encode/decode of unsigned char */
	void test_uchar(void) {
		NetStream stream;
		u_char c = 0b10101010;

		try {
			stream << c;
			stream >> c;
			TS_ASSERT(c == 0b10101010);
		}
		catch(const StreamException &se) {
			TS_FAIL(se.what());
		}
	}

	/*!	@brief Test encode/decode of signed char */
	void test_in16(void) {
		NetStream stream;
		int16_t c = 0b0101010101010101;

		try {
			stream << c;
			stream >> c;
			TS_ASSERT(c == 0b0101010101010101);
		}
		catch(const StreamException &se) {
			TS_FAIL(se.what());
		}
	}

	/*!	@brief Test encode/decode of unsigned char */
	void test_uint16(void) {
		NetStream stream;
		u_int16_t c = 0b1010101010101010;

		try {
			stream << c;
			stream >> c;
			TS_ASSERT(c == 0b1010101010101010);
		}
		catch(const StreamException &se) {
			TS_FAIL(se.what());
		}
	}

	/*!	@brief Test encode/decode of signed char */
	void test_in32(void) {
		NetStream stream;
		int32_t c = 0b01010101010101010101010101010101L;

		try {
			stream << c;
			stream >> c;
			TS_ASSERT(c == 0b01010101010101010101010101010101L);
		}
		catch(const StreamException &se) {
			TS_FAIL(se.what());
		}
	}

	/*!	@brief Test encode/decode of unsigned char */
	void test_uint32(void) {
		NetStream stream;
		u_int32_t c = 0b10101010101010101010101010101010UL;

		try {
			stream << c;
			stream >> c;
			TS_ASSERT(c == 0b10101010101010101010101010101010UL);
		}
		catch(const StreamException &se) {
			TS_FAIL(se.what());
		}
	}

	/*!	@brief Test encode/decode of signed char */
	void test_in64(void) {
		NetStream stream;
		int64_t c = 0b01010101010101010101010101010101LL;

		try {
			stream << c;
			stream >> c;
			TS_ASSERT(c == 0b01010101010101010101010101010101LL);
		}
		catch(const StreamException &se) {
			TS_FAIL(se.what());
		}
	}

	/*!	@brief Test encode/decode of unsigned char */
	void test_uint64(void) {
		NetStream stream;
		u_int64_t c = 0b10101010101010101010101010101010ULL;

		try {
			stream << c;
			stream >> c;
			TS_ASSERT(c == 0b10101010101010101010101010101010ULL);
		}
		catch(const StreamException &se) {
			TS_FAIL(se.what());
		}
	}

};

#endif //Inet namespace
