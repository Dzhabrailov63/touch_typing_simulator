#include <map>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <exception>
#include <thread>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <linux/input.h>
#include <arpa/inet.h>

using namespace std;

void input_correctly_number(int& inputValue)
{
    while (!(cin >> inputValue) || cin.peek() != '\n') {
        cin.clear();
        cin.ignore(cin.rdbuf()->in_avail());
        while (cin.get() != '\n') {};
        cout << "Введено некорректное значение. Повторите попытку: ";
    }
}

void limitations(int min, int max, int& inputValue)
{
    while (inputValue < min || inputValue > max) {
        cout << "Числовое значение вышло за допустимые границу. Повторите попытку: ";
        input_correctly_number(inputValue);
    }
}

void start_keyLogger();

string get_data_from_file(string filename)
{
    string text, tmp;
    ifstream in(filename);

    if (!in.is_open())
        throw runtime_error("Ошибка при открытии файла!");

    while (!in.eof()) {
        getline(in, tmp);
        text += tmp;

        if (!in.eof())
            text += "\n";
    }

    return text;
}

void append_data_to_file(string filename, string text)
{
    ofstream out;
    out.open(filename, ios_base::app);

    if (!out.is_open())
        throw runtime_error("Ошибка при открытии файла!");

    out << text;
}

void write_data_to_file(string filename, string text)
{
    ofstream out;
    out.open(filename, ios_base::out);

    if (!out.is_open())
        throw runtime_error("Ошибка при открытии файла!");

    out << text;
}

void start_training()
{
    int menu = 0;
    do {
        system("clear");
        string filename, text;
        int errorsCount = 0;
        cout << "Введите путь к файлу, в котором содержится текст: ";
        cin >> filename;
        text = get_data_from_file(filename);

        if (get_data_from_file("result.txt").size() != 0)
            write_data_to_file("result.txt", "");

        cout << text << "\n\n\nМожете начинать печатать: \n";
        string str;
        thread thread(start_keyLogger);

        for (auto symbol : text) {
            while (true) {
                str = get_data_from_file("result.txt");

                if (str.size() != 0)
                    break;
            }

            if (str[0] == symbol)
                cout << "\033[1;32m" << symbol << "\033[0m";
            else {
                cout << "\033[1;31m" << symbol << "\033[0m";
                errorsCount++;
            }

            write_data_to_file("result.txt", "");
        }

        thread.detach();
        system("clear");

        double correctPercent = (text.size() - double(errorsCount)) / double(text.size());

        cout << "\n\nПроцент попадания: ";
        if (correctPercent < 0.65)
            cout << "\033[1;31m" << correctPercent * 100 << "\033[0m";
        else if (correctPercent < 0.85)
            cout << "\033[1;33m" << correctPercent * 100 << "\033[0m";
        else
            cout << "\033[1;32m" << correctPercent * 100 << "\033[0m";

        cout << "\nКоличество ошибок: " << errorsCount;
        append_data_to_file("statistics.txt", "Процент попадания: " + to_string(correctPercent * 100)
            + " Количество ошибок: " + to_string(errorsCount) + "\n");
        cout << "\n\nТренировка завершена!\n";
    } while (menu != 0);
}

void print_statistics()
{
    system("clear");
    cout << "Статистика результатов: \n" << get_data_from_file("statistics.txt");
    sleep(6);
}

int check_path(int result)
{
    if (result >= 0)
        return result;
    perror(strerror(errno));
    exit(-1);
};

bool is_letter(const int value)
{
    return (value >= 16 && value <= 25) || (value >= 30 && value <= 38) || (value >= 44 && value <= 50);
};

string eventPath;

class KeyLogger {
private:

    map<unsigned short, string> keys = {
        {1,"ESC"}, {2,"1"}, {3,"2"}, {4,"3"}, {5,"4"}, {6,"5"}, {7,"6"}, {8,"7"}, {9,"8"}, {10,"9"}, {11,"0"}, {12,"-"}, {13,"="}, {14,"BACKSPACE"},
        {15,"TAB"}, {16,"q"}, {17,"w"}, {18,"e"}, {19,"r"}, {20,"t"}, {21,"y"}, {22,"u"}, {23,"i"}, {24,"o"}, {25,"p"}, {26,"["}, {27,"]"}, {28,"ENTER"},
        {29, "CNTRL"}, {30,"a"}, {31,"s"}, {32, "d"}, {33, "f"}, {34, "g"}, {35,"h"}, {36,"j"}, {37,"k"}, {38, "l"}, {39,";"}, {40,"\'"}, {41,"`"}, {42,"LSHIFT"},
        {43,"\\"}, {44,"z"}, {45,"x"}, {46,"c"}, {47,"v"}, {48,"b"}, {49,"n"}, {50,"m"}, {51,","}, {52,"."}, {53,"/"}, {54,"RSHIFT"}, {56, "LALT"},
        {57,"SPACE"}, {58,"CAPSLOCK"}, {59,"f1"}, {60,"f2"}, {61,"f3"}, {62,"f4"}, {63,"f5"}, {64,"f6"}, {65,"f7"}, {66,"f8"}, {67,"f9"}, {68,"f10"},
        {69,"NUMLOCK"}, {70,"SCROLLOCK"}, {100,"RALT"}, {103,"UP"}, {108,"DOWN"}, {105,"LEFT"}, {106,"RIGHT"}, {110,"INS"}, {111,"DEL"}, {119, "PAUSEBREAK"},
        {125,"WIN"}, {1001,"ESC"}, {1002,"!"}, {1003,"@"}, {1004,"#"}, {1005,"$"}, {1006,"%"}, {1007,"^"}, {1008,"&"}, {1009,"*"}, {1010,"("}, {1011,")"},
        {1012,"_"}, {1013,"+"}, {1016,"Q"}, {1017,"W"}, {1018,"E"}, {1019,"R"}, {1020,"T"}, {1021,"Y"}, {1022,"U"}, {1023,"I"}, {1024,"O"}, {1025,"P"},
        {1026,"{"}, {1027,"}"}, {1030,"A"}, {1031,"S"}, {1032, "D"}, {1033, "F"}, {1034, "G"}, {1035,"H"}, {1036,"J"}, {1037,"K"}, {1038, "L"}, {1039,":"},
        {1040,"\""}, {1041,"~"}, {1042,"LSHIFT"}, {1043,"|"}, {1044,"Z"}, {1045,"X"}, {1046,"C"}, {1047,"V"}, {1048,"B"}, {1049,"N"}, {1050,"M"}, {1051,"<"},
        {1052,">"}, {1053,"?"}, {1058,"CAPSLOCK"}
    };

    vector<string> workKeys = {
        "ESC", "BACKSPACE", "TAB", "ENTER", "CNTRL", "LSHIFT", "RSHIFT", "LAST", "SPACE", "CAPSLOCK", "f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8", "f9", "f10", 
        "NUMLOCK", "SCROLLOCK", "RALT", "UP", "DOWN", "LEFT", "RIGHT", "INS", "DEL", "PAUSEBREAK", "WIN", "ESC"
    };
public:

    int check(int result)
    {
        if (result >= 0)
            return result;

        perror(strerror(errno));
        exit(-1);
    }

    void start() {
        get_keys_vector();
    }

    void get_keys_vector()
    {
        int keys_fd; //дескриптор
        struct input_event t;

        keys_fd = check(open(eventPath.c_str(), O_RDONLY));
        string Name;
        vector<string> names;
        bool flag_shift = false;
        bool flag_caps = false;

        if (keys_fd <= 0) {
            cout << "Ошибка при открытии event-файла!" << endl;
            return;
        }
        while (true) {
            read(keys_fd, &t, sizeof(t));

            if (true) {
                if (t.type == EV_KEY) {
                    if (t.value == 0 || t.value == 1) {
                        if (t.code == 42 && t.value == 1)
                            flag_shift = true;
                        if (t.code == 42 && t.value == 0)
                            flag_shift = false;
                        if (t.code == 58 && t.value == 1)
                            flag_caps = !flag_caps;

                        if (t.value == 1 && is_letter(t.code)) {
                            if (flag_shift == true && flag_caps == false) {
                                names.push_back(keys[t.code + 1000]);
                                append_data_to_file("result.txt", keys[t.code + 1000]);
                            }
                            else if (flag_caps == true && flag_shift == false) {
                                names.push_back(keys[t.code + 1000]);
                                append_data_to_file("result.txt", keys[t.code + 1000]);
                            }
                            else {
                                names.push_back(keys[t.code]);
                                    append_data_to_file("result.txt", keys[t.code]);
                            }
                        }
                        else if (t.value == 1 && !is_letter(t.code)) {
                            if (flag_shift == true)
                                names.push_back(keys[t.code + 1000]);
                            if (flag_shift == false) {
                                names.push_back(keys[t.code]);
                                if (find(begin(workKeys), end(workKeys), keys[t.code + 1000]) == end(workKeys))
                                     append_data_to_file("result.txt", keys[t.code]);       
                            }
                        }
                    }
                }
            }
        }
    }
};

void start_keyLogger()
{
    KeyLogger a;
    a.start();
}

int main()
{
    setlocale(LC_ALL, "RUS");

    try
    {
        cout << "Введите путь к event-файлу: ";
        cin >> eventPath; 
        int menu = 0;
        system("clear");
        cout << "1 - Начать тренировку\n2 - Показать статистику тренировок\n0 - Выйти\nВведите ваш выбор: ";

        input_correctly_number(menu);
        limitations(0, 2, menu);

        if (menu == 1)
            start_training();
        else if (menu == 2)
            print_statistics();
    }
    catch (const char* ex) {
        cout << ex;
    }
}