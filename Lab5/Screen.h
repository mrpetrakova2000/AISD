#pragma once

const int XMAX = 120; //Размер экрана
const int YMAX = 50;
struct point { //Точка на экране
public:
    int x, y;
    point(int a = 0, int b = 0) : x(a), y(b) { }
};
//Набор утилит для работы с экраном
void put_point(int a, int b);    //Вывод точки
void put_point(point p) { put_point(p.x, p.y); }
void put_line(int, int, int, int); //Вывод линии
void put_line(point a, point b)    // (интерфейс)
{
    put_line(a.x, a.y, b.x, b.y);
}
void screen_init();    //Инициализация буфера
void screen_destroy(); //Удаление экрана
void screen_refresh(); //Обновление картинки
void screen_clear();   //Очистка