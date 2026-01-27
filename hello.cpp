#include <iostream>
#include <string>
#include <cassert>
#include <fstream>
#include <stdexcept>
#include <filesystem>

namespace fs = std::filesystem;
using namespace std;
int MAX_CHARACTERS_NAME = 20;
string FORBIDEN_CHARACTERS = "!?,.%:;'&#@";
int choice = 1;

bool validate(const string& text){
    if (text.length() > MAX_CHARACTERS_NAME){
        throw runtime_error("Длина имени не должна превышать 20 символов!" );
    }
    if(text.empty()){
        throw runtime_error("Имя не может быть пустое, ты тупой?");
    }
    if(text.find_first_of(FORBIDEN_CHARACTERS) != string::npos){
        throw runtime_error("Убери специальные символы из имени!" );
    }
    return true;
}

void writen(const string& name){
    string folder_path = "/Users/egor/Documents/1 Project/Pilots";
    string file_path = "/Users/egor/Documents/1 Project/Pilots/pilots.txt";
    fs::create_directory(folder_path);

    ofstream file(file_path, ios::app);
    if (!file.is_open()){
        throw runtime_error("Файл уже открыт");
    }
    file << name << endl;
    file.close();
}
void next_choice(){
    string answer;
    cout << "Добавляем нового пользователя?" << endl;
    getline(cin, answer);
    try{
        if(answer == "Да" || answer == "да"){
            choice = 1;
        }
        else if(answer == "Нет" || answer == "нет"){
            choice = 0;
        }
    }
    catch(const exception& e){
        throw runtime_error("Принимаем только 'Да/Нет'");
    }
}

int main(){
//Интерактив
    while (choice == 1){
        string name;
        cout << "Введите имя (QUIT — выход из программы): " << endl;
        getline(cin, name);
        if (name == "QUIT"){
            break;
        }
        try{
            if (validate(name)){
                writen(name);
                cout << "Система приветствует, " << name << ". Инициализация протокола 'СП ТХС'" << endl;
                next_choice();
            }
        }
        catch(const exception& e){
            cerr << "Ошибка! " << e.what() << endl;
        }
    }
    return 0;
}