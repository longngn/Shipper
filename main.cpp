#include <iostream>
#include <ctime>
#include <cstdlib>
#include "gen_data.h"
#include "process.h"
#include "info.h"

using namespace std;

int main()
{
    ios::sync_with_stdio(false);
    srand(time(0));

    //gen_map();
    //gen_event();
    //
    process();
}

