#include <iostream>
#include <string>
#include <cassert>
#include <fstream>

using namespace std;

int main(){
    cout << "Введите никнейм:" << endl;
    string name;
    getline(cin, name);

    ofstream file("heroes.txt");
    if (file.is_open()){
        file << name;
        file.close();
        cout << "Система приветствует, " << name << ". Инициализация протокола 'СП ТХС'" << endl;
    }
}