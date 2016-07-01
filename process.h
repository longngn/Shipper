#ifndef process_h
#define process_h

#include <vector>
#include "geometry.h"
#include "class.h"
using namespace std;

void add_edge(Tpoint a, Tpoint b);
int cal_fee(Tpoint a, Tpoint b);   // tinh phi cuoc cua don hang
double score(int u, int v);
void Astar(int s, int t);
double distant(Tpoint a, Tpoint b, vector<Tpoint>& trace);
double distant(Tpoint a, Tpoint b);
void read_map();
bool find_sp(Trequest r);   // tim xem co shipper nao den kip de nhan don hang, neu co thi lay shipper co thu nhap thap nhat
bool check(Tplace a, int time);
bool assignn(int i, Trequest r);  // giao don hang cho shipper i va tinh lai lo trinh
void read_event();
void process();

#endif
