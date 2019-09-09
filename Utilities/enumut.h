/*
* Name: Enumeration Utilities
* Author: Robert Ciborowski
* Date: 05/03/2018
* Description: A file used for performing operations on enums.
               If you told me that enumut is a silly-sounding name, I'd agree with you.
*/

#pragma once

// This gets the value of an enum class.
// https://stackoverflow.com/questions/11421432/how-can-i-output-the-value-of-an-enum-class-in-c11
template <typename Enumeration> auto enumToInteger(Enumeration const value) {
	return static_cast<typename std::underlying_type<Enumeration>::type>(value);
}