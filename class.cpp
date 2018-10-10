#include "class.h"
#include "info.h"
#include "geometry.h"
#include <map>
#include <iostream>
//bla blaaaaffff
//What the helllllll ?????
using namespace std;

#define forc(i,c) for(typeof(c.begin()) i=c.begin(); i!=c.end(); i++)

Tplace::Tplace(Tpoint p): pos(p), type(0) {}
Tplace::Tplace(Tpoint p, int t, int iid): pos(p), type(t), id(iid) {}

time_range::time_range(int x, int y): from(x), to(y) {}
time_range::time_range() {}

Trequest::Trequest() { wait_gtime=wait_dtime=oo; gtime=dtime=time_range(0, day_time); }

bool operator<(Tplace a, Tplace b)
{
    return a.pos==b.pos ? (a.id==b.id ? a.type<b.type : a.id<b.id) : a.pos<b.pos;
}

void Tshipper::update_pos(int time)    // cap nhap vi tri cua shipper khi sau thoi gian time
{
    double s= velocity*time, old_s=s;

    while (path.size() > 1)
    {
        Tpoint a=path.begin()->pos, b=(++path.begin())->pos;
        int t=path.begin()->type;
        if (t==1)
        {
            map<Tplace,Tplace>::iterator it = required_plc.find(path.front());
            if (it!=required_plc.end())
            {
                pair<Tplace,Tplace> tmp = *it;
                required_plc.erase(*path.begin());
                tmp.first=tmp.second, tmp.second.pos.x=-1;
                required_plc.insert(tmp);
            }
        } else
            if (t==2) required_plc.erase(*path.begin());

        if (kc(a,b)<=s)
        {
            s-=kc(a,b);
            path.erase(path.begin());
        } else
        {
            path.front()=Tplace(find_C(a,b,s));
            break;
        }
    }
    if (path.size()==1 && path.front().type==2) required_plc.erase(path.front());

    income += salary * (old_s-s);
    expected_income -= salary * (old_s-s);
    pos=path.front().pos;
}

double Tshipper::total_in() { return income+expected_income; }
