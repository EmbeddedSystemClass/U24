#pragma once

#include <string>

class Base64
{
public:
	Base64(void);
	~Base64(void);

	/**
	 * Encode string to base64 string.
	 *
	 * @param bytes_to_encode The original string to be encoded.
	 * @param in_len The string length.
	 *
	 * @return The string encoded by base64.
	 */
	static std::string Base64Encode(unsigned char const* bytes_to_encode, unsigned int in_len);
	
	/**
	 * Decode base64 string to string.
	 *
	 * @param encoded_string The string encoded by base64.
	 *
	 * @return The original string.
	 */
	static std::string Base64Decode(std::string const& encoded_string);

};
