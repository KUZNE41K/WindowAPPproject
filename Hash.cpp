#include "Hash.h"

static std::vector<uint8_t> generateSalt(size_t size = 16)
{
	std::vector<uint8_t> salt(16);
	std::random_device rd;
	
	for (size_t i = 0; i < size; ++i) {
		salt[i] = static_cast<uint8_t>(rd());
	}
	return salt;
}



std::string Hash::hash(const std::string& password)
{
	uint32_t t_cost = 3; // Time cost
	uint32_t m_cost = 1 << 16; // Memory cost (64 MB)
	uint32_t parallelism = 1; // Parallelism

	std::vector<uint8_t> salt = generateSalt();
	char hash_encoded[128];

	int result = argon2id_hash_encoded(t_cost,
		m_cost,
		parallelism,
		password.c_str(),
		password.size(),
		salt.data(),
		salt.size(),
		32,
		hash_encoded,
		sizeof(hash_encoded));

	if (result != ARGON2_OK)
	{
		throw std::runtime_error("Hashing failed");
	}
	return std::string(hash_encoded);
}

bool Hash::verifyPassword(const std::string& password, const std::string& hash)
{
	return argon2id_verify(hash.c_str(),password.c_str(),password.length()) == ARGON2_OK;
}
