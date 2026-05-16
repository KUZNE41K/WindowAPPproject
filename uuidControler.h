#include <memory>
#include "CreateSessionHandler.h"
#ifndef UUID_CONTROLER_H
#define UUID_CONTROLER_H


class UuidController
{
public:
	UuidController(std::shared_ptr<CreateSessionHandler> uuidControler);

	std::string getUuid();
private:
	std::shared_ptr<CreateSessionHandler> uuidControler_ ;
};
#endif // !UUID_CONTROLER_H

