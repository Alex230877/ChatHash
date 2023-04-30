#include "Chat.h"
#include "iostream"
#include "string.h"

Chat::Chat() {
    _data_count = 0;
    _del_data_count = 0;
    _mem_size = 8;
    _data = new UserAuthData[_mem_size];
}

void Chat::reg(const std::string& login, const std::string& pass) {
    uint* _pass_sha1_hash = sha1(pass.c_str(), pass.length());
    addUser(login, _pass_sha1_hash);
    std::cout << "пароль_sha1_хэш = " << _pass_sha1_hash << std::endl;
}

bool Chat::login(const std::string& login, const std::string& pass)
{
    for (int i = 0; i < _mem_size; i++)
    {
        int index = hash_func(login, i);
        if (_data[index]._login == login)
        {
            uint* PasHash = sha1(pass.c_str(), pass.length());
            if (!memcmp(_data[index]._pass_sha1_hash, PasHash, SHA1HASHLENGTHBYTES))
            {
                std::cout << "Добро пожаловать " << _data[index]._login << "!" << std::endl;
                delete[] PasHash;
                return true;
            }
        }
    }
    return false;
}

int Chat::hash_func(const std::string& login, int offset) {
    long sum = 0;
    for (int i = 0; i < login.length(); i++) {
        sum += login[i];
    }
    return (hf_multiply(sum) + offset * offset) % _mem_size;
}

int Chat::hf_multiply(int val) {
    const double A = 0.7;
    return int(_mem_size * (A * val - int(A * val)));
}

void Chat::addUser(std::string login, uint* password)
{
    if (_data_count >= _mem_size)
        resize();

    int index = -1;
    int i = 0;
    // берем пробы по всем i от 0 до размера массива
    for (; i < _mem_size; i++) {
        index = hash_func(login, i);
        if (_data[index]._status == enUserStatus::free)
            break;
    }
    if (i >= _mem_size)
    {
        resize();
        addUser(login, password);
    }
    else
    {
        // найдена пустая ячейка, занимаем ее кладем в свободную ячейку пару
        _data[index] = UserAuthData(login, password);
        _data_count++;
    }
}

void Chat::delUser(const std::string& login)
{
    int index = -1;
    for (int i = 0; i < _mem_size; i++)
    {
        index = hash_func(login, i);

        if (_data[index]._status == engaged && _data[index]._login == login)
        {
            _data[index]._status = deleted;
            _data_count--;
            _del_data_count++;
            std::cout << "Удалить: " << _data[index]._login << std::endl;
            if (_del_data_count >= (_mem_size / 2))
                sizeReduction();
            return;
        }
        else if (_data[index]._status == free)
        {
            std::cout << "Удалить не удалось" << std::endl;
            return;
        }
        else if (_data[index]._status == deleted)
        {
            std::cout << _data[index]._login << " уже удален" << std::endl;
            return;
        }
    }
}

void Chat::resize()
{
    UserAuthData* save_data = _data;
    int oldSize = _mem_size;
    _mem_size *= 2;
    _data_count = 0;
    _data = new UserAuthData[_mem_size];

    for (int i = 0; i < oldSize; i++)
    {
        UserAuthData& old_data = save_data[i];
        if (old_data._status == enUserStatus::engaged)
        {
            uint* sha_hash_copy = new uint[SHA1HASHLENGTHUINTS];
            memcpy(sha_hash_copy, old_data._pass_sha1_hash, SHA1HASHLENGTHBYTES);
            addUser(old_data._login, sha_hash_copy);
        }
    }
    delete[] save_data;
}

void Chat::sizeReduction()
{
    UserAuthData* save_data = _data;
    int oldSize = _mem_size;
    _mem_size /= 2;
    _data_count = 0;
    _data = new UserAuthData[_mem_size];

    for (int i = 0; i < oldSize; i++)
    {
        UserAuthData& old_data = save_data[i];
        if (old_data._status == enUserStatus::engaged)
        {
            uint* sha_hash_copy = new uint[SHA1HASHLENGTHUINTS];
            memcpy(sha_hash_copy, old_data._pass_sha1_hash, SHA1HASHLENGTHBYTES);
            addUser(old_data._login, sha_hash_copy);
        }
    }
    delete[] save_data;
    _del_data_count = 0;
    return;
}

void Chat::print()
{
    std::cout << "\nОтобразить количество пользователей = " << _data_count << std::endl;
    std::cout << "Отобразить количество удалённых пользователей = " << _del_data_count << std::endl;
    for (int i = 0; i < _mem_size; i++)
    {
        std::cout << "Индекс = " << i
            << "\tЛогин = " << _data[i]._login
            << "\tХэш пароля = " << _data[i]._pass_sha1_hash
            << "\tСтатус = " << _data[i]._status << std::endl;
    }
    std::cout << std::endl;
}