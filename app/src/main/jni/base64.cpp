/*
 * base64.cpp
 *
 *  Created on: 30/04/2011
 *      Author: nicholas
 */

#include "base64.h"
#include <cctype>
#include <cstdint>
#include <algorithm>

namespace base64
{

namespace
{

static const std::string BASE64_CHARS = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

size_t encoded_size(size_t raw)
{
	switch((raw*8)%6)
	{
		case 0:
			return (raw*8)/6;
		case 2:
			return ((raw*8)/6) + 3;
		case 4:
			return ((raw*8)/6) + 2;
	}
	return raw;
}

size_t decoded_size(size_t unpadded)
{
	return (unpadded*6)/8;
}

int base64_index(std::string::value_type c)
{
	if(c >= 'A' && c <= 'Z')
		return c-'A';
	else if(c >= 'a' && c <= 'z')
		return c-'a' + 26;
	else if(c >= '0' && c <= '9')
		return c-'0' + 52;
	else if(c == '+')
		return 62;
	else if(c == '/')
		return 63;
	else
		return -1;
}

}

encode_t::encode_t(std::string::size_type size)
 : state(zero), remainder(0)
{
	encoded.reserve(encoded_size(size));
}

/*
State zero
	8 bits input, zero remaining from last
	6 bits consumed, 2 remaining
	=> two
State two
	8 bits input, 2 remaining from last
	4 bits consumed, 4 remaining
	=> four
State four
	8 bits input, 4 remaining from last
	2 bits consumed, 6 remaining
	6 bits input, 6 remaining from last
	6 bits consumed, 0 remaining
	=> zero
 */
void encode_t::operator() (std::string::value_type c)
{
	unsigned char value(0);
	switch(state)
	{
		case zero:
			value = (c & 0xfc) >> 2;
			remainder = c & 0x3;
			encoded.push_back(BASE64_CHARS[value]);
			state = two;
			break;
		case two:
			value = (remainder << 4) | ((c & 0xf0) >> 4);
			remainder = c & 0xf;
			encoded.push_back(BASE64_CHARS[value]);
			state = four;
			break;
		case four:
			value = (remainder << 2) | ((c & 0xc0) >> 6);
			remainder = c & 0x3f;
			encoded.push_back(BASE64_CHARS[value]);
			value = remainder;
			encoded.push_back(BASE64_CHARS[value]);
			state = zero;
			break;
	}
}

std::string encode_t::str()
{
	unsigned char value(0);
	switch(state)
	{
		case zero:
			break;
		case two:
			value = remainder << 4;
			encoded.push_back(BASE64_CHARS[value]);
			encoded.push_back('=');
			encoded.push_back('=');
			state = zero;
			break;
		case four:
			value = remainder << 2;
			encoded.push_back(BASE64_CHARS[value]);
			encoded.push_back('=');
			state = zero;
			break;
	}
	return encoded;
}

decode_t::decode_t(std::string::size_type size)
 : state(zero), remainder(0)
{
	decoded.reserve(decoded_size(size));
}

/*
State zero
	6 bits input, zero remaining from last
	6 bits consumed, zero remaining
	=> six
State six
	6 bits input, 6 remaining from last
	write 1 byte, 4 remaining
	=> four
State four
	6 bits input, 4 remaining from last
	write 1 byte, 2 remaining
	=> two
State two
	6 bits input, 2 remaining from last
	write 1 byte, 0 remaining
	=> zero
 */
void decode_t::operator() (std::string::value_type c)
{
	unsigned char value(0);
	int index = base64_index(c);
	if(index == -1)
		return;
	switch(state)
	{
		case zero:
			remainder = index;
			state = six;
			break;
		case six:
			value = (remainder << 2) | ((index & 0x30) >> 4);
			remainder = index & 0xf;
			decoded.push_back(value);
			state = four;
			break;
		case four:
			value = (remainder << 4) | ((index & 0x3c) >> 2);
			remainder = index & 0x3;
			decoded.push_back(value);
			state = two;
			break;
		case two:
			value = (remainder << 6) | index;
			decoded.push_back(value);
			state = zero;
			break;
	}
}

std::string decode_t::str() const
{
	return decoded;
}

std::string encode(const std::string& str)
{
	return std::for_each(str.begin(), str.end(), encode_t(str.size())).str();
}

std::string decode(const std::string& str)
{
	size_t unpadded_size = str.size();
	if(str.size() > 0 && str[str.size()-1] == '=')
		unpadded_size -= 1;
	if(str.size() > 1 && str[str.size()-2] == '=')
		unpadded_size -= 1;

	return std::for_each(str.begin(), str.end(), decode_t(unpadded_size)).str();
}

}
