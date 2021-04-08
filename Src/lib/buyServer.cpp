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
    #ifdef EBUG
    if (ser.name == "") {
        throw;
    }
    #endif
    return ser.id;
}
void createAction(request & req , int serId, int lorR) {
    auto &ser = serverList[serId];
    virtualServer & virSerType = virtualServerType[req.name];
    if (virSerType.isDouble == 0) {
        if (lorR == 0) {
            ser.lCore -= virSerType.core;
            ser.lRam -= virSerType.ram;
        } else {
            ser.rCore -= virSerType.core;
            ser.rRam -= virSerType.ram;
        }
        virtualServerList[req.id] = virSerType;
        virtualServerList[req.id].serverId = ser.id;
        virtualServerList[req.id].where = lorR;
        virtualServerList[req.id].id = req.id;
        recordCreate(req.day, ser.id, lorR, req.order);
    } else {
        ser.lCore -= virSerType.core / 2;
        ser.rCore -= virSerType.core / 2;
        ser.lRam -= virSerType.ram / 2;
        ser.rRam -= virSerType.ram / 2;
        virtualServerList[req.id] = virSerType;
        virtualServerList[req.id].serverId = ser.id;
        virtualServerList[req.id].id = req.id;
        recordCreate(req.day , ser.id , -1, req.order);
    }
}

std::pair<int, int> merge(std::vector<request> arr) {
    std::pair<int, int> ret = {0,0};
    bool flag = 0;
    int lCore = 0, rCore = 0, lRam = 0, rRam = 0;
    bool now = 0;
    for (auto & i : arr) {
        // 第三个策略 如何使左右差值最小
        auto virSer = virtualServerType[i.name];
        if (virSer.isDouble == 1) {
            lCore += virSer.core / 2;
            lRam += virSer.ram / 2;
            rCore += virSer.core / 2;
            rRam += virSer.ram / 2;
        } else {
            if (now == 0) {
                now = 1;
                lCore += virSer.core;
                lRam += virSer.ram;
            } else {
                now = 0;
                rCore += virSer.core;
                rRam += virSer.ram;
            }
        }
    }
    // 第二个参数 玄学倍数
    ret.first = std::max(lCore, rCore);
    ret.second = std::max(lRam, rRam);
#ifdef EBUG
    // std::cout << "merge : " << ret.first << " " << ret.second << std::endl;
#endif
    return ret;
}

std::pair<std::string, int> solveMinCost(std::pair<int , int> x, int day) {
    std::pair<std::string, int> ret;
    std::string ans = "";
    int oldCost = INT_MAX / 20;
    for (auto & i : serverType) {
        if (i.second.core >= x.first * 2 && i.second.ram >= x.second * 2) {
            if (ans == "") {
                ans = i.first;
                continue;
            }
            int nowCost = i.second.cost + i.second.dayCost * (T - day);
            if (nowCost < oldCost) {
                ans = i.first;
                oldCost = nowCost;
            }
        }
    }
    ret.first = ans;
    ret.second = oldCost;
#ifdef EBUG
    // if (ans == "") {
    //     std::cout << oldCost << std::endl;
    // }
    // std::cout << "solve : " << serverType[ans].core << " " << serverType[ans].ram << std::endl;
#endif
    return ret;
}
#ifdef EBUG
template class std::unordered_map<std::string, server>;
template class std::unordered_map<int, server>;
template class std::unordered_map<std::string, virtualServer>;
template class std::unordered_map<int, virtualServer>;
#endif
// 购买一台可以放得下的服务器
void buyServer(std::vector<request> arr , int day) {
    int n = arr.size(); // request的个数
    std::vector<request > temp; // 暂存
    int ans = INT_MAX; int index = -1; // 存放最优位置
    // 第四个策略 这是一个固定序列 应该要全排列序列
    for (int i = 0; i <= (1 << (n-1))-1 ; i++) {
        temp.clear();
        int bit = i; // 二进制枚举
        int cost = 0;
        bool flag = 1;
        for (auto & req : arr) {
            temp.push_back(req);
            int ans = bit & 1;
            bit >>= 1;
            if (ans) {
                auto ret = solveMinCost(merge(temp), day);
                if (ret.first == "") {
                    flag = 0;
                }
                cost += ret.second;
                temp.clear();
            }
        }
        if (temp.size() > 0) {
            auto ret = solveMinCost(merge(temp), day);
            if (ret.first == "") {
                flag = 0;
            }
            cost += ret.second;
            temp.clear();
        }
        if (flag && cost < ans) {
            ans = cost;
            index = i;
        }
    }
    temp.clear();
    int bit = index; // 二进制枚举
    for (auto & req : arr) {
        temp.push_back(req);
        int ans = bit & 1;
        bit >>= 1;
        if (ans) {
            std::string name = solveMinCost(merge(temp), day).first;
            int serId = buyAction(day, name);
            bool lorR = 0;
            for (auto & reqs : temp) {
                createAction(reqs, serId, lorR);
                if (virtualServerType[reqs.name].isDouble == 0) lorR ^= 1;
            }
            temp.clear();
        }
    }
    if (temp.size() > 0) {
        std::string name = solveMinCost(merge(temp), day).first;
        int serId = buyAction(day, name);
        bool lorR = 0;
        for (auto & reqs : temp) {
            createAction(reqs, serId, lorR);
            if (virtualServerType[reqs.name].isDouble == 0) lorR ^= 1;
        }
        temp.clear();
    }
}