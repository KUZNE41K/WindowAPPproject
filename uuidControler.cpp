#include "uuidControler.h"

UuidController::UuidController(std::shared_ptr<CreateSessionHandler> uuidControler) : uuidControler_(uuidControler)
{
}

std::string UuidController::getUuid()
{
	return uuidControler_->generate();
}
