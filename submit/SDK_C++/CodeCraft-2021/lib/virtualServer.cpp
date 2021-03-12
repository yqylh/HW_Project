#ifndef __virtualServer
#define __virtualServer

#include <string>

struct virtualServer {
    std::string name; // 虚拟机名称
    int core; // 核心数量
    int ram; // 内存大小
    int isDouble; // 是否双节点部署
    int id; // 虚拟机id
    int serverId; // 部署服务器id
    int where; // 部署位置 0->A 左 , 1->B 右
    virtualServer() {
        name.clear();
        core = 0;
        ram = 0;
        isDouble = 0;
    }
    ~virtualServer() {}
    void input(std::string name, int core, int ram, int isDouble) {
        this->name = name;
        this->core = core;
        this->ram = ram;
        this->isDouble = isDouble;
    }
};

#endif // __virtualServer