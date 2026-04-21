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


bool AuthService::registerUser(std::string user, std::string email, std::string password)
{
    std::cout << "=== AuthService::registerUser ===" << std::endl;
    std::cout << "User: " << user << std::endl;
    std::cout << "Email: " << email << std::endl;

    // Используем существующий userRepository_, а не создаем новый!
    if (!userRepository_) {
        std::cout << "ERROR: userRepository_ is null!" << std::endl;
        return false;
    }

    std::cout << "Using existing userRepository_" << std::endl;

    // Создаем хендлеры с существующим репозиторием
    auto registrationValidation = std::make_shared<ValidationHandler>();
    auto regFetch = std::make_shared<UserFetchHandler>(userRepository_);
    auto regNotExists = std::make_shared<UserNotExistsHandler>();
    auto createUser = std::make_shared<CreateUserHandler>(userRepository_);

    registrationValidation->setNext(regFetch);
    regFetch->setNext(regNotExists);
    regNotExists->setNext(createUser);

    Request registrationRequest(user, email, password);
    registrationValidation->handle(registrationRequest);

    std::cout << "After chain - success: " << registrationRequest.success_ << std::endl;
    std::cout << "After chain - errorMessage: " << registrationRequest.errorMessage_ << std::endl;

    if (registrationRequest.success_)
    {
        std::cout << "User registered successfully!" << std::endl;
        return true;
    }
    else
    {
        std::cout << "Registration failed: " << registrationRequest.errorMessage_ << std::endl;
        return false;
    }
}

AuthService::LoginResult AuthService::login(std::string login, std::string password)
{
	LoginResult result;
	result.success = false;

	if (!userRepository_) {
		std::cout << "UserRepository is null!" << std::endl;
		return result;
	}


	auto loginValidation = std::make_shared<ValidationHandler>();
	auto loginFetch = std::make_shared<UserFetchHandler>(userRepository_);
	auto loginExists = std::make_shared<UserExistsHandler>();
	auto passwordCheck = std::make_shared<PasswordCheckHandler>();
	auto createSession = std::make_shared<CreateSessionHandler>(userRepository_); // генерирует JWT и сохраняет в БД

	loginValidation->setNext(loginFetch);
	loginFetch->setNext(loginExists);
	loginExists->setNext(passwordCheck);
	passwordCheck->setNext(createSession);

	Request loginRequest(login, password); // логин и пароль

	// запускаем цепочку
	loginValidation->handle(loginRequest);

	if (loginRequest.success_)
	{
		result.success = true;
		result.accessToken = loginRequest.jwtToken_;
		result.refreshToken = loginRequest.refresh_token_;

		std::cout << "Login success! JWT: " << loginRequest.refresh_token_ << std::endl;
	}
	else
	{
		result.errorMessage = loginRequest.errorMessage_;
		std::cout << "Login failed: " << loginRequest.errorMessage_ << std::endl;
	}
	return result;
}
