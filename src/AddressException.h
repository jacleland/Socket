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
#ifndef ADDRESSEXCEPTION_H_INCLUDED
#define ADDRESSEXCEPTION_H_INCLUDED

//System includes

//Library includes
#include <exception>
#include <string>

///Project includes

//Namespace container
namespace Inet {

/*!	@brief Exception type thrown by Address
 *	@author jcleland
 */
class AddressException : public std::exception {
	public:
		/*!	@brief Constructor for char* error message
		 *	@param code An integer value to be associated with this exception
		 *	@param message Point to buffer containing NULL-terminated string
		 */
		explicit AddressException(const int& code, const char* message) :
			code_(code), msg_(message) {
		}

		/*!	@brief Constructor for std::string error message
		 *	@param code An integer value to be associated with this exception
		 *	@param message A std::string instance containing the error message
		 */
		explicit AddressException(const int& code, const std::string& message) :
			code_(code), msg_(message) {
		}

		/*!	@brief Destructor */
		virtual ~AddressException() throw() {}

		/*! @brief Returns an integer exception code
		 *	@return An integer value representing the code associated with this exception
		 */
		virtual const int& code() const noexcept {
			return code_;
		}

		/*!	@brief Returns a pointer to the (constant) error description
		 *	@return A const char* pointing to the error message string
		 *	@throw std::exception if message is null
		 */
		virtual const char* what() const noexcept {
			return msg_.c_str();
		}

	protected:
		int						code_;				/*! Error code associated with this exception */
		std::string 	msg_;					/*! Message string */
};
} //Inet namespace

#endif	//ADDRESSEXCEPTION_H_INCLUDED
