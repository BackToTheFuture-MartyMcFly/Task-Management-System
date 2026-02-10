#include <iostream>
#include "User.h"
#include "TextFileDataBase.h"
#include "Logger.h"


int main()
{
    // переработать использование логгера
    Logger::init("UserSrvice");

    Logger::getGlobalLogger()->info("HELLO");
    Logger::getServiceLogger()->info("Hello");

    system("PAUSE");
}
