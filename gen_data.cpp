#include <fstream>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include "geometry.h"
#include "info.h"
#include "class.h"
#include "gen_data.h"
using namespace std;

#define forc(i,c) for(typeof(c.begin()) i=c.begin(); i!=c.end(); i++)


Tpoint rand_point()
{
    double a= double( rand()%(map_size*1000) )/1000,
           b= double( rand()%(map_size*1000) )/1000;
    return Tpoint(a,b);
}

string s_rand_point()
{
    Tpoint a= rand_point();
    stringstream ss; ss<<a.x<<' '<<a.y;
    string s; getline(ss,s); return s;
}

int rand_time(int from, int to)
{
    return from + rand()%(to-from);
}

void gen_map()
{
    ofstream f("map.txt");
    f.precision(3); f<<fixed;

    f<<n_road<<'\n';
    for(int i=1; i<=n_road; i++)
    {
        Tpoint a=rand_point(), b=rand_point();
        Tline d=get_line(a,b);
        f<<d.a<<' '<<d.b<<' '<<d.c<<'\n';
    }
    f.close();
}

struct Tevent
{
    int time,type,type2,id;
    Tevent(int t, int tp, int i): time(t), type(tp), id(i) {}
    Tevent(int t, int tp, int tp2, int i): time(t), type(tp), type2(tp2), id(i) {}
    bool operator<(const Tevent& a) const { return time==a.time ? type>a.type : time<a.time; }
};

void gen_event()
{
    ofstream f("event.txt");
    f.precision(3); f<<fixed;

    /* 1 la request, 2 la shipper bat dau lam, 3 la shipper ket thuc lam
    request: 3 loai
    time 1 1 sx sy tx ty
    time 1 2 sx sy tx ty dtime - dtime
    time 1 3 sx sy tx ty gtime - gtime dtime - dtime
    --------
    time 2 id x y
    time 3 id */


    vector<Tevent> event;

    for(int i=1; i<=1000; i++) event.push_back(Tevent(rand_time(450, 1320), 1, 1, i));
    for(int i=1; i<=1000; i++) event.push_back(Tevent(rand_time(450, 1320), 1, 2, 1000+i));
    for(int i=1; i<=1000; i++) event.push_back(Tevent(rand_time(450, 1320), 1, 3, 2000+i));
    for(int i=1; i<=n_shipper; i++)
    {
        int a=420, b=1380;
        event.push_back(Tevent(min(a,b), 2, i));
        event.push_back(Tevent(max(a,b), 3, i));
    }

    sort(event.begin(), event.end());
    f<<event.size()<<'\n';
    forc(it,event)
    {
        f<<it->time<<' '<<it->type<<' ';
        if (it->type==1)
        {
            int t=it->type2;
            f<<t<<' '<<s_rand_point()<<' '<<s_rand_point()<<' ';
            if (t>1)
            {
                vector<int> buffer;
                for(int i=1; i<= (t==2? 2:4); i++) buffer.push_back(rand_time(it->time, 1320));
                sort(buffer.begin(),buffer.end());
                forc(it,buffer) f<<*it<<' ';
            }
            f<<'\n';
        }
        else    if (it->type==2) f<<it->id<<' '<<s_rand_point()<<'\n';
        else    f<<it->id<<'\n';
    }
    f.close();
}








