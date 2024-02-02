// Test_Junior_E_Signature.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include <fstream>
#include <iostream>
#include <windows.h>
#include <string>

#define GREEN_TEXT SetConsoleTextAttribute(hConsole, 10);
#define WHITE_TEXT SetConsoleTextAttribute(hConsole, 15); 
#define RED_TEXT SetConsoleTextAttribute(hConsole, 12); 

using namespace std;
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

INT64 GetLineHash(string line) // кодируем строку
{
    INT64 result = 0;
    int i = 1;
    for (char c : line) // каждый символ в строке складываем и умножаем на i
    {
        result += c * i;
        i++;
    }
    return result;
}

void AddHash(string filename)
{    
    ifstream fin(filename); // открываем файл с данными для чтения
    string out_filename;
    string line;
    unsigned char c;
    char buff[50];

    if (fin.is_open())
    {
        GREEN_TEXT
        cout << "The file has been opened successfully in AddHash! (read)\n";
        WHITE_TEXT

        cout << "Enter the FULL name of the new file to write data + hash: ";    // ввод имени файла куда пишем хэш
        cin >> out_filename;
        
        ofstream fout(out_filename); // создаём и открываем пустой файл для записи хэша

        if (fout.is_open())
        {
            GREEN_TEXT
                cout << "The file has been opened successfully in AddHash! (write)\n";
            WHITE_TEXT
        } 
        else
        {
            RED_TEXT
            cout << "ERROR: failed to create a file";
            exit(0);
        }

        string text;
        while (!fin.eof())  
        {
            getline(fin, line);
            text += line + "\n";    // сохраняем содержимое файла
        }
        fout << text;               // записываем содержимое файла в новый файл
        fout << char(177) << " ";   // начало хэша в последней строке

        fin.clear(); 
        fin.seekg(fin.beg);         // возвращаемся в начало исходного файла для чтения

        while (!fin.eof())
            {
            getline(fin, line);
            INT64 hash_line = GetLineHash(line); // получаем хэш строки
            _i64toa_s(hash_line, buff, 50, 32);  // преобразуем хэш в 32-битную систему
            fout << buff << " ";
            }

        fout << char(177); // конец хэша
        fin.close();
        fout.close();
    }
  }

bool CheckHash(string filename)
{
    ifstream fin(filename);     // открываем файл с данными для чтения
    string line;
    string original_hash;
    string result_hash;
    result_hash += char(177);   // начало хэша
    unsigned char c;
    char buff[50];

    while (getline(fin, line))
    {
        //проходим до последней строке где находится хэш
    }
    original_hash = line;

    fin.clear();
    fin.seekg(fin.beg);     // возвращаемся в начало исходного файла для чтения

    while (!fin.eof())      // пройдусь по всему файлу, а потом удалю последнюю строку из получившегося хэша
    {
        getline(fin, line);
        INT64 hash_line = GetLineHash(line); // снова получаем хэш строки и преобразуем хэш в 32-битную систему
        _i64toa_s(hash_line, buff, 50, 32);  
        result_hash += ' ';
        result_hash += buff;
    }
    result_hash.erase(result_hash.rfind(' ')); // удаляем из хэша последнее значение (хэш самого хэша)

    result_hash += ' ';
    result_hash += char(177);
   
    cout << "Original = " << original_hash << endl;
    cout << "Result   = " << result_hash << endl;

    if (original_hash == result_hash)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool DeleteHash(string filename)
{
    ifstream fin(filename);
    string line;
    string text;
    while (getline(fin, line)) // сохраняем содержимое файла
    {
        text += line + "\n";
    }
    text.erase(text.rfind(line)); // удаляем строку с хэшэм
    text.erase(text.rfind('\n'));
    fin.close();

    fstream fout(filename, ios::out); // перезапись с нуля
    if (fout.is_open())
    {
        fout << text;
        fout.close();
        return true;
    }
    else
    {
        return false;
    }
}

int FindHash(string filename)
{
    ifstream fin(filename); // открываем файл с данными для проверки хэша
    char c;
    int hash_marker = 0;
       
    if (fin.is_open())
    {
        GREEN_TEXT
            cout << "The file has been opened successfully in CheckHash! (read)\n";
        WHITE_TEXT

            while (!fin.eof())
            {
                fin >> c;
                if (c == char(177))
                    hash_marker++;
                if (hash_marker == 2)
                {
                    fin.close();
                    return 1;
                }
            }
        fin.close();
        return 0;
    }
    else
    {
       fin.close();
       return -1;
    }
}

int main()
{   
    setlocale(LC_ALL, "RUS");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    string filename;
    string command;
    bool result;

    while (true)
    {
        cout << "Enter the full path to the file. "
            "\nExample: C:\\Users\\Alex\\Desktop\\test.txt\n";
        
        getline(cin, filename);
         
        switch (FindHash(filename))
        {
        case 0:
        {
            RED_TEXT
            cout << "Hash not found!\n";
            WHITE_TEXT
            cout << "Do you want to add a hash? (yes/no) ";
            cin >> command;
            if (command == "yes")
            {
                AddHash(filename);
            }
            else
            {
                cout << "OK, exit";
                return 0;
            }
            break;
        }
        case 1:
        {
            GREEN_TEXT
            cout << "Hash found!\n";
            WHITE_TEXT
            cout<< "Check the hash? (yes/no) ";
            cin >> command;
            if (command == "yes")
            {
                result = CheckHash(filename);
                if (result)
                {
                    GREEN_TEXT
                        cout << "Hash correct!\n";
                    WHITE_TEXT
                }
                else
                {
                    RED_TEXT
                        cout << "Hash incorrect!\n";
                    WHITE_TEXT
                }
            }
            else
            {
                cout << "Delete hash? (yes/no) "; 
                cin >> command;
                if (command == "yes")
                {
                    result = DeleteHash(filename);
                    if (result == true)
                    {
                        GREEN_TEXT
                            cout << "Deletion is successful!\n";
                        WHITE_TEXT
                    }
                    else
                    {
                        RED_TEXT
                            cout << "Deletion failed!\n";
                        WHITE_TEXT
                    }
                }

            }
            break;
        }
        default:
            RED_TEXT
                cerr << "ERROR. The file cannot be opened\n ";
            WHITE_TEXT
                cin.clear();
                continue;
                break;
        }
        cin.ignore();
    }
}
