#include <iostream>
#include "User.h"
#include "TextFileDataBase.h"
#include "Logger.h"

int main()
{
    // переработать использование логгера
    LOG_INIT("UserService");

    LOG_ERROR("Hello");

    //GLOBAL_LOG_ERROR("HELLO WORLD");
    //LOG_ERROR("HELLO");
    //LOG_INFO("HELLO");

    system("PAUSE");
}
