#include "AuthService.h"
#include "Connection.h"
#include "ValidationHandler.h"
#include "UserFetchHandler.h"
#include "UserNotExistsHandler.h"
#include "CreateUserHandler.h"
#include "UserExistsHandler.h"
#include "PasswordCheckHandler.h"
#include "CreateSessionHandler.h"


AuthService::AuthService(std::shared_ptr<UserRepository> userRepository): userRepository_(userRepository)
{
}


void AuthService::registerUser(std::string user,std::string email, std::string password)
{
	auto db = std::make_shared<Connections>();
	db->connectDataBase();

	auto userRepo = std::make_shared<UserRepository>(db);

	// рега
	auto registrationValidation = std::make_shared<ValidationHandler>();
	auto regFetch = std::make_shared<UserFetchHandler>(userRepo);
	auto regNotExists = std::make_shared<UserNotExistsHandler>();
	auto createUser = std::make_shared<CreateUserHandler>(userRepo);


	registrationValidation->setNext(regFetch);
	regFetch->setNext(regNotExists);
	regNotExists->setNext(createUser);

	Request reqistrationRequest(user, email, password);
	registrationValidation->handle(reqistrationRequest);

	if (reqistrationRequest.success_)
	{
		std::cout << "User registered successfully!" << std::endl;
	}
	else
	{
		std::cout << "Registration failed: " << reqistrationRequest.errorMessage_ << std::endl;
	}
}

void AuthService::login(std::string login, std::string password)
{
	auto db = std::make_shared<Connections>();
	db->connectDataBase();

	auto userRepo = std::make_shared<UserRepository>(db);

	auto loginValidation = std::make_shared<ValidationHandler>();
	auto loginFetch = std::make_shared<UserFetchHandler>(userRepo);
	auto loginExists = std::make_shared<UserExistsHandler>();
	auto passwordCheck = std::make_shared<PasswordCheckHandler>();
	auto createSession = std::make_shared<CreateSessionHandler>(userRepo); // генерирует JWT и сохраняет в БД

	loginValidation->setNext(loginFetch);
	loginFetch->setNext(loginExists);
	loginExists->setNext(passwordCheck);
	passwordCheck->setNext(createSession);

	Request loginRequest(login, password); // логин и пароль

	// запускаем цепочку
	loginValidation->handle(loginRequest);

	if (loginRequest.success_)
	{
		std::cout << "Login success! JWT: " << loginRequest.jwtToken_ << std::endl;
	}
	else
	{
		std::cout << "Login failed: " << loginRequest.errorMessage_ << std::endl;
	}
}
