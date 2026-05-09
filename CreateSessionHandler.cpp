#include "CreateSessionHandler.h"

CreateSessionHandler::CreateSessionHandler( std::shared_ptr<UserRepository> repo)
	: repo_(std::move(repo))
{
}

CreateSessionHandler::CreateSessionHandler()
{
}

void CreateSessionHandler::handle(Request& request)
{
    auto parentTokenHash = "";
    if (!request.user_->isValid()) {
        request.setSuccess(false);
        request.setErrorMessage("User not found for session creation.");
        return;
    }

    int lifetimeSeconds = 2592000; // 30 дней

    // Генерируем refresh token (UUID)
    std::string rawToken = generate();  // Оригинальный UUID
    request.refresh_token_ = rawToken;   // Отдаем клиенту оригинал

    
    std::string hashedToken = Hash::hashToken(rawToken);

    std::string accessToken = generateJwtToken(std::to_string(request.user_->getId()));
    request.jwtToken_ = accessToken;

    
    bool saved = repo_->saveSession(
        std::to_string(request.user_->getId()),
        hashedToken,  // Хеш, а не rawToken
        parentTokenHash,
        lifetimeSeconds
    );

    if (!saved) {
        request.setSuccess(false);
        request.setErrorMessage("Failed to save session");
        return;
    }

    request.setSuccess(true);

    if (next_) {
        next_->handle(request);
    }
}

std::string CreateSessionHandler::generate()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    uuids::uuid_random_generator uuid_gen(gen);
    uuids::uuid id = uuid_gen();
    return uuids::to_string(id);
}

std::string CreateSessionHandler::generateJwtToken(const std::string& userId)
{
	auto now = std::chrono::system_clock::now();
	auto expires = now + std::chrono::minutes(2); // Token valid for 15 minutes

	auto token = jwt::create()
		.set_issuer("MyApp")
		.set_subject(userId)
		.set_issued_at(now)
		.set_expires_at(expires)
		.sign(jwt::algorithm::hs256{ jwtSecret_ });
	return token;
}

bool CreateSessionHandler::saveSessionToDatabase(const std::string& userId, const std::string& token, const std::string& parentTokenHash, int lifetimeSeconds)
{
    return repo_->saveSession(userId, token, parentTokenHash, lifetimeSeconds);
}
