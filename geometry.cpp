#include <cmath>
#include "geometry.h"
#include "info.h"

Tline::Tline(double aa, double bb, double cc): a(aa), b(bb), c(cc) {}
Tline::Tline() {}

double Tline::calxfromy(double y) { return (c-b*y)/a; }
double Tline::calyfromx(double x) { return (c-a*x)/b; }

Tpoint::Tpoint(double xx, double yy): x(xx), y(yy) {}
Tpoint::Tpoint() {}

Tpoint Tpoint::projection(Tline d)
{
    double x1,y1,x2,y2, x3=x, y3=y, x4,y4;

    if (d.b!=0)
    {
        x1=1, y1=d.calyfromx(x1);
        x2=2, y2=d.calyfromx(x2);
    } else
    {
        y1=1, x1=d.calxfromy(y1);
        y2=2, x2=d.calxfromy(y2);
    }

    double k = ((y2-y1) * (x3-x1) - (x2-x1) * (y3-y1)) / (pow(y2-y1,2) + pow(x2-x1,2));
    x4 = x3 - k * (y2-y1);
    y4 = y3 + k * (x2-x1);
    return Tpoint(x4,y4);
}

bool Tpoint::invalid() { return x==-1; }
bool Tpoint::inside() { return x>=0 && x<=map_size && y>=0 && y<=map_size; }

Tline get_line(Tpoint p1, Tpoint p2)
{
    double a=p2.y-p1.y, b=p1.x-p2.x, c=a*p1.x+b*p1.y;
    return Tline(a,b,c);
}

bool operator<(Tpoint a, Tpoint b) { return a.x==b.x ? a.y<b.y : a.x<b.x; }
bool operator==(Tpoint a, Tpoint b) { return a.x==b.x && a.y==b.y; }

Tpoint intersection(Tline d1, Tline d2)
{
    Tpoint res;
    double det = d1.a*d2.b - d2.a*d1.b;
    if (det == 0) res.x=-1;
    else
    {
        res.x = (d2.b*d1.c - d1.b*d2.c)/det;
        res.y = (d1.a*d2.c - d2.a*d1.c)/det;
    }
    return res;
}

double kc(Tpoint a, Tpoint b)
{
    return sqrt(pow(a.x-b.x,2) + pow(a.y-b.y,2));
}

Tpoint find_C(Tpoint a, Tpoint b, double d)
{
    Tpoint c;
    double l=kc(a,b);
    c.x = a.x + d/l * (b.x-a.x);
    c.y = a.y + d/l * (b.y-a.y);
    return c;
}
