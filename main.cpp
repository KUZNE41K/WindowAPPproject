#include <iostream>
#include <fstream>
#include<string>
#include "generate_salt.h"
#include "Hash.h"

int main()
{
	std::string salt = generate_salt();
	std::cout << salt;

	std::string password = "jfbvvjfn";
	std::cout << salt << "\n";
	std::cout << password << "\n";

	std::string Hash = hashPasswordWithSalt(password,salt);


	std::cout << Hash;
}