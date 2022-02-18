#pragma once
#include "Shape.h"
#include "Screen.h"

//Класс "прямоугольник"

class rectangle : public rotatable, public reflectable {
protected:
    point sw, ne;
public:
    rectangle(point a, point b) : sw(a), ne(b) {}; //Конструктор по точкам
    point north() const { return point((sw.x + ne.x) / 2, ne.y); }
    point south() const { return point((sw.x + ne.x) / 2, sw.y); }
    point east() const { return point(ne.x, (sw.y + ne.y) / 2); }
    point west() const { return point(sw.x, (sw.y + ne.y) / 2); }
    point neast() const { return ne; }
    point seast() const { return point(ne.x, sw.y); }
    point nwest() const { return point(sw.x, ne.y); }
    point swest() const { return sw; }

    void rotate_right() override //Поворот вправо относительно se
    {
        int w = ne.x - sw.x, h = ne.y - sw.y; //(учитывается масштаб по осям)
        sw.x = ne.x - h * 2; ne.y = sw.y + w / 2;
    }
    void rotate_left() override // Поворот влево относительно sw
    {
        int w = ne.x - sw.x, h = ne.y - sw.y; //(учитывается масштаб по осям)
        ne.x = sw.x + h * 2; ne.y = sw.y + w / 2;
    }

    void move(int a, int b)
    {
        sw.x += a; sw.y += b; ne.x += a; ne.y += b;
    }
    void resize(double d)
    {
        ne.x = sw.x + (ne.x - sw.x) * d; ne.y = sw.y + (ne.y - sw.y) * d;
    }
    void draw()
    {
        put_line(nwest(), ne);   put_line(ne, seast());
        put_line(seast(), sw);   put_line(sw, nwest());
    }
    void flip_horisontally() override
    {
        rotate_left();
        rotate_left();
    }

    void flip_vertically() override
    {
        rotate_left();
        rotate_left();
    }
};

