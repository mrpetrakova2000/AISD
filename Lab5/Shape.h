#pragma once
#include "Screen.h"
#include <list>
#include <iostream>

char screen[YMAX][XMAX];   //Буфер для изображения
enum color { black = '*', white = '.' };
void screen_init()        //Очистка буфера (инициализация)
{
    for (int y = 0; y < YMAX; y++)
        for (auto& x : screen[y])  x = white;
}
void screen_destroy()
{
    for (auto y = 0; y < YMAX; ++y)
        for (auto& x : screen[y])  x = black;
}
void screen_clear() { screen_init(); } //Очистка экрана
void screen_refresh() // Обновление экрана
{
    for (int y = YMAX - 1; 0 <= y; --y) { // с верхней строки до нижней
        for (auto x : screen[y])    // от левого столбца до правого
            std::cout << x;
        std::cout << '\n';
    }
}


bool on_screen(int a, int b)     // Проверка попадания
{
    return 0 <= a && a < XMAX && 0 <= b && b < YMAX;
}


void put_point(int a, int b)               // Рисование точки
{
    if (on_screen(a, b)) screen[b][a] = black;
}


void put_line(int x0, int y0, int x1, int y1) // Рисование линии
{
    int dx = 1, a = x1 - x0;
    if (a < 0) { dx = -1;  a = -a; }
    int dy = 1, b = y1 - y0;
    if (b < 0) { dy = -1; b = -b; }
    int twoA{ a * 2 }, twoB{ b * 2 }, xCrit{ -b + twoA }, eps{ 0 };
    for (;;)
    {
        put_point(x0, y0);
        if (x0 == x1 && y0 == y1)  break;
        if (eps <= xCrit) { x0 += dx; eps += twoB; }
        if (eps >= a || a < b) { y0 += dy; eps -= twoA; }
    }
}

struct shape { // Виртуальный базовый класс «фигура»
    static std::list<shape*> shapes; // Список фигур (один на все фигуры!)
    shape() { shapes.push_back(this); }
    //Конструктор присоединяет фигуру к списку для рисования
    virtual point north() const = 0;      //Точки для привязки
    virtual point south() const = 0;
    virtual point east() const = 0;
    virtual point west() const = 0;
    virtual point neast() const = 0;
    virtual point seast() const = 0;
    virtual point nwest() const = 0;
    virtual point swest() const = 0;
    virtual void draw() = 0;              //Рисование
    virtual void move(int, int) = 0;          //Перемещение
    virtual void resize(double) = 0;          //Изменение размера
    virtual ~shape() { shapes.remove(this); }
};

std::list<shape*> shape::shapes; //Размещение списка фигур

void shape_refresh() //Перерисовка всех фигур на экране
{
    screen_clear();
    for (auto p : shape::shapes) p->draw(); //Динамическое связывание!!!
    screen_refresh();
}
//Промежуточные классы специального назначения

class rotatable : virtual public shape { //Фигуры, пригодные к повороту
public:
    virtual void rotate_left() = 0; //Повернуть влево
    virtual void rotate_right() = 0; //Повернуть вправо
};

class reflectable : virtual public shape {
public:
    virtual void flip_horisontally() = 0;
    virtual void flip_vertically() = 0;
};


void up(shape& p, const shape& q) // Поместить p над q
{     // Это ОБЫЧНАЯ функция, не член класса!
  // Динамическое связывание!!
    point n = q.north();
    point s = p.south();
    p.move(n.x - s.x, n.y - s.y + 1);
}

// ПРИМЕР ДОБАВКИ : дополнительная функция присоединения…

void down(shape& p, const shape& q) //Поместить p под q
{
    point n = q.south();
    point s = p.north();
    p.move(n.x - s.x, n.y - s.y - 1);
}