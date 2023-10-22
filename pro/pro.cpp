#include "BMP.h"
using namespace std;
//В задании написано в ответ прикрепить исходный код с комментариями, надеюсь я правильно понял назначение комментариев.
//вариант будет представлен ниже или же если написать info при запуске программы
int main()
{
    //Тут на самом деле все понятно, всё волшебство просиходит в классе BMP
    setlocale(LC_ALL, "rus");
    string input;
    BMP img;
    cout << "Вы открыли мини проект по ПВС! Чтобы вывести список всех команд введите help\n";
    do {
        cout << endl;
        getline(cin, input);
        if (input == "help") {
            cout << "Доступные команды:\nload [NAME].bmp [-g]\tЗагрузка изображения для обработки\n-g\tПеревод RGB в градации серого.\n\n";
            cout << "matrix [-s[ NAME]] \tВывод матрицы, соответствующей плотности серого в каждой точке загруженного изображения\n-s\tСохранить в текстовом документе\n\n";
            cout << "task [-p | -l]\tПрименение сверточного фильтра\n-p\tПараллельное выполнение задания\n-l\tПоследовательное выполнение задания\n\n";
            cout << "save [NAME].bmp\t Сохранение полученного изображения\n\n";
            cout << "info\t вывод информации об авторе, задании и инструкциях\n\n";
            cout << "exit\tВыход\n\n";
        }
        else if (input == "info") {
            cout << "Выполнил студент группы КИ21-07б Петров Илья Вячеславович\nВариант - 27\n";
            cout << "Свертка со следующими параметрами:\n";
            cout << "1. Количество каналов исходного изображения: 1\n";
            cout << "2. Количество ядер свертки: 3\n";
            cout << "3. Размер ядер: 5\n";
            cout << "4. Паддинг: valid\n";
            cout << "5. Итоговый результат: сумма сверток по каналам.\n\n";
            cout << "Перед началом работы необходимо загрузить изображение, ВАЖНО указать колличество каналов изображения, так как дальнейшая обработка будет использовать информацию об одном из каналов, исключая проверку на их равенство друг с другом в случае если изображение не чб (потому что я так хочу).\n";
            cout << "После загрузки можно выполнить следующие команды: matrix, save, task.\ В принципе при вводе task вам будут выданы все дальнейшие инструкции.";
        }
        else if (input == "matrix") {
            img.matrixOut();
        }
        else if (input.find("matrix -s") != string::npos) {
            if (input.size() <= 10) {
                cout << "Неверный синтаксис команды!\n";
                continue;
            }
            string name = input.substr(10);
            img.matrixOut(name);
        }
        else if ((input.substr(0, 5) == "save ") and (input.find(".bmp") != string::npos)) {
            if (input.size() <= 9) {
                cout << "Неверный синтаксис команды!\n";
                continue;
            }
            string name = input.substr(5);
            img.saveImg(name);
        }
        else if (input.find("task -p") != string::npos) {
            if (input.find("-t") != string::npos) {
                cout << "task -p -t\n";
            }
            else {
                img.doTaskLine(true);
            }
        }
        else if (input.find("task -l") != string::npos) {
            if (input.find("-t") != string::npos) {
                cout << "task -l -t\n";
            }
            else {
                img.doTaskLine(false);
            }
        }
        else if ((input.substr(0, 4) == "load") and (input.find(".bmp") != string::npos)) {
            if (input.size() <= 5) {
                cout << "Неверный синтаксис команды!\n";
                continue;
            }
            input = input.substr(5);
            if (input.find("-g") != string::npos) {
                string name = input.erase(input.find(" -g"), 3);
                img.loadImg(name, true);
            }
            else {
                string name = input;
                img.loadImg(name, false);
            }
        }
        else if (input == "exit") {
            cout << "Программа завершила свою работу";
        }
        else {
            cout << "Неизвестная команда \"" << input << "\"\n";
        }
    } while (input != "exit");
    return 0;
}