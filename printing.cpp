#include <string>
#include <sstream>
using namespace std;

string print_currency(int x)
{
    stringstream ss; ss<<x;
    string s; ss>>s;
    string res; int cnt=0;
    for(int i=s.length()-1; i>=0; i--)
    {
        cnt++; if (cnt%3==1 && cnt>1) res=','+res;
        res=s[i]+res;
    }
    return res;
}

string print_time(int x)
{
    stringstream ss; string s;
    int y=x/60; ss<<y<<'h'; y=x%60; ss<<y<<"ph";
    ss>>s; return s;
}
