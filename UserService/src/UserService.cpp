
#include <iostream>
#include "User.h"
#include "TextFileDataBase.h"

int main()
{
    //User user(4, "Sergio", "ser@mail.ru");
    //user.setPasswordHash("qwerty");
    
    TextFileDataBase db;
    if (!db.connect("D:/C++_projects/TaskManagementSystem/UserService/DataBase.txt"))
        std::cout << "Error!" << std::endl;
    else {
        //db.createUser(user);
        db.deleteUserFromId(2);
    
        db.disconnect();
    }


    system("PAUSE");
}
