#include "AuthService.h"
#include "Connection.h"
#include "ValidationHandler.h"
#include "UserFetchHandler.h"
#include "UserNotExistsHandler.h"
#include "CreateUserHandler.h"
#include "UserExistsHandler.h"
#include "PasswordCheckHandler.h"
#include "CreateSessionHandler.h"
#include <jwt-cpp/jwt.h>
#include <jwt-cpp/traits/kazuho-picojson/defaults.h>
#include <nlohmann/json.hpp> 


AuthService::AuthService(std::shared_ptr<UserRepository> userRepository): userRepository_(userRepository)
{
}


bool AuthService::registerUser(std::string user, std::string email, std::string password)
{

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

AuthService::TokenValidationResult AuthService::validateToken(const std::string& accessToken, const std::string& refreshToken)
{
    CreateSessionHandler handler;

	TokenValidationResult result;
	result.isValid = false;

    std::cout << "=== validateToken ===" << std::endl;
    std::cout << "AccessToken: " << (accessToken.empty() ? "empty" : accessToken.substr(0, 50) + "...") << std::endl;
    std::cout << "RefreshToken: " << (refreshToken.empty() ? "empty" : refreshToken) << std::endl;

    if (accessToken.empty()) {
        result.errorMessage = "Access token is empty.";
        return result;
    }

    if (refreshToken.empty())
    {
		result.errorMessage = "Refresh token is empty.";
		return result;
    }

	auto session = userRepository_->findSessionByToken(refreshToken);
    if (!session.has_value())
    {
        result.errorMessage = "Invalid access token.";
        std::cout << "Session not found for hashed token" << std::endl;
		return result;
    }

    std::cout << "Session found for user: " << session->userId << std::endl;

	std::string userId = extractUserIdFromToken(accessToken);
    std::cout << "Extracted userId from access token: " << (userId.empty() ? "empty" : userId) << std::endl;
    if (userId.empty())
    {
		result.errorMessage = "Invalid access token: user ID not found.";
		return result;
    }

    if (session->userId != userId)
    {
		result.errorMessage = "Refresh token does not match user ID.";
        std::cout << "Mismatch: session.userId=" << session->userId << ", token.userId=" << userId << std::endl;
		return result;
    }
    std::cout << "Generate token";
    std::string newAccessToken = handler.generateJwtToken(userId);
	std::string newRefreshToken = handler.generate();
	std::string hashedNewRefreshToken = Hash::hashToken(newRefreshToken);

	bool rotated = userRepository_->rotateRefreshToken(userId, refreshToken, hashedNewRefreshToken, 2592000); // 30 дней
    if(!rotated)
    {
        result.errorMessage = "Failed to rotate refresh token.";
        return result;
	}
    result.isValid = true;
    result.userId = userId;
    result.accessToken = newAccessToken;
    result.refreshToken = newRefreshToken;
    std::cout << "Validation successful, new tokens generated" << std::endl;
	return result;
}

std::string AuthService::extractUserIdFromToken(const std::string& accessToken)
{
    try {
        std::cout << "=== extractUserIdFromToken ===" << std::endl;
        std::cout << "AccessToken length: " << accessToken.length() << std::endl;
        
        // Декодируем JWT
        auto decoded = jwt::decode(accessToken);
        
        // Получаем payload как JSON строку
        std::string payload = decoded.get_payload();
        std::cout << "Decoded payload: " << payload << std::endl;
        
        // Парсим JSON вручную
        auto json = nlohmann::json::parse(payload);
        
        // Извлекаем sub
        if (json.contains("sub")) {
            std::string userId = json["sub"].get<std::string>();
            std::cout << "Extracted userId from JSON: " << userId << std::endl;
            return userId;
        }
        
        if (json.contains("user_id")) {
            std::string userId = json["user_id"].get<std::string>();
            std::cout << "Extracted userId from JSON: " << userId << std::endl;
            return userId;
        }
        
        std::cout << "No user ID in JSON" << std::endl;
        return "";
        
    } catch (const std::exception& e) {
        std::cerr << "Error extracting user ID: " << e.what() << std::endl;
        return "";
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
        result.userId = loginRequest.userId_;
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



