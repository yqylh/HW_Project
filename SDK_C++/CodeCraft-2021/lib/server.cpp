#ifndef __server
#define __server

#include <string>

struct server {
    std::string name; // 服务器名称
    int core;  // 核心数
    int ram;  // 内存大小
    int cost;  // 硬件成本
    int dayCost; // 每日能耗
    server() {
        name.clear();
        core = 0;
        ram = 0;
        cost = 0;
        dayCost = 0;
    }
    ~server() {}
    void input(std::string name, int core, int ram, int cost, int dayCost) {
        this->name = name;
        this->core = core;
        this->ram = ram;
        this->cost = cost;
        this->dayCost = dayCost;
    }
};

#endif // __server