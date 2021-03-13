#include "input.cpp"
#include "init.cpp"

// 购买一台可以放得下的服务器
void buyServer(request & req, virtualServer & virSerType , bool isDouble) {
    // 策略三 预处理一天的量进行考虑
    // static bool initOnce = false;
    // if (initOnce == false ) {
    //     initOnce = true;
    //     for (auto & i : requestList) {
    //         if (i.type == 0) {
    //             serverRequest[i.day].first += virtualServerType[i.name].core;
    //             serverRequest[i.day].second += virtualServerType[i.name].ram;
    //         } else {
    //             // 对于删除 有可能会有今天创建今天删除的
    //             // 记录峰值是不是更好 ? 实际加了删除一点都没变
    //             if (virtualServerList.count(i.id)) {
    //                 serverRequest[i.day].first -= virtualServerList[i.id].core;
    //                 serverRequest[i.day].second -= virtualServerList[i.id].ram;
    //             }
    //         }
    //     }
    // }
    // if (serverRequest[req.day].first < virSerType.core) serverRequest[req.day].first = virSerType.core;
    // if (serverRequest[req.day].second < virSerType.ram) serverRequest[req.day].second = virSerType.ram;

    // std::string ans = "";
    // bool bigger = 0; // 1->  ans比需求量大 0->ans比需求量小
    // for (auto & i : serverType) {
    //     if (ans == "") {
    //         ans = i.first;
    //         if (i.second.core > serverRequest[req.day].first && i.second.ram > serverRequest[req.day].second) {
    //             bigger = 1;
    //         }
    //         continue;
    //     }
    //     if (!bigger) { // 还没超过需求 尽量买大的
    //         if (i.second.core > serverType[ans].core && i.second.ram > serverType[ans].ram) {
    //             ans = i.first;
    //             if (i.second.core > serverRequest[req.day].first && i.second.ram > serverRequest[req.day].second) {
    //                 bigger = 1;
    //             }
    //         }
    //     } else { // 超过需求了 尽量买便宜的
    //         if (i.second.core > serverRequest[req.day].first && i.second.ram > serverRequest[req.day].second) {
    //             if (serverType[ans].cost + serverType[ans].dayCost * (T - req.day) > i.second.cost + i.second.dayCost * (T - req.day)) {
    //                 ans = i.first;
    //             }
    //         }
    //     }
    // }
    // serverRequest[req.day].first -= virtualServerType[ans].core;
    // serverRequest[req.day].second -= virtualServerType[ans].ram;

    // 策略一和二
    std::string ans = "";
    for (auto & i : serverType) {
        if (i.second.core / 2 > virSerType.core && i.second.ram / 2 > virSerType.ram) {
            if (ans == "") {
                ans = i.first;
                continue;
            }
            int nowCost = i.second.cost + i.second.dayCost * (T - req.day);
            int oldCost = serverType[ans].cost + serverType[ans].dayCost * (T - req.day);
            // 策略一 买总消耗最便宜的 消耗越小大概率越匹配 产生的碎片可能无法利用
            if (nowCost < oldCost) ans = i.first;
            // 策略二 买单核心均价最便宜的
            // if ((double)nowCost / (double)i.second.core < (double)oldCost / (double)serverType[ans].core) ans = i.first;
        }
    }

    if (isDouble == 0) {
        serverList[serverId] = serverType[ans];
        auto &temp = serverList[serverId];
        temp.lCore = temp.core / 2;
        temp.rCore = temp.core / 2;
        temp.lRam = temp.ram / 2;
        temp.rRam = temp.ram / 2;
        temp.id = serverId++;
        temp.createDay = req.day;
        // 默认就扔在左边
        temp.lCore -= virSerType.core;
        temp.lRam -= virSerType.ram;
        virtualServerList[req.id] = virtualServerType[req.name];
        virtualServerList[req.id].serverId = temp.id;
        virtualServerList[req.id].where = 0;
        virtualServerList[req.id].id = req.id;
        recordBuy(temp.createDay , temp.name);
        recordCreate(req.day , temp.id, 0);
    } else {
        serverList[serverId] = serverType[ans];
        auto &temp = serverList[serverId];
        temp.lCore = temp.core / 2;
        temp.rCore = temp.core / 2;
        temp.lRam = temp.ram / 2;
        temp.rRam = temp.ram / 2;
        temp.id = serverId++;
        temp.createDay = req.day;
        temp.lCore -= virSerType.core / 2;
        temp.rCore -= virSerType.core / 2;
        temp.lRam -= virSerType.ram / 2;
        temp.rRam -= virSerType.ram / 2;
        virtualServerList[req.id] = virtualServerType[req.name];
        virtualServerList[req.id].serverId = temp.id;
        virtualServerList[req.id].id = req.id;
        recordBuy(temp.createDay , temp.name);
        recordCreate(req.day , temp.id , -1);
    }
}

// 策略一与策略二
// 购买一台可以放得下的服务器
// void buyServer(request & req, virtualServer & virSerType , bool isDouble) {

//     if (isDouble == 0) {
//         serverList[serverId] = serverType[ans];
//         auto &temp = serverList[serverId];
//         temp.lCore = temp.core / 2;
//         temp.rCore = temp.core / 2;
//         temp.lRam = temp.ram / 2;
//         temp.rRam = temp.ram / 2;
//         temp.id = serverId++;
//         temp.createDay = req.day;
//         // 默认就扔在左边
//         temp.lCore -= virSerType.core;
//         temp.lRam -= virSerType.ram;
//         virtualServerList[req.id] = virtualServerType[req.name];
//         virtualServerList[req.id].serverId = temp.id;
//         virtualServerList[req.id].where = 0;
//         virtualServerList[req.id].id = req.id;
//         recordBuy(temp.createDay , temp.name);
//         recordCreate(req.day , temp.id, 0);
//     } else {
//         serverList[serverId] = serverType[ans];
//         auto &temp = serverList[serverId];
//         temp.lCore = temp.core / 2;
//         temp.rCore = temp.core / 2;
//         temp.lRam = temp.ram / 2;
//         temp.rRam = temp.ram / 2;
//         temp.id = serverId++;
//         temp.createDay = req.day;
//         temp.lCore -= virSerType.core / 2;
//         temp.rCore -= virSerType.core / 2;
//         temp.lRam -= virSerType.ram / 2;
//         temp.rRam -= virSerType.ram / 2;
//         virtualServerList[req.id] = virtualServerType[req.name];
//         virtualServerList[req.id].serverId = temp.id;
//         virtualServerList[req.id].id = req.id;
//         recordBuy(temp.createDay , temp.name);
//         recordCreate(req.day , temp.id , -1);
//     }
// }
