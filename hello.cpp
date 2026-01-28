#include <iostream>
#include <string>
#include <cassert>
#include <fstream>
#include <stdexcept>
#include <filesystem>
#include <ctime>
#include <vector>
#include <ncurses.h>
#include <cstdio>

namespace fs = std::filesystem;
using namespace std;
int MAX_CHARACTERS_NAME = 20;
string FORBIDEN_CHARACTERS = "!?,.%:;'&#@";
int choice = 1;
int input = 1;
int back = 1;
string folder_path = "/Users/egor/Documents/1 Project/Pilots";
string file_path = "/Users/egor/Documents/1 Project/Pilots/pilots.txt";
string temp_path = "/Users/egor/Documents/1 Project/Pilots/tempilots.txt";
vector<string> pilots;




void logEvent(string importance, string message){
    time_t result = time(NULL);
    string time_str = ctime(&result);
    ofstream file("/Users/egor/Documents/1 Project/logs.txt", ios::app);
    if (file.is_open()){
        file << "[" << time_str << "]" << "[" << importance << "] " << message << endl;
        file.close();
    }  
}
void back_to_menu(){
    string answer;
    while(back == 1){
    cout << "Вернуться в главное меню?" << endl;
    getline(cin, answer);
    try{
        if(answer == "Да" || answer == "да"){
            system("clear");
            logEvent("INFO", "Открытие главного меню");
            choice = 1;
            break;
        }
        else if(answer == "Нет" || answer == "нет"){
            logEvent("INFO", "Завершение программы");
            choice = 0;
            break;
        }
        else{
            system("clear");
            throw runtime_error("Принимаем только 'Да/Нет'");
            back = 0;
            break;
        }
    }
    catch(const exception& e){
        cerr << "Ошибка! " << e.what() << endl; 
        back = 1;
    }
    }
}
void add_to_list(){
    pilots.clear();
    int c = 0;
    string line;
    ifstream list_file(file_path);
    if (list_file.is_open()){
        while(getline(list_file, line)){
            c++;
            pilots.push_back(line);
        }
    }

}
bool validate_id(string& id){
    int q = 0;
    for (char c : id){
        if (isdigit(c)){
            q += 1;
        }
    }
    if (q == id.length()){
        if (stoi(id) > 0){
            if (static_cast<size_t>(stoi(id)) > pilots.size()){
                throw invalid_argument("Вы вышли за рамки допустимых значений!");
            }
        }
        if (stoi(id) <= 0){
            throw invalid_argument("Вы вышли за рамки допустимых значений!");
        }
    }
    else{
        throw invalid_argument("Некорректный ввод!");
    }
    return true;
}

void remove_from_list(){
    ifstream file(file_path);
    ofstream temp_file(temp_path, ios::app);
    string num_pilot;
    string delete_me;
    int c = 1;
    string line;
    ifstream list_file(file_path);
    cout << "Напишите порядковый номер пилота, которого хотите уволить нахуй" << endl;
    logEvent("INFO", "Введен ID пилота для удаления");
    
    getline(cin, num_pilot);
    try{
        if (validate_id(num_pilot)){
            delete_me = pilots[stoi(num_pilot)-1];
            if (temp_file.is_open() && file.is_open()){
                while(getline(file, line)){
                    if (line != delete_me){
                        temp_file << line << endl;
                    }
                }
            }
            temp_file.close(); file.close();
            logEvent("INFO", "Удален текстовый файл 'pilots.txt'");
            logEvent("INFO", "'tempilots.txt' заменен на pilots.txt'");
            remove("/Users/egor/Documents/1 Project/Pilots/pilots.txt");
            rename("/Users/egor/Documents/1 Project/Pilots/tempilots.txt", "/Users/egor/Documents/1 Project/Pilots/pilots.txt");

            cout << "Успешно! Пилот с номером " << num_pilot << " был удален из реестра" << endl;
            back_to_menu();
        }
    }
    catch(const exception& e){
        cerr << "Ошибка! " << e.what() << endl;
        system("clear");
        choice = 1;
    }
       
}
    
void list(){
    
    add_to_list();
    for (int i = 0; i < pilots.size(); i++){
        cout << i+1 << ")" << pilots[i] << endl;
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

bool check_duplicates(const string& find_name){
    for (size_t i = 0; i < pilots.size(); i++){
        if (find_name == pilots[i]){
            throw runtime_error("Такое имя уже существует в реестре!");
        }
    }
    
    return true;
    back_to_menu();
}
void input_pilot(){
    string name; string answer;
    while(input == 1){
        cout << "Введите имя: " << endl;
        getline(cin, name);
        try{
            if (validate(name) && check_duplicates(name)){
                writen(name);
                logEvent("INFO", "Новый пилот добавлен в реестр.");
                cout << "Система приветствует, " << name << ". Инициализация протокола 'СП ТХС'" << endl;
                cout << "Добавляем еще нового пользователя?" << endl;
                getline(cin, answer);
                if(answer == "Да" || answer == "да"){
                    logEvent("INFO", "Новый процесс добавления пилота");
                    input_pilot();
                    input = 1;
                    break;
                }
                else if(answer == "Нет" || answer == "нет"){
                    system("clear");
                    logEvent("INFO", "Возврат в главное меню");
                    input = 0;
                    choice = 1;
                    break;
                }
                else{
                    system("clear");
                    throw runtime_error("Принимаем только 'Да/Нет'");
                    break;
                    
                }
            }
        }
        catch(const exception& e){
            cerr << "Ошибка! " << e.what() << endl;
            system("clear");
            choice = 1;
            break;
            
        }
    }
}




int main(){
//Интерактив
    add_to_list();
    while (choice == 1){
        int point;
        string str_point;
        cout << "WELCOME TO TXC SYSTEM" << endl;
        cout << "1. Вывести список пилотов " << endl << "2. Добавить пилота " << endl << "3. Удалить пилота " << endl << "4.Выход " << endl;
        try{ 
            getline(cin, str_point);
            point = stoi(str_point);
            switch(point)
            {
            case 1:
                system("clear");
                logEvent("INFO", "Выведен список реестра пилотов");
                list();
                back_to_menu();
                break;
            case 2:
                input = 1;
                input_pilot();
                break;
            case 3:
                remove_from_list();
                break;
                
            case 4:
                logEvent("INFO", "Завершение программы");
                choice = 0;
                break;
            default:
                logEvent("ERROR", "Обнаружение ошибки неизветсного действия!!!");
                system("clear");
                cerr << "Ошибка! Неправильное действие!" << endl;   
        }
        
        }
        catch(...){
            logEvent("ERROR", "Обнаружение ошибки неизветсного действия!!!");
            system("clear");
            cerr << "Ошибка! Неправильное действие!" << endl;
            
        }
        
    }
    return 0;
}
