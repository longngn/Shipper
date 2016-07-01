#ifndef gen_data_h
#define gen_data_h

#include <string>
#include "geometry.h"
using namespace std;

Tpoint rand_point();
string s_rand_point();
int rand_time(int from, int to);
void gen_map();
void gen_event();

#endif
