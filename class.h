#ifndef class_h
#define class_h

#include <vector>
#include <list>
#include <map>
#include "geometry.h"
using namespace std;

struct Tshipper;
struct time_range;
struct Trequest;
struct Tplace;

struct Tshipper
{
    Tpoint pos;
    int rq_cnt; vector<int> assigned_rq;
    double income, expected_income;
    list<Tplace> path;
    map<Tplace,Tplace> required_plc;

    double total_in();
    void update_pos(int time);
};

struct time_range
{
    int from,to;
    time_range(int, int);
    time_range();
};

struct Trequest
{
    int type, time, id;
    Tpoint from,to;
    int wait_gtime, wait_dtime;
    time_range gtime, dtime;

    Trequest();
};

struct Tplace
{
    Tpoint pos;
    int type,id; // id=0 la node thuong, type 1 la diem nhan hang, 2 la giao hang
    Tplace(Tpoint);
    Tplace(Tpoint, int, int);
};

bool operator<(Tplace a, Tplace b);


#endif
