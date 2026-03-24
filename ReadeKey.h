#include <fstream>
#include<string>
#include <iostream>



std::string key()
{
	std::string line;

	std::ifstream key("public.pem");

	if (key.is_open())
	{
		std::getline(key, line);
	}
	return line;
}
