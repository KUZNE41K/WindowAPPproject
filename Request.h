#ifndef REQUEST_H
#define REQUEST_H
#include <string>
#include <memory>
#include "User.h"


class User;

//  ласс Request представл€ет собой структуру данных, котора€ содержит информацию о запросе на регистрацию или аутентификацию пользовател€. 
// ќн включает в себ€ пол€ дл€ хранени€ логина, электронной почты, парол€, соли, а также флаг успеха и сообщение об ошибке.  роме того, он может 
// содержать указатель на объект User, который будет заполнен при успешной аутентификации.
class Request
{
public:
	std::string login_;
	std::string email_;
	std::string password_;
	std::string salt_;
	bool success_ = false;
	std::string errorMessage_;
	std::shared_ptr<User> user_;
	std::string refresh_token_; 
	std::string jwtToken_;
	std::string responseBody_;

	// threads
	std::string title_;
	std::string uuid_;
	int createdId_;
	std::string newTitle_;
	std::string threadId_;

	// messageThread
	int messageId_;
	std::string contentThread_;
	std::string newContentThread_;
	int userId_;


	Request()
		: login_(""), email_(""), password_(""),
		salt_(""), success_(false), errorMessage_(""),
		user_(nullptr), refresh_token_(""), jwtToken_("") {
	}
	// конструктор по умолчанию, который инициализирует все пол€ пустыми строками, устанавливает флаг успеха в false, а указатель на пользовател€ в nullptr.

	Request( const std::string& login,
		const std::string& email,
		const std::string& password
		) : login_(login), email_(email), password_(password),success_(false),errorMessage_(""),user_(nullptr), refresh_token_("") {
	}
	// конструктор, который принимает логин, электронную почту и пароль в качестве аргументов
	// и инициализирует соответствующие пол€. ќстальные пол€ устанавливаютс€ по умолчанию.
	Request(const std::string& login, const std::string& password)
    : login_(login), password_(password), success_(false), errorMessage_(""), user_(nullptr), refresh_token_(""), jwtToken_("") {
	}

	// конструктор дл€ веток
	Request(const std::string& title, const std::string& uuid, int& createdId)
		: title_(title), uuid_(uuid), createdId_(createdId){
	}
	Request(const std::string& threadId)
		: threadId_(threadId){
	}
	static Request ThreadRename(const std::string& threadId, const std::string& newTitle) {
		Request request;
		request.threadId_ = threadId;
		request.newTitle_ = newTitle;
		return request;
	}

	static Request CreateMessageThread(const std::string& threadId, int messageId, int userId, const std::string& contentThread)
	{
		Request request;
		request.threadId_ = threadId;
		request.messageId_ = messageId;
		request.userId_ = userId;
		request.contentThread_ = contentThread;
	}
	static Request DeleteMessageThread(const std::string& threadId, int messageId)
	{
		Request request;
		request.threadId_ = threadId;
		request.messageId_ = messageId;
	}
	static Request UpdateMessageThread(const std::string& threadId, int messageId,const std::string newContentThread)
	{
		Request request;
		request.threadId_ = threadId;
		request.messageId_ = messageId;
		request.newContentThread_ = newContentThread;
	}
	
	~Request() = default;

	void setSuccess(bool success);
	void setErrorMessage(const std::string& errorMessage);
	void setUser(std::shared_ptr<User> user);
	std::shared_ptr<User> getUser() const;
	void setSessionToken(const std::string& jwtToken);

};

#endif // REQUEST_H

