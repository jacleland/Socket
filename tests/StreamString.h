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
#ifndef STREAMSTRING_H_INCLUDED
#define STREAMSTRING_H_INCLUDED

//CxxTest includes
#include <cxxtest/TestSuite.h>

//Runtime includes

//Standard library includes
#include <iostream>
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

#define STRING_DATA "This is a string"

/*!
 * Unit tests for the Inet::Address class
 * @author jcleland
 */
class StreamString : public CxxTest::TestSuite {
public:
	/*!	@brief Test encode/decode of signed char */
	void test_string(void) {
		NetStream stream;
		std::string str(STRING_DATA);

		try {
			stream << str;
			stream >> str;
			TS_ASSERT(str.compare(STRING_DATA) == 0);
		}
		catch(const StreamException &se) {
			TS_FAIL(se.what());
		}
	}

	/*!	@brief Test setting raw stream data */
	void test_rawstring(void) {
		std::string str;
		char raw[] = {
			0x73, 0x54, 0x68, 0x69, 0x73, 0x20, 0x69, 0x73,
			0x20, 0x61, 0x20, 0x73, 0x74, 0x72, 0x69, 0x6e,
			0x67, 0x00, 0x00, 0x00, 0x10, 0x73
		};

		try {
			NetStream stream(raw, sizeof(raw));
			stream >> str;
			TS_ASSERT(str.compare(STRING_DATA) == 0);
		}
		catch(const StreamException &se) {
			TS_FAIL(se.what());
		}
	}
};

#endif //Include once
