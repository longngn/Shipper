#ifndef geometry_h
#define geometry_h

struct Tline
{
    double a,b,c;
    Tline(double, double, double);
    Tline();

    double calxfromy(double);
    double calyfromx(double);
};

struct Tpoint
{
    double x,y;
    Tpoint(double, double);
    Tpoint();

    Tpoint projection(Tline);
    bool invalid();
    bool inside();
};

Tline get_line(Tpoint, Tpoint);
double kc(Tpoint, Tpoint);
Tpoint intersection(Tline, Tline);
Tpoint find_C(Tpoint, Tpoint, double);

bool operator<(Tpoint, Tpoint);
bool operator==(Tpoint a, Tpoint b);




#endif
