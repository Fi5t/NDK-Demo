/*
 * base64.h
 *
 *  Created on: 30/04/2011
 *      Author: nicholas
 */

#ifndef BASE64_H_
#define BASE64_H_
#include <string>

namespace base64
{

class encode_t
{
private:
	enum
	{
		zero = 0,
		two,
		four
	} state;
	unsigned int remainder;
	std::string encoded;
public:
	encode_t(std::string::size_type size);
	void operator() (std::string::value_type c);
	std::string str();
};

class decode_t
{
private:
	enum
	{
		zero = 0,
		six,
		four,
		two
	} state;
	unsigned int remainder;
	std::string decoded;
public:
	decode_t(std::string::size_type size);
	void operator() (std::string::value_type c);
	std::string str() const;
};

/*
 * Encode the given string @str into its base64 representation
 */
std::string encode(const std::string& str);

/*
 * Decode the base64 encoded string @str
 */
std::string decode(const std::string& str);

}

#endif /* BASE64_H_ */
