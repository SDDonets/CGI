#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <windows.h>
#include <string>
#include <algorithm>
#include <cmath>
#include <fstream>
using namespace std;
//Функція декодує посилання на кирилицю
string urlDecode(string& SRC) {
    string ret;
    char ch;
    int i, ii;
    for (i = 0; i < SRC.length(); i++) {
        if (SRC[i] == '%') {
            sscanf_s(SRC.substr(i + 1, 2).c_str(), "%x", &ii);
            ch = static_cast<char>(ii);
            ret += ch;
            i = i + 2;
        }
        else {
            ret += SRC[i];
        }
    }
    return (ret);
}
//функція розбиття рядка на параметри
void destribution(string s, string& model, string& power, string& weight, string& type, bool& cb_dyn, bool& cb_html, bool& cb_txt)
{
    auto it1 = s.find_first_of('=') + 1;
    auto it2 = s.find_first_of('&', it1);
    string t_model(it2 - it1, '\0');
    copy(s.begin() + it1, s.begin() + it2, t_model.begin());
    s.erase(0, it2 + 1);
    model = t_model;

    it1 = s.find_first_of('=') + 1;
    it2 = s.find_first_of('&', it1);
    string t_weight(it2 - it1, '\0');
    copy(s.begin() + it1, s.begin() + it2, t_weight.begin());
    s.erase(0, it2 + 1);
    weight = t_weight;

    it1 = s.find_first_of('=') + 1;
    it2 = s.find_first_of('&', it1);
    string t_power(it2 - it1, '\0');
    copy(s.begin() + it1, s.begin() + it2, t_power.begin());
    s.erase(0, it2 + 1);
    power = t_power;

    it1 = s.find_first_of('=') + 1;
    it2 = s.find_first_of('&', it1);
    string t_type(it2 - it1, '\0');
    copy(s.begin() + it1, s.begin() + it2, t_type.begin());
    s.erase(0, it2 + 1);
    type = t_type;

    bool found = s.find("dynamic") != string::npos;
    if (found)
        cb_dyn = true;
    found = s.find("html") != string::npos;
    if (found)
        cb_html = true;
    found = s.find("txt") != string::npos;
    if (found)
        cb_txt = true;
}

int main()
{
    SetConsoleCP(1251); //встановлення введення в кодуванні windows-1251
    SetConsoleOutputCP(1251); //встановлення виводу в кодуванні windows-1251
    string power_weight, request_uri, s_weight, s_power, s_type, model;
    bool cb_dyn = false, cb_html = false, cb_txt = false; //оголошення змінних рядків

    cout << "Content-Type: text/html\n\n";//заголовок-сутність визначення типу ресурсу для скрипта
    cout << "<html>"; //створюємо html документ
    cout << "<head>\n"; //начало хэда
    cout << "<meta charset=\"windows-1251\"> "; //встановлення кодування 1251 для правильного відображення кирилиці
    cout << "<title>CGI_Donets</title>\n"; //назва тайтла
    cout << "</head>"; //кінець хеда
    cout << "<body>\n"; //початок body
    cout << "<style type=\"text/css\">#radiolist{text-align: left; float:right} table{border-collapse: collapse}";
    cout << "#check_box{text-align: left; float:left} </style>";//оголошення та визначення каскадних стилів css

    //СТВОРЕННЯ ГОЛОВНОЇ ФОРМИ HTML ДЛЯ ЗАПОЛНЕННЯ
    cout << "<form method = \"post\" accept-charset=\"windows-1251\">";
    cout << "<table class = \"content_fields\" align=\"center\"> <tbody> <tr>";
    cout << "<td colspan=\"2\" style=\"padding: 10px; background-color: FFD700; color: #fff; border: 1px solid #00008B;\" ";
    cout << "align=\"center\">Введіть параметри автомобіля</td></tr>";
    cout << "<tr><td style=\"padding: 10px; border: 1px solid #00008B;\" align=\"right\">Модель авто:</td>";
    cout << "<td style=\"padding: 10px; text-align: justify; border: 1px solid #00008B;\"align=\"left\">";
    cout << "<input required maxlength=\"20\" name=\"name\" type=\"text\" style=\"width: 120px;\"></td></tr><tr>";
    cout << "<td style=\"padding: 10px; border: 1px solid #00008B;\" align=\"right\">Вага:</td>";
    cout << "<td style=\"padding: 10px; text-align: justify; border: 1px solid #00008B;\" align=\"left\"> ";
    cout << "<input required min = \"1\" type=\"number\" maxlength=\"5\" name=\"weight\" style=\"width: 120px;\"></td></tr> ";
    cout << "<tr style=\"background-color: #fff;\"> <td style=\"padding: 10px; border: 1px solid #00008B;\" align=\"right\">Потужність:</td> ";
    cout << "<td style=\"padding: 10px; border: 1px solid #00008B;\" align=\"left\"><div style=\"float: left; text-align: justify;\">";
    cout << "<input required min = \"20\" type=\"number\" maxlength=\"5\" name=\"power\" style=\"width: 120px;\"></div></td></tr>";
    cout << "<tr><td colspan=\"2\" style=\"padding-top: 10px; border: 1px solid #00008B;\" align=\"center\">";
    cout << "<input type=\"submit\" value=\"Розрахувати відношення ваги та потужності\" style=\"border: 3px solid #FFD700;";
    cout << "padding: 7px; margin-right: 10px; margin-top: 7px\">";
    cout << "<div id=\"radiolist\"><input type = \"radio\" name = \"ans_type\" id = \"kst\" value = \"kgks\"checked>";
    cout << "<label for = \"kgks\">кг/к.с. </label><br><input type = \"radio\" name = \"ans_type\" id = \"kst\"";
    cout << "value = \"html\"><label for = \"kst\">к.с./т </label></div>";
    cout << "<div id=\"check_box\" style = \"margin-right: 5px\">";
    cout << "<input type = \"checkbox\" id = \"dynamic\" name = \"dynamic\" value = \"dynamic\">";
    cout << "<label for = \"dynamic\">dynamic</label><br>";
    cout << "<input type = \"checkbox\" id = \"html\" name = \"html\" value = \"html\">";
    cout << "<label for = \"vehicle2\">html</label><br>";
    cout << "<input type = \"checkbox\" id = \"txt\" name = \"txt\" value = \"txt\">";
    cout << "<label for = \"vehicle3\">txt</label></div>";
    cout << "</td></tr>";
    cout << "<tr> <td colspan = \"2\" style = \"padding: 10px; background-color: #eff4ef; border: 1px solid #00008B;\"";
    cout << "align = \"center\"> <div>";

    //ВИКОНАННЯ РОЗРАХУНКІВ СКРИПТУ
    char* method = getenv("METHOD REQUEST"); //зчитуємо метод передачі параметрів
    if (method == "GET") //Якщо GET
        char* request_uri = getenv("QUERY_STRING"); //то зчитуємо рядок, що передається через getenv("QUERY_STRING")
    else //Якщо метод POST
        cin >> request_uri; //то зчитуємо переданий рядок через стандартний потік введення c++

    string s = urlDecode(request_uri);  s += "&"; //розкодуємо посилання та додамо & в кінці рядка
    destribution(s, model, s_power, s_weight, s_type, cb_dyn, cb_html, cb_txt); //розіб'ємо її на параметри
    int weight = stoi(s_weight); //витягнемо вагу в цілісний тип даних
    float power = stoi(s_power); //витягнемо зростання
    double pw = weight / power; //формула розрахунку кілограм на кіньсу силу
    double pw2 = power / weight * 1000; //формула розрахунку кіньских сил на тону

    //складемо відповідь користувачу в одному рядку


    if (s_type == "kgks") //для кілограм на кіньску силу
    {
        power_weight = power_weight + model + ", відношення масси та потужності дорівнює: " + to_string(pw).erase(to_string(pw).size() - 5);
    }
    else //для кінських сил на тону
    {
        power_weight = power_weight + model + ", відношення масси та потужності дорівнює: " + to_string(pw2).erase(to_string(pw2).size() - 5);
    }

    //Перевіримо множинний тип відповіді, яку вибрав користувач
    if (cb_dyn)
    {
        cout << power_weight; //просто виведемо відповідь у підготовлений div на формі
        cout << "<br>";
    }

    if (cb_html) //якщо html сторінка
    {
        ofstream out; //створимо об'єкт файлу
        out.open("./power_weight.html"); //створимо файл "power_weight.html"
        if (out.is_open()) //якщо файл успішно відкритий
        {
            out << "<html><head><title>IMT_RESULT</title><meta charset=\"windows-1251\">"; //створюємо простий документ
            out << "</head><body><h3>";
            out << power_weight; //друкуємо там відповідь як заголовок у тегу <h3>
            out << "</h3></body><html>";
            out.close(); //закриємо файл
            cout << "<a href=\"./power_weight.html\" target=\"_blank\">Перейти до результатів</a>";//Запропонуємо користувачеві посилання на відповідь
            cout << "<br>";
        }
    }

    if (cb_txt) //якщо текст
    {
        ofstream out; //створимо об'єкт файлу
        out.open("power_weight.txt"); //створимо текстовий документ "power_weight.txt"
        if (out.is_open()) //якщо файл успішно відкритий
        {
            out << power_weight; //надрукуємо туди відповідь
            out.close(); //закриємо файл
            cout << "<a href=\"./power_weight.txt\" download>Скачати відповідь</a>"; //запропонуємо користувачеві файл на скачування
            cout << "<br>";
        }
    }

    //завершимо документ закриваючими тегами
    cout << "</div></td></tr></tbody></table></form>";
    cout << "</body> </html>\n";
    //Кінець виконання програми
    return 0;
}
