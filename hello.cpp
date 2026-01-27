#include <iostream>
#include <string>
#include <cassert>
#include <fstream>
#include <stdexcept>
#include <filesystem>
#include <ctime>


namespace fs = std::filesystem;
using namespace std;
int MAX_CHARACTERS_NAME = 20;
string FORBIDEN_CHARACTERS = "!?,.%:;'&#@";
int choice = 1;
string folder_path = "/Users/egor/Documents/1 Project/Pilots";
string file_path = "/Users/egor/Documents/1 Project/Pilots/pilots.txt";

void list(){
    int c = 1;
    string line;
    ifstream list_file(file_path);
    if (list_file.is_open()){
        while(getline(list_file, line)){
            cout << c << ")" << line << endl;
            c++;
        }
    }
}
void logEvent(string importance, string message){
    time_t result = time(NULL);
    string time_str = ctime(&result);
    ofstream file("/Users/egor/Documents/1 Project/logs.txt", ios::app);
    if (file.is_open()){
        file << "[" << time_str << "]" << "[" << importance << "] " << message << endl;
        file.close();
    }  
}
bool validate(const string& text){
    if (text.length() > MAX_CHARACTERS_NAME){
        logEvent("ERROR", "Введена слишком большая длина имени!");
        throw runtime_error("Длина имени не должна превышать 20 символов!" ); 
    }
    if(text.empty()){
        logEvent("ERROR", "Введено постое имя!");
        throw runtime_error("Имя не может быть пустое, ты тупой?");
    }
    if(text.find_first_of(FORBIDEN_CHARACTERS) != string::npos){
        logEvent("ERROR", "Введены специальные (запрещенные) символы!");
        throw runtime_error("Убери специальные символы из имени!"); 
    }
    return true;
}

void writen(const string& name){
    
    fs::create_directory(folder_path);

    ofstream file(file_path, ios::app);
    file << name << endl;
    file.close();
}
void next_user(){
    string answer;
    cout << "Добавляем нового пользователя?" << endl;
    getline(cin, answer);
    try{
        if(answer == "Да" || answer == "да"){
            choice = 1;
            logEvent("INFO", "Новый процесс добавления пилота");
        }
        else if(answer == "Нет" || answer == "нет"){
            choice = 0;
            logEvent("INFO", "Завершение программы");
        }
    }
    catch(const exception& e){
        throw runtime_error("Принимаем только 'Да/Нет'");
    }
}
void back_to_menu(){
    string answer;
    cout << "Вернуться в главное меню?" << endl;
    getline(cin, answer);
    try{
        if(answer == "Да" || answer == "да"){
            choice = 1;
            logEvent("INFO", "Открытие главного меню");
        }
        else if(answer == "Нет" || answer == "нет"){
            choice = 0;
            logEvent("INFO", "Завершение программы");
        }
    }
    catch(const exception& e){
        throw runtime_error("Принимаем только 'Да/Нет'");
    }
}
int main(){
//Интерактив
    while (choice == 1){
        int point;
        string str_point;
        string name;
        cout << "WELCOME TO TXC SYSTEM" << endl;
        cout << "1. Вывести список пилотов " << endl << "2. Добавить пилота " << endl << "3. Удалить пилота " << endl << "4.Выход " << endl;
        
        try{ 
            getline(cin, str_point);
            point = stoi(str_point);
            switch(point)
            {
            case 1:
                list();
                back_to_menu();
                break;
            case 2:
                while(true){
                    cout << "Введите имя: " << endl;
                    getline(cin, name);
                    try{
                        if (validate(name)){
                            writen(name);
                            logEvent("INFO", "Новый пилот добавлен в реестр.");
                            cout << "Система приветствует, " << name << ". Инициализация протокола 'СП ТХС'" << endl;
                            next_user();
                            break;
                        }
                    }
                catch(const exception& e){
                    cerr << "Ошибка! " << e.what() << endl;
                }
                }
                
                
            case 4:
                logEvent("INFO", "Завершение программы");
                choice = 0;
                break;

            default:
                logEvent("ERROR", "Обнаружение неизвестной ошибки!!!");
                cerr << "Неизвестная ошибка!" << endl;
        }
        
        }
        catch(...){
            cerr << "Ошибка! Неправильное действие!" << endl;
        }
        
    }
    return 0;
}