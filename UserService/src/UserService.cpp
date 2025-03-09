
#include <iostream>
#include "User.h"
#include "TextFileDataBase.h"

int main()
{
    User user;

    TextFileDataBase db;
    if (!db.connect("D:/C++_projects/TaskManagementSystem/UserService/DataBase.txt"))
        std::cout << "Error!" << std::endl;
    else {
        user.setId(1);
        user.setName("qqwqe");
        user.setEmail("qwqe@fvg.ru");
        user.setPasswordHash("qqeeq");
        db.createUser(user);

        user.setId(2);
        user.setName("qqwww");
        user.setEmail("reg@jg.ru");
        user.setPasswordHash("cvvv");
        db.createUser(user);

        user.setId(3);
        user.setName("ffdsf");
        user.setEmail("regggggjg@.ru");
        user.setPasswordHash("csdfbvv");
        db.createUser(user);

        user.setId(4);
        user.setName("1111");
        user.setEmail("2121ggjg@.ru");
        user.setPasswordHash("c123vv");
        db.createUser(user);

        user.setId(5);
        user.setName("qqwqe");
        user.setEmail("2121ggjg@.ru");
        user.setPasswordHash("c123vv");
        db.createUser(user);

        User user2(1, "Hello", "hello@mail.ru");
        user2.setPasswordHash("qwerty");
        db.deleteUserFromId(2);
        db.updateUserFromId(1,user2);

        user.setId(5);
        user.setName("vova");
        user.setEmail("vov@qqq.ru");
        user.setPasswordHash("qwerty");
        db.createUser(user);
    
        db.disconnect();
    }


    system("PAUSE");
}
