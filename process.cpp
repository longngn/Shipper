#include <fstream>
#include <vector>
#include <map>
#include <set>
#include <list>
#include <algorithm>
#include <cmath>
#include <ctime>
#include <iostream>
#include "geometry.h"
#include "info.h"
#include "class.h"
#include "process.h"
#include "printing.h"
using namespace std;

#define forc(i,c) for(typeof(c.begin()) i=c.begin(); i!=c.end(); i++)
#define all(c) c.begin(),c.end()

int last_update=1;

vector<int> ke[max_node];
map<Tpoint,int> id;
double d[max_node];
Tpoint node_pos[max_node];
int n_node, tr[max_node];
vector<Tpoint> node_list[max_line];
int n_line; Tline line[max_line];

set<int> available_sp;
Tshipper sp[max_sp];

Trequest rq[max_rq]; int n_rq;
int assigned_sp[max_rq];
int get_time[max_rq];

int total_income;

clock_t last_time=clock();


void add_edge(Tpoint a, Tpoint b)
{
    int u=id[a], v=id[b];
    ke[u].push_back(v);
    ke[v].push_back(u);
}

int cal_fee(Tpoint a, Tpoint b)   // tinh phi cuoc cua don hang
{
    int x1=a.x/block_size + eps, y1=a.y/block_size + eps, x2=b.x/block_size + eps, y2=b.y/block_size + eps;
    int d=abs(x1-x2)+abs(y1-y2);
    if (d==0) return c0;
    else    if (d==1) return c1;
    else    if (d==2) return c2;
    else    return c2 + c3*(d-2);
}

double score(int u, int v)
{
    return d[u] + kc(node_pos[u],node_pos[v]);
}
void Astar(int s, int t)
{
    for(int i=1; i<=n_node; i++) tr[i]=0;
    tr[s]=-1, d[s]=0;
    set<pair<double,int> > heap; heap.insert(make_pair(score(s,t),s));
    while (!heap.empty())
    {
        int u=heap.begin()->second; heap.erase(heap.begin());
        forc(it,ke[u])
        {
            int v=*it; double ts=kc(node_pos[u],node_pos[v]);
            if (tr[v]==0)
            {
                tr[v]=u; d[v]=d[u] + ts;
                if (v==t) return;
                heap.insert(make_pair(score(v,t),v));
            } else
                if (d[u]+ts<d[v])
                {
                    heap.erase(make_pair(score(v,t),v));
                    d[v]=d[u]+ts;
                    heap.insert(make_pair(score(v,t),v));
                }
        }
    }
}

double distant(Tpoint a, Tpoint b, vector<Tpoint>& trace)
{
    int d1,d2; double mi;
    mi=oo;
    for(int i=1; i<=n_line; i++)
    {
        Tpoint tmp=a.projection(line[i]);
        if (tmp.inside() && kc(a,tmp)<mi) mi=kc(a,tmp), d1=i;
    }
    mi=oo;
    for(int i=1; i<=n_line; i++)
    {
        Tpoint tmp=b.projection(line[i]);
        if (tmp.inside() && kc(b,tmp)<mi) mi=kc(b,tmp), d2=i;
    }

    Tpoint pj1=a.projection(line[d1]),
           pj2=b.projection(line[d2]);
    Tpoint p1=*lower_bound(all(node_list[d1]), pj1),
           p2=*lower_bound(all(node_list[d2]), pj2);

    int u=id[p1], v=id[p2];

    // dung A* de tim duong di ngan nhat tu u den v
    Astar(v,u);

    trace.clear();
    trace.push_back(a); trace.push_back(pj1); int old_u=u;
    while (u!=v)
    {
        trace.push_back(node_pos[u]);
        u=tr[u];
    }
    trace.push_back(node_pos[v]);
    trace.push_back(pj2); trace.push_back(b);

    return kc(a,pj1)+kc(pj1,p1)+d[old_u]+kc(p2,pj2)+kc(pj2,b);
}

double distant(Tpoint a, Tpoint b)
{
    int d1,d2; double mi;
    mi=oo;
    for(int i=1; i<=n_line; i++)
    {
        Tpoint tmp=a.projection(line[i]);
        if (tmp.inside() && kc(a,tmp)<mi) mi=kc(a,tmp), d1=i;
    }
    mi=oo;
    for(int i=1; i<=n_line; i++)
    {
        Tpoint tmp=b.projection(line[i]);
        if (tmp.inside() && kc(b,tmp)<mi) mi=kc(b,tmp), d2=i;
    }

    Tpoint pj1=a.projection(line[d1]),
           pj2=b.projection(line[d2]);
    Tpoint p1=*lower_bound(all(node_list[d1]), pj1),
           p2=*lower_bound(all(node_list[d2]), pj2);

    int u=id[p1], v=id[p2];
    // dung A* de tim duong di ngan nhat tu u den v
    Astar(v,u);

    return kc(a,pj1)+kc(pj1,p1)+d[u]+kc(p2,pj2)+kc(pj2,b);
}

void read_map()
{
    ifstream f("map.txt");
    f>>n_line;
    for(int i=1; i<=n_line; i++)
    {
        double a,b,c;
        f>>a>>b>>c;
        line[i]=Tline(a,b,c);
    }
    line[++n_line]=Tline(1,0,0);
    line[++n_line]=Tline(1,0,map_size);
    line[++n_line]=Tline(0,1,0);
    line[++n_line]=Tline(0,1,map_size);

    for(int i=1; i<=n_line-1; i++)
        for(int j=i+1; j<=n_line; j++)
        {
            Tpoint tmp=intersection(line[i],line[j]);
            if (tmp.invalid() || !tmp.inside()) continue;
            node_list[i].push_back(tmp);
            node_list[j].push_back(tmp);
            id[tmp]=++n_node;
            node_pos[n_node]=tmp;
        }

    for(int i=1; i<=n_line; i++)
    {
        sort(node_list[i].begin(),node_list[i].end());
        for(int j=0; j<=int(node_list[i].size())-2; j++) add_edge(node_list[i][j], node_list[i][j+1]);
    }
    f.close();
}

bool find_sp(Trequest r)   // tim xem co shipper nao den kip de nhan don hang, neu co thi lay shipper co thu nhap thap nhat
{
    vector<pair<double,int> > potential_shipper;

    if (r.time>last_update)
    {
        forc(it,available_sp) sp[*it].update_pos(r.time-last_update);
        last_update=r.time;
    }
    forc(it,available_sp)
    {
        double d=distant(sp[*it].pos, r.from);
        if ( d/velocity <= r.wait_gtime)
            {
                if (sp[*it].assigned_rq.empty() && d > first_shipment_max_dist) continue;
                potential_shipper.push_back(make_pair( sp[*it].total_in() , *it) );
            }
    }

    sort(all(potential_shipper));
    forc(it,potential_shipper)
    {
        if (assignn(it->second, r)) return true;
        if (double(clock()-last_time)/CLOCKS_PER_SEC > 5) return false;
    }
    return false;
}

bool check(Tplace a, int time)
{
    int i=a.id;
    if (rq[i].type==1)
    {
        if (a.type==1 && time > rq[i].time + rq[i].wait_gtime ) return false;
        if (a.type==2 && time > get_time[i] + distant(rq[i].from,rq[i].to)/velocity + rq[i].wait_dtime ) return false;
    } else
        if (rq[i].type==2)
        {
            if (a.type==2 && time > rq[i].dtime.to ) return false;
        } else
        {
            if (a.type==1 && time > rq[i].gtime.to ) return false;
            if (a.type==2 && time > rq[i].dtime.to ) return false;
        }
    return true;
}

bool assignn(int i, Trequest r)  // giao don hang cho shipper i va tinh lai lo trinh
{
    Tplace start=Tplace(r.from, 1, r.id),
           finish=Tplace(r.to, 2, r.id);
    sp[i].required_plc.insert(make_pair(start,finish));
    double min_path=oo; Tplace dm(Tpoint(0,0));

    vector<Tplace> p,tr; p.push_back(Tplace(sp[i].pos));
    forc(it,sp[i].required_plc) if (it->second.pos.invalid()) p.push_back(dm);
                   else p.push_back(dm), p.push_back(dm);

    vector<Tplace> buffer;
    for(int t=1; t <= max_try; t++)
    {
        buffer.clear();
        forc(it,sp[i].required_plc) buffer.push_back(it->first);
        double s=0; bool xd=true; int time=r.time;

        for(int j=1; j<=int(p.size())-1; j++)
        {
            int tmp=rand() % buffer.size();
            p[j]=buffer[tmp];
            double d=distant(p[j-1].pos, p[j].pos);
            s+=d; time+= d/velocity;
            if (rq[p[j].id].type>1 && p[j].type==2) time=max(time  , rq[p[j].id].dtime.from);
            if (rq[p[j].id].type==3 && p[j].type==1) time=max(time, rq[p[j].id].gtime.from);
            if (p[j].type==1) get_time[p[j].id]=time;

            if (!check( p[j], time )) { xd=false; break; }

            map<Tplace,Tplace>::iterator it = sp[i].required_plc.find(buffer[tmp]);
            if (it==sp[i].required_plc.end() || it->second.pos.invalid()) buffer.erase(buffer.begin()+tmp);
            else    buffer[tmp]=it->second;
        }

        /*double s=0; bool xd=true;
        for(int j=1; j<=int(p.size())-1;  j++)
        {
            s+= distant(p[j-1].first , p[j].first);
            if (p[j].second==1 && time + s/velocity > double(request_initial_time[p[j].first]) + max_wait_time )
            {
                xd=false; break;
            }
        }*/

        if (!xd) continue;
        if (s<min_path)
        {
            min_path=s;
            tr=p;
        }
    }

    if (min_path==oo) return false;

    sp[i].path.clear();
    for(int j=0; j<=int(tr.size())-2; j++)
    {
        vector<Tpoint> trace;
        distant(tr[j].pos, tr[j+1].pos, trace);
        sp[i].path.push_back(tr[j]);
        for(int k=1; k<=int(trace.size())-2; k++) sp[i].path.push_back(Tplace(trace[k]));
        sp[i].path.push_back(tr[j+1]);
    }

    sp[i].expected_income = min_path*salary;
    if (sp[i].assigned_rq.empty()) sp[i].expected_income -= distant(sp[i].pos, r.from)*salary;
    total_income += cal_fee(r.from , r.to);
    sp[i].assigned_rq.push_back(r.id);
    assigned_sp[r.id]=i;
    sp[i].rq_cnt++;
    return true;
}

void read_event()
{
    ifstream f("event.txt");
    int n_event; f>>n_event;
    for(int ie=1; ie<=n_event; ie++)
    {
        int time, e_type;
        f>>time>>e_type;
        if (e_type==1)
        {
            ++n_rq;
            rq[n_rq].id=n_rq; rq[n_rq].time=time;
            int rq_type; f>>rq_type; rq[n_rq].type=rq_type;

            double x,y;
            f>>x>>y; rq[n_rq].from=Tpoint(x,y);
            f>>x>>y; rq[n_rq].to=Tpoint(x,y);

            if (rq_type==1) rq[n_rq].wait_gtime=15, rq[n_rq].wait_dtime=10;
            int tf,tt;
            if (rq_type==3) { f>>tf>>tt; rq[n_rq].gtime=time_range(tf,tt); }
            if (rq_type==2 || rq_type==3) { f>>tf>>tt; rq[n_rq].dtime=time_range(tf,tt); }

            bool tmp=find_sp(rq[n_rq]);

            cout<<"Don hang "<<n_rq<<":\nThoi gian: "<<print_time(time)<<". Nhan: "<<(tmp?"Co":"Khong")<<". Gia tri: "<<print_currency(cal_fee(rq[n_rq].from,rq[n_rq].to))<<".\n";
            {
                int j=assigned_sp[n_rq];
                cout<<"Shipper "<<j<<" dang lam "<<sp[j].required_plc.size()<<'/'<<sp[j].rq_cnt<<" don hang, thu nhap "<<print_currency(sp[j].total_in())<<".\n";
            }
            cout<<"Tong thu: "<<print_currency(total_income)<<". Tong chi: ";
            double outcome=0; for(int i=1; i<=n_shipper; i++) outcome+=sp[i].total_in();
            cout<<print_currency(outcome)<<".\nTong loi nhuan: "<<print_currency(total_income-outcome);
            cout<<". Ti le: "<<int((total_income-outcome)/total_income*100)<<"%";
            cout<<".\nThoi gian chay: "<<double(clock()-last_time)/CLOCKS_PER_SEC<<"s.\n"<<endl;
            last_time=clock();

            //cin.get();
        } else
        {
            int id; f>>id;
            if (e_type==2)
            {
                double x,y;
                f>>x>>y;
                available_sp.insert(id);
                sp[id].pos=Tpoint(x,y);
                sp[id].path.push_back(Tplace(Tpoint(x,y)));
            } else  available_sp.erase(id);
        }
    }
    f.close();
}

void process()
{
    read_map();
    read_event();
}




