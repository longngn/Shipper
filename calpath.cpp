#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <string>
#include <sstream>
#include <map>
#include <set>
#include <vector>
#include <cmath>
#include <utility>
#include <list>
#include <bitset>
#include <functional>
#include <cstring>
#include <ctime>
#include <fstream>

using namespace std;

#define forc(it,c) for(typeof(c.begin()) it=c.begin(); it!=c.end(); it++)
#define all(c) c.begin(),c.end()

typedef long long ll;
typedef pair<int,int> prii;

const char* fi="input.txt";
const char* fo="output.txt";

// constrain
const double eps=1e-6;
const int max_shipper=500, max_node=1e4+100, max_line=200, max_event=10000, max_try=100, max_request=5000;

// constant
const int map_size=8, block_size=2, max_wait_time=15;
const double velocity=0.5 /* km/phut */, oo=1e9,
const int initial_time=450;


struct Tline
{
    double a,b,c;
    Tline(double aa, double bb, double cc): a(aa), b(bb), c(cc) {}
    Tline() {}
    double calxfromy(double y) { return (c-b*y)/a; }
    double calyfromx(double x) { return (c-a*x)/b; }
};

struct point
{
    double x,y;
    point(double xx, double yy): x(xx), y(yy) {}
    point() {}

    bool inside()
    {
        return x>=0 && x<=map_size && y>=0 && y<=map_size;
    }

    point projection(Tline d)
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
        return point(x4,y4);
    }

    bool invalid() { return x==-1; }

    //bool operator<(point a) { return x==a.x ? y<a.y : x<a.x; }
};

bool operator<(point a, point b) { return a.x==b.x ? a.y<b.y : a.x<b.x; }

struct Tevent
{
    int time, type, id; // 1 la shipper nghi lam, 2 la shipper bat dau lam, 3 la co don hang moi
    point a,b;

    Tevent(int t, int i): time(t), type(1), id(i) {};
    Tevent(int t, double x, double y, int i): time(t), type(2), id(i) { a=point(x,y); }
    Tevent(int t, double x1, double y1, double x2, double y2, int i): time(t), type(3), id(i) { a=point(x1,y1); b=point(x2,y2); }
    Tevent() {}
};

bool operator<(Tevent a, Tevent b) { return a.time==b.time ? a.type<b.type : a.time<b.time; }

list<pair<point,int> > path[max_shipper];
// la lo trinh cua 1 shipper, duoc bieu dien duoi dang list cac nut giao thong (point), truong thu 2 (int) la tinh chat cua nut, 0 la nut binh thuong, 1 la nut nhan hang, 2 la nut giao hang

map<point,point> st[max_shipper];
// la danh sach cac nut bat buoc phai toi cua 1 shipper (nut nhan hang or giao hang)

int last_update=initial_time;
vector<point> dummy;
double income[max_shipper], expected_income[max_shipper], total_income;
point pos[max_shipper];

int n_lines, n_shippers, n_requests, n_nodes, n_events;
Tevent event[max_event];

set<int> available_shipper;

Tline line[max_line];
vector<point> node_list[max_line];
double d[max_node];
int tr[max_node];
map<point,int> id;
point node_pos[max_node];
vector<Tevent> unassigned_requests;
vector<int> stats[max_shipper];



set<pair<double,int> > heap;
vector<int> ke[max_node];








void xl()
{
    clock_t last_time=clock();
    int update_count=0;


    // doc cac event
    input>>n_requests;
    for(int i=1; i<=n_requests; i++)
    {
        int t; double x1,y1,x2,y2;
        input>>t>>x1>>y1>>x2>>y2;
        event[++n_events] = Tevent(t,x1,y1,x2,y2,i);
        request_initial_time[point(x1,y1)] = t;
    }

    input>>n_shippers;
    for(int i=1; i<=n_shippers; i++)
    {
        int t1,t2; double x,y;
        input>>t1>>t2>>x>>y;
        event[++n_events] = Tevent(t1,x,y,i);
        event[++n_events] = Tevent(t2,i);
    }

    sort(event+1, event + n_events+1);

    // duyet theo timeline
    for(int i=1; i<=n_events; i++)
    {
        if (event[i].type == 1) available_shipper.erase(event[i].id);
        else    if (event[i].type == 2)
        {
            available_shipper.insert(event[i].id);
            path[event[i].id].push_back(make_pair(event[i].a,0));
            pos[event[i].id]=event[i].a;
        } else
        {
            bool j=find_shipper(event[i]);
            if (j==false) unassigned_requests.push_back(event[i]);

            cout<<"Don hang "<<(++update_count)<<":\nThoi gian: "<<print_time(event[i].time)<<". Nhan: "<<(j?"Co":"Khong")<<". Gia tri: "<<print_currency(cal_fee(event[i].a,event[i].b))<<".\n";
            {
                int j=assigned_shipper[event[i].id];
                cout<<"Shipper "<<j<<" dang lam "<<st[j].size()<<'/'<<request_cnt[j]<<" don hang, thu nhap "<<print_currency(income[j]+expected_income[j])<<".\n";
            }
            cout<<"Tong thu: "<<print_currency(total_income)<<". Tong chi: ";
            double outcome=0; for(int i=1; i<=n_shippers; i++) outcome+=income[i]+expected_income[i];
            cout<<print_currency(outcome)<<".\nTong loi nhuan: "<<print_currency(total_income-outcome);
            cout<<". Ti le: "<<int((total_income-outcome)/total_income*100)<<"%";
            cout<<".\nThoi gian chay: "<<double(clock()-last_time)/CLOCKS_PER_SEC<<"s.\n"<<endl;
            last_time=clock();

            //cin.get();
        }
    }

    // output
    /*cout<<"Tong tien thu ve: "<<total_income<<"\n";

    { double tmp=0; for(int i=1; i<=n_shippers; i++) tmp+= income[i]+expected_income[i];
    cout<<"Tong tien phai tra: "<<tmp<<"\n";
    cout<<"Loi nhuan: "<<total_income-tmp<<"\n\n"; }

    cout<<"Thong ke moi shipper:\n";
    for(int i=1; i<=n_shippers; i++)
    {
        cout<<"Shipper thu "<<i<<":\n";
        cout<<" Nhan don hang: ";
        forc(it,stats[i]) cout<<*it<<' ';
        cout<<'\n';
        cout<<" Tien cong phai tra: "<<income[i]+expected_income[i]<<'\n';
    }
    cout<<'\n';
    cout<<"Nhung don hang bi huy: ";
    forc(it,unassigned_requests) cout<<it->id<<' ';*/
}

int main()
{
    #ifndef ONLINE_JUDGE
        //freopen(fi,"r",stdin);
        //freopen(fo,"w",stdout);
    #endif
    input.open("input.txt");
    srand(time(NULL));
    ios::sync_with_stdio(false);
    cout<<fixed;
    xl();

    input.close();
}

