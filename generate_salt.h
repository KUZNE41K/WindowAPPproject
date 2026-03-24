#include <iostream>
#include <string>
#include <openssl/rand.h>
#include <vector>
#include "ReadeKey.h"

std::string generate_salt(size_t length = 20)
{
	std::vector<unsigned char> bufer(length);

	if (RAND_bytes(bufer.data(), length) != 1)
	{
		throw std::runtime_error("Error");
	}
	const std::string chars = key();

	std::string salt;
	salt.reserve(length);

	for (size_t i = 0; i < length; i++)
	{
		salt += chars[bufer[i] % chars.size()];
	}
	return salt;
}