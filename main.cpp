#include "Sha1.h"
#include "Chat.h"

#include <iomanip>

int main()
{
    setlocale(LC_ALL, "");
    std::string _login{};
    std::string _pass{};

    Chat chat;

    bool exit = false;
    int answ;

    chat.reg("Alexey", "123456");
    chat.reg("Viktor", "000000");
    chat.reg("Pavel", "654123");
    chat.reg("Dima", "qwerty");
    chat.reg("Fedor", "qqqqqq");
    chat.reg("Semen", "ytrewq");

    chat.print();

    chat.delUser("Semen");
    chat.delUser("Pavel");
    chat.delUser("Fedor");
    
    chat.print();

    while (!exit)
    {
        std::cout << "\n1 - Регистрация\n2 - Авторизация\n3 - Выход\n";
        std::cin >> answ;
        if (answ == 1)
        {
            std::cout << "Регистрация-введите логин:\n";
            std::cin >> _login;
            std::cout << "Регистрация-введите пароль:\n";
            std::cin >> _pass;
            chat.reg(_login, _pass);
        }
        if (answ == 2)
        {
            std::cout << "\nАвторизация-введите логин:\n";
            std::cin >> _login;
            std::cout << "Авторизация-введите пароль:\n";
            std::cin >> _pass;

            if (chat.login(_login, _pass))
                std::cout << "\nАвторизация прошла успешно\n";
            else
                std::cout << "\nАвторизация не удалась\n";
        }
        if (answ == 3)
            exit = true;
    }

    return 0;
}