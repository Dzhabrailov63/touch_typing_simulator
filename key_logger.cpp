#include <map>
#include <string>
#define DEV_PATH "/dev/input/event1"
#include <vector>
#include <iostream>
#include <linux/input.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <iostream>
#include <errno.h>
#include <string.h>
#include <string>
#include <assert.h>
#include <fstream>
#include <sstream>
#include <exception>
#include <fstream>
#include <string>
#include <map>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
using namespace std;

void input_correctly_number(int& inputValue) 
{
    while (!(cin >> inputValue) || cin.peek() != '\n') 
    {
        cin.clear();
        cin.ignore(cin.rdbuf()->in_avail());
        cout << "Введено некорректное значение. Повторите попытку: ";
    }
}

void Limitations(int min, int max, int& inputValue) 
{
    while (inputValue < min || inputValue > max) 
    {
        cout << "Числовое значение вышло за допустимые границу. Повторите попытку: ";
        input_correctly_number(inputValue);
    }
}

string GetDataFromFile(string filename)
{
    string text, tmp;
    ifstream in(filename);

    if (!in.is_open())
    {
        throw runtime_error("Ошибка при открытии файла!");
    }

    while (!in.eof())
    {
        getline(in, tmp);
        text += tmp;

        if (!in.eof())
        {
            text += "\n";
        }
    }

    return text;
}

void AppendDataToFile(string filename, string text)
{
    ofstream out;
    out.open(filename, ios_base::app); 

    if (!out.is_open())
    {
        throw runtime_error("Ошибка при открытии файла!");
    }

    out << text;
}

int getch() {
    struct termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}

void StartTraining()
{
    int menu = 0;
    do
    {
        system("clear");
        string filename, text;
        int errorsCount = 0;
        cout << "Введите путь к файлу, в котором содержится текст: ";
        cin >> filename;
        text = GetDataFromFile(filename);

        cout << text << "\n\n\nМожете начинать печатать: ";

        int keys_fd; //дескриптор
        keys_fd = open(DEV_PATH, O_RDONLY);

        if (keys_fd < 0)
        {
            perror(strerror(errno));
            exit(-1);
        }

        //HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

        //for (auto symbol : text)
        //{
            //if ((int)symbol == getch())
            //{
                //SetConsoleTextAttribute(hStdOut, FOREGROUND_GREEN);
            //}
            //else
            //{
                //SetConsoleTextAttribute(hStdOut, FOREGROUND_RED);
              //  errorsCount++;
            //}

            //cout << symbol;
        //}

        //SetConsoleTextAttribute(hStdOut, 7);
        

        double correctPercent = (text.size() - double(errorsCount)) / double(text.size());

        cout << "\n\nПроцент попадания: ";
        //SetConsoleTextAttribute(hStdOut, correctPercent < 0.65 ? 12 : correctPercent < 0.85 ? 6 : 10);
        cout << correctPercent * 100 << "%\n\n";
        //SetConsoleTextAttribute(hStdOut, 7);
        cout << "Количество ошибок: " << errorsCount;
        AppendDataToFile("statistics.txt", "Процент попадания: " + to_string(correctPercent * 100) 
            + " Количество ошибок: " + to_string(errorsCount) + "\n");
        cout << "\n\nТренировка завершена!\n1 - Повторить тренировку\n0 - Вернуться в главное меню\nВведите ваш выбор: ";
        input_correctly_number(menu);
        Limitations(0, 1, menu);

    } while (menu != 0);

}

void PrintStatistics()
{
    system("clear");
    cout << "Статистика результатов: \n" << GetDataFromFile("statistics.txt");
    sleep(6);
}

int check_path(int result)
{
    if (result >= 0)
        return result;
    perror(strerror(errno));
    exit(-1);
}
;

bool isLetter(const int value)
{
    if ((value >= 16 && value <= 25) || (value >= 30 && value <= 38) || (value >= 44 && value <= 50))
        return true;
    else
        return false;
}

class Key_logger{
private:
    map<unsigned short, string> keys;
public:
    Key_logger(){
        
        keys.insert({1,"ESC "});
        keys.insert({2,"1 "});
        keys.insert({3,"2 "});
        keys.insert({4,"3 "});
        keys.insert({5,"4 "});
        keys.insert({6,"5 "});
        keys.insert({7,"6 "});
        keys.insert({8,"7 "});
        keys.insert({9,"8 "});
        keys.insert({10,"9 "});
        keys.insert({11,"0 "});
        keys.insert({12,"- "});
        keys.insert({13,"= "});
        keys.insert({14,"BACKSPACE "});
        keys.insert({15,"TAB "});
        keys.insert({16,"q "});
        keys.insert({17,"w "});
        keys.insert({18,"e "});
        keys.insert({19,"r "});
        keys.insert({20,"t "});
        keys.insert({21,"y "});
        keys.insert({22,"u "});
        keys.insert({23,"i "});
        keys.insert({24,"o "});
        keys.insert({25,"p "});
        keys.insert({26,"[ "});
        keys.insert({27,"] "});
        keys.insert({28,"ENTER "});
        keys.insert({29, "CNTRL "});
        keys.insert({30,"a "});
        keys.insert({31,"s "});
        keys.insert({32, "d "});
        keys.insert({33, "f "});
        keys.insert({34, "g "});
        keys.insert({35,"h "});
        keys.insert({36,"j "});
        keys.insert({37,"k "});
        keys.insert({38, "l "});
        keys.insert({39,"; "});
        keys.insert({40,"\' "});
        keys.insert({41,"` "});
        keys.insert({42,"LSHIFT "});
        keys.insert({43,"\\ "}); 
        keys.insert({44,"z "});
        keys.insert({45,"x "});
        keys.insert({46,"c "});
        keys.insert({47,"v "});
        keys.insert({48,"b "});
        keys.insert({49,"n "});
        keys.insert({50,"m "});
        keys.insert({51,", "});
        keys.insert({52,". "});
        keys.insert({53,"/ "});
        keys.insert({54,"RSHIFT "});
        keys.insert({56, "LALT "});
        keys.insert({57,"SPACE "});
        keys.insert({58,"CAPSLOCK "});
        keys.insert({59,"f1 "});
        keys.insert({60,"f2 "});
        keys.insert({61,"f3 "});
        keys.insert({62,"f4 "});
        keys.insert({63,"f5 "});
        keys.insert({64,"f6 "});
        keys.insert({65,"f7 "});
        keys.insert({66,"f8 "});
        keys.insert({67,"f9 "});
        keys.insert({68,"f10 "});
        keys.insert({69,"NUMLOCK "});
        keys.insert({70,"SCROLLOCK "}); 
        keys.insert({100,"RALT "});
        keys.insert({103,"UP "});
        keys.insert({108,"DOWN "});
        keys.insert({105,"LEFT "});
        keys.insert({106,"RIGHT "});
        keys.insert({110,"INS "});
        keys.insert({111,"DEL "});
        keys.insert({119, "PAUSEBREAK"});
        keys.insert({125,"WIN "});

        keys.insert({1001,"ESC"});
        keys.insert({1002,"! "});
        keys.insert({1003,"@ "});
        keys.insert({1004,"# "});
        keys.insert({1005,"$ "});
        keys.insert({1006,"% "});
        keys.insert({1007,"^ "});
        keys.insert({1008,"& "});
        keys.insert({1009,"* "});
        keys.insert({1010,"( "});
        keys.insert({1011,") "});
        keys.insert({1012,"_ "});
        keys.insert({1013,"+ "});
        keys.insert({1016,"Q "});
        keys.insert({1017,"W "});
        keys.insert({1018,"E "});
        keys.insert({1019,"R "});
        keys.insert({1020,"T "});
        keys.insert({1021,"Y "});
        keys.insert({1022,"U "});
        keys.insert({1023,"I "});
        keys.insert({1024,"O "});
        keys.insert({1025,"P "});
        keys.insert({1026,"{ "});
        keys.insert({1027,"} "});

        keys.insert({1030,"A "});
        keys.insert({1031,"S "});
        keys.insert({1032, "D "});
        keys.insert({1033, "F "});
        keys.insert({1034, "G "});
        keys.insert({1035,"H "});
        keys.insert({1036,"J "});
        keys.insert({1037,"K "});
        keys.insert({1038, "L "});
        keys.insert({1039,": "});
        keys.insert({1040,"\" "});
        keys.insert({1041,"~ "});
        keys.insert({1042,"LSHIFT "});
        keys.insert({1043,"| "}); //!!
        keys.insert({1044,"Z "});
        keys.insert({1045,"X "});
        keys.insert({1046,"C "});
        keys.insert({1047,"V "});
        keys.insert({1048,"B "});
        keys.insert({1049,"N "});
        keys.insert({1050,"M "});
        keys.insert({1051,"< "});
        keys.insert({1052,"> "});
        keys.insert({1053,"? "});
        keys.insert({1058,"CAPSLOCK  "});
    }
    
    int check(int result) {
    if (result >= 0)
        return result;


    perror(strerror(errno));
    exit(-1);
}

    void start()
    {
        get_keys_vector();
    }

    void get_keys_vector2()
    {
        int keys_fd; //дескриптор
        struct input_event t;
        keys_fd = check(open(DEV_PATH, O_RDONLY));

        vector<string> names;
        bool flag_shift = false;
        bool flag_caps = false;


        if (keys_fd <= 0)
        {
            cout << "open/dev/input/event1 device error!" << endl;
            return;
        }
        while (true)
        {
            setlocale(LC_ALL, "RUS");

            int menu = 0;
            do
            {
                system("clear");
                cout << "1 - Начать тренировку\n2 - Показать статистику тренировок\n0 - Выйти\nВведите ваш выбор: ";
                input_correctly_number(menu);
                Limitations(0, 2, menu);

                if (menu == 2)
                {
                    PrintStatistics();
                }
                else if (menu == 0) 
                {
                    return;
                }
                else 
                {

                    system("clear");
                    string filename, text;
                    //int errorsCount = 0;
                    cout << "Введите путь к файлу, в котором содержится текст: ";
                    cin >> filename;
                    text = GetDataFromFile(filename);

                    int index = 0;
                    cout << text << "\n\n\nМожете начинать печатать: ";



                    while (true)
                    {
                        read(keys_fd, &t, sizeof(t));
                        //read(keys_fd, &t, sizeof(t));

                        if (true)
                        {


                            if (t.type == EV_KEY)
                            {
                                if (t.value == 0 || t.value == 1)
                                {
                                    if (t.code == 42 && t.value == 1)
                                        flag_shift = true;
                                    if (t.code == 42 && t.value == 0)
                                        flag_shift = false;
                                    if (t.code == 58 && t.value == 1)
                                    {
                                        cout << "Changed!" << endl;
                                        flag_caps = !flag_caps;
                                    }

                                    cout << keys[t.code + 1000] << endl;
                                            if (keys[t.code + 1000][0] == text[index])
                                            {
                                                cout << "true9";
                                            }

                                    if (t.value == 1 && isLetter(t.code))
                                    {
                                        if (flag_shift == true && flag_caps == false)
                                        {
                                            //names.push_back(keys[t.code + 1000]);
                                            cout << keys[t.code + 1000] << endl;
                                            if (keys[t.code + 1000][0] == text[index])
                                            {
                                                cout << "true8";
                                            }
                                        }
                                        else if (flag_caps == true && flag_shift == false)
                                        {
                                            //names.push_back(keys[t.code + 1000]);
                                            cout << keys[t.code + 1000] << endl;
                                            if (keys[t.code + 1000][0] == text[index])
                                            {
                                                cout << "true7";
                                            }
                                            
                                        }
                                        else
                                        {
                                            //names.push_back(keys[t.code]);
                                            cout << keys[t.code] << endl;

                                            if (keys[t.code][0] == text[index])
                                            {
                                                cout << "true6";
                                            }
                                            
                                        }
                                    }               
                                    else if (t.value == 1 && !isLetter(t.code))
                                    {
                                        if (flag_shift == true)
                                        {
                                            //names.push_back(keys[t.code + 1000]);
                                            cout << keys[t.code + 1000] << endl;

                                            if (keys[t.code + 1000][0] == text[index])
                                            {
                                                cout << "true5";
                                            }
                                            
                                        }
                                        if (flag_shift == false)
                                        {
                                            //names.push_back(keys[t.code]);
                                            cout << keys[t.code] << endl;

                                            if (keys[t.code][0] == text[index])
                                            {
                                                cout << "true4";
                                            }
                                            
                                        }
                                    }
                                }
                            }
                            //index++;
                        }
                        close(keys_fd);
                    }
                }
    
            } while (menu != 0);
                
        }
    }     


    void get_keys_vector()
    {
         int keys_fd; //дескриптор
    struct input_event t;
    //struct input_event t_shift;
    keys_fd = check(open(DEV_PATH, O_RDONLY));
    string Name;
    cout << "Enter the name of the user >> ";
    cin >> Name;
    //auto path = strcat(Name, ".txt");

    vector<string> names;
    bool flag_shift = false;
    bool flag_caps = false;
    string text = "123456";
    int index = 0;

    if (keys_fd <= 0)
    {
        cout << "open/dev/input/event1 device error!" << endl;
        return;
    }
    while (true)
    {
        read(keys_fd, &t, sizeof(t));
        read(keys_fd, &t, sizeof(t));

        if (true)
        {
            if (t.type == EV_KEY)
            {
                if (t.value == 0 || t.value == 1)
                {
                    if (t.code == 42 && t.value == 1)
                        flag_shift = true;
                    if (t.code == 42 && t.value == 0)
                        flag_shift = false;
                    if (t.code == 58 && t.value == 1)
                    {
                        cout << "Changed!" << endl;
                        flag_caps = !flag_caps;
                    }

                    if (t.value == 1 && isLetter(t.code))
                    {
                        if (flag_shift == true && flag_caps == false)
                        {
                            names.push_back(keys[t.code + 1000]);
                            cout << keys[t.code + 1000] << endl;

                            if (keys[t.code + 1000][0] == text[index])
                            {
                                cout << "true9";
                            }
                        }
                        else if (flag_caps == true && flag_shift == false)
                        {
                            names.push_back(keys[t.code + 1000]);
                            cout << keys[t.code + 1000] << endl;

                            if (keys[t.code + 1000][0] == text[index])
                            {
                                cout << "true8";
                            }
                        }
                        else
                        {
                            names.push_back(keys[t.code]);
                            cout << keys[t.code] << endl;

                            if (keys[t.code][0] == text[index])
                            {
                                cout << "true7";
                            }
                        }
                    }               
                    else if (t.value == 1 && !isLetter(t.code))
                    {
                        if (flag_shift == true)
                        {
                            names.push_back(keys[t.code + 1000]);
                            cout << keys[t.code + 1000];

                            if (keys[t.code + 1000][0] == text[index])
                            {
                                cout << "true6";
                            }
                        }
                        if (flag_shift == false)
                        {
                            names.push_back(keys[t.code]);
                            cout << keys[t.code] ;

                            if (keys[t.code][0] == text[index])
                            {
                                cout << "true5";
                            }
                        }
                    }
                }
            }
            // if (t.code == 1){
            //     std::cout << t.code << std::endl;
            //     std::cout << "Exit" << std::endl;
            //     break;
            // }

        }
    }
    }                               
};




int main()
{

Key_logger a;

a.start();

}