#include "input.cpp"
#include "init.cpp"

int buyAction(int day, std::string name){
    serverList[serverId] = serverType[name];
    auto &ser = serverList[serverId];
    ser.lCore = ser.core / 2;
    ser.rCore = ser.core / 2;
    ser.lRam = ser.ram / 2;
    ser.rRam = ser.ram / 2;
    ser.id = serverId++;
    ser.createDay = day;
    recordBuy(ser.createDay , ser.name);
    return ser.id;
}
void createAction(request & req , int serId, virtualServer & virSerType) {
    auto &ser = serverList[serId];
    if (virtualServerType[req.name].isDouble == 0) {
        // 默认就扔在左边
        ser.lCore -= virSerType.core;
        ser.lRam -= virSerType.ram;
        virtualServerList[req.id] = virtualServerType[req.name];
        virtualServerList[req.id].serverId = ser.id;
        virtualServerList[req.id].where = 0;
        virtualServerList[req.id].id = req.id;
        recordCreate(req.day , ser.id, 0, req.order);
    } else {
        ser.lCore -= virSerType.core / 2;
        ser.rCore -= virSerType.core / 2;
        ser.lRam -= virSerType.ram / 2;
        ser.rRam -= virSerType.ram / 2;
        virtualServerList[req.id] = virtualServerType[req.name];
        virtualServerList[req.id].serverId = ser.id;
        virtualServerList[req.id].id = req.id;
        recordCreate(req.day , ser.id , -1, req.order);
    }
}

std::pair<int, int> merge(std::vector<request> arr) {
    std::pair<int, int> ret = {0,0};
    bool flag = 0;
    int lastCore = 0, lastRam = 0;
    for (auto & i : arr) {
        auto virSer = virtualServerType[i.name];
        if (virSer.isDouble == 1) {
            ret.first += virSer.core / 2;
            ret.second += virSer.ram / 2;
        } else {
            if (lastCore == 0 && lastRam == 0) {
                lastCore += virSer.core;
                lastRam += virSer.ram;
            } else {
                
            }
        }
    }
}

// 购买一台可以放得下的服务器
void buyServer(std::vector<request>::iterator & req, virtualServer & virSerType , bool isDouble) {
    std::vector<request> arr;
    int limit = 13;
    while (limit--) {
        if (req == requestList[req->day].end()) break;
        if (req->type == 1) break;
        arr.push_back(*req);
    }
    req--;
    int n = arr.size(); // request的个数
    std::vector<request > temp; // 暂存
    int ans = INT_MAX; int index = -1; // 存放最优位置
    for (int i = 0; i <= (1 << (n-1))-1 ; i++) {
        temp.clear();
        int bit = i; // 二进制枚举
        int cost = 0;
        for (auto & req : arr) {
            temp.push_back(req);
            int ans = bit & 1;
            bit >> 1;
            if (ans) {
                cost += solveMinCost(merge(temp));
                temp.clear();
            }
        }
        if (temp.size() > 0) {
            cost += solveMinCost(merge(temp));
            temp.clear();
        }
        if (cost < ans) {
            ans = cost;
            index = i;
        }
    }
    temp.clear();
    int bit = index; // 二进制枚举
    int cost = 0;
    for (auto & req : arr) {
        temp.push_back(req);
        int ans = bit & 1;
        bit >> 1;
        if (ans) {
            std::string name = findMinCost(merge(temp));
            int serId = buyAction(req.day, name);
            for (auto & req : temp) {
                createAction(req, serId, virSerType);
            }
            temp.clear();
        }
    }
    if (temp.size() > 0) {
        std::string name = findMinCost(merge(temp));
        int serId = buyAction(req.day, name);
        for (auto & req : temp) {
            createAction(req, serId, virSerType);
        }
        temp.clear();
    }
    // std::string ans = "";
    // double cs = 2.15; // 买2倍大
    // if (virSerType.isDouble == 0) cs *= 1.45; // 1.4~1.5是最优的

    // for (auto & i : serverType) {
    //     if (i.second.core >= virSerType.core * cs && i.second.ram >= virSerType.ram * cs) {
    //         if (ans == "") {
    //             ans = i.first;
    //             continue;
    //         }
    //         int nowCost = i.second.cost + i.second.dayCost * (T - req->day);
    //         int oldCost = serverType[ans].cost + serverType[ans].dayCost * (T - req->day);
    //         // 策略一 买总消耗最便宜的 消耗越小大概率越匹配 产生的碎片可能无法利用
    //         if (nowCost < oldCost) ans = i.first;
    //     }
    // }
    // int serId = buyAction(req, ans);
    // createAction(req, serId, virSerType, isDouble);
}