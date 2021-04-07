#include "input.cpp"
#include "init.cpp"

int buyAction(std::vector<request>::iterator & req, std::string name){
    serverList[serverId] = serverType[name];
    auto &ser = serverList[serverId];
    ser.lCore = ser.core / 2;
    ser.rCore = ser.core / 2;
    ser.lRam = ser.ram / 2;
    ser.rRam = ser.ram / 2;
    ser.id = serverId++;
    ser.createDay = req->day;
    recordBuy(ser.createDay , ser.name);
    return ser.id;
}
void createAction(std::vector<request>::iterator & req , int serId, virtualServer & virSerType, bool isDouble) {
    auto &ser = serverList[serId];
    if (isDouble == 0) {
        // 默认就扔在左边
        ser.lCore -= virSerType.core;
        ser.lRam -= virSerType.ram;
        virtualServerList[req->id] = virtualServerType[req->name];
        virtualServerList[req->id].serverId = ser.id;
        virtualServerList[req->id].where = 0;
        virtualServerList[req->id].id = req->id;
        recordCreate(req->day , ser.id, 0, req->order);
    } else {
        ser.lCore -= virSerType.core / 2;
        ser.rCore -= virSerType.core / 2;
        ser.lRam -= virSerType.ram / 2;
        ser.rRam -= virSerType.ram / 2;
        virtualServerList[req->id] = virtualServerType[req->name];
        virtualServerList[req->id].serverId = ser.id;
        virtualServerList[req->id].id = req->id;
        recordCreate(req->day , ser.id , -1, req->order);
    }
}

// 购买一台可以放得下的服务器
void buyServer(std::vector<request>::iterator & req, virtualServer & virSerType , bool isDouble) {
    std::string ans = "";
    double cs = 2.15; // 买2倍大 
    if (virSerType.isDouble == 0) cs *= 1.45; // 1.4~1.5是最优的

    for (auto & i : serverType) {
        if (i.second.core >= virSerType.core * cs && i.second.ram >= virSerType.ram * cs) {
            if (ans == "") {
                ans = i.first;
                continue;
            }
            int nowCost = i.second.cost + i.second.dayCost * (T - req->day);
            int oldCost = serverType[ans].cost + serverType[ans].dayCost * (T - req->day);
            // 策略一 买总消耗最便宜的 消耗越小大概率越匹配 产生的碎片可能无法利用
            if (nowCost < oldCost) ans = i.first;
        }
    }
    int serId = buyAction(req, ans);
    createAction(req, serId, virSerType, isDouble);
}