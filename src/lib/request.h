#ifndef __request__
#define __request__

#include <string>

struct request
{
    bool type; // 0-> add || 1-> delete
    int id; //虚拟机id
    std::string name; // 虚拟机型号
    int day; // 第几天的请求
    int order;
    int rate;

    request()
    {
        type = 0;
        id = 0;
        name.clear();
        day = 0;
        order = 0;
        rate = 0;
    }
};

#endif