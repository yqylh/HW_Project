#ifndef __delete__
#define __delete__
#include "input.cpp"

// 删除一台虚拟机
void deleteVitrualServer(int _id) {
    // 找到这个id对应的实例
    auto &x = virtualServerList[_id];
    // 双节点
    if (x.isDouble == 1) {
        serverList[x.serverId].lCore += x.core / 2;
        serverList[x.serverId].rCore += x.core / 2;
        serverList[x.serverId].lRam += x.ram / 2;
        serverList[x.serverId].rRam += x.ram / 2;
    } else { // 单节点
        if (x.where == 0) {
            serverList[x.serverId].lCore += x.core;
            serverList[x.serverId].lRam += x.ram;
        } else {
            serverList[x.serverId].rCore += x.core;
            serverList[x.serverId].rRam += x.ram;
        }
    }
    virtualServerList.erase(_id);
}
#endif