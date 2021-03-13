#ifndef __move__
#define __move__
#include "rateSolve.h"
#include "input.cpp"
// 迁移行动
void moveAction(moveMark x, int day) {
    moveList[day].push_back(moveMark(x.virSerId, x.serId, x.where));
    auto & virSer = virtualServerList[x.virSerId];
    auto & ser = serverList[x.serId];
    auto & before = serverList[virSer.serverId];
    // 单核心迁移
    if (virSer.isDouble == 0) {
        // 迁移到左边
        if (x.where == 0) {
            ser.lCore -= virSer.core;
            ser.lRam -= virSer.ram;
            if (virSer.where == 0) {
                before.lCore += virSer.core;
                before.lRam += virSer.ram;
            } else {
                before.rCore += virSer.core;
                before.rRam += virSer.ram;
            }
            virSer.serverId = ser.id;
            virSer.where = 0;
        } else { // 迁移到右边
            ser.rCore -= virSer.core;
            ser.rRam -= virSer.ram;
            if (virSer.where == 0) {
                before.lCore += virSer.core;
                before.lRam += virSer.ram;
            } else {
                before.rCore += virSer.core;
                before.rRam += virSer.ram;
            }
            virSer.serverId = ser.id;
            virSer.where = 1;
        }
    } else { // 双核心迁移
        ser.lCore -= virSer.core / 2;
        ser.lRam -= virSer.ram / 2;
        ser.rCore -= virSer.core / 2;
        ser.rRam -= virSer.ram / 2;

        before.lCore += virSer.core / 2;
        before.lRam += virSer.ram / 2;
        before.rCore += virSer.core / 2;
        before.rRam += virSer.ram / 2;

        virSer.serverId = ser.id;
    }
}

// 迁移策略
void move(int day) {
    int maxMoveNum = virtualServerList.size() * 5 / 1000;
    // 策略一 : 把利用率低的服务器上的虚拟机迁移到利用率高的服务器上
    // 先只考虑双核心虚拟机
    std::vector<rateSolve> rate; // 数值越小利用率越高;
    for (auto & i : serverList) {
        rate.push_back(rateSolve(i.first, (double)(i.second.lCore + i.second.rCore) / (double)(i.second.core) + (double)(i.second.lRam + i.second.lRam) / (double)(i.second.ram)));
    }
    std::sort(rate.begin(), rate.end()); // begin是利用率最高的, rbegin是利用率最低的
    for (auto & i : virtualServerList) if (i.second.isDouble == 1) {
        rate[i.second.serverId].virSerId.push_back(i.first);
    }
    std::vector<int> canMoveList;
    for (auto i = rate.rbegin(); i != rate.rend() && maxMoveNum; i++) {
        for (auto & j : i->virSerId) if (maxMoveNum) {
            canMoveList.push_back(j);
            maxMoveNum--;
        }
    }
    for (auto & i : canMoveList) {
        for (auto j = rate.begin(); j != rate.end(); j++) {
            auto &virSer = virtualServerList[i];
            auto &Ser = serverList[j->id];
            if (Ser.lCore > virSer.core / 2 && Ser.lRam > virSer.ram / 2 && Ser.rCore > virSer.core / 2 && Ser.rRam > virSer.ram / 2 && virSer.serverId != Ser.id) {
                moveAction(moveMark(virSer.id, Ser.id, -1), day);
                break;
            }
        }
    }

    // 策略零 : 能迁移就迁移
    // for (auto & i : virtualServerList) if (maxMoveNum) {
    //     auto & before = serverList[i.second.serverId];

    //     int moveServerId; // 返回变量
    //     moveServerId = -1;
    //     bool lorR = 0; // 0代表左边 1代表右边

    //     if (i.second.isDouble == 0) {
    //         for (auto & j : serverList) {
    //             if (j.second.lCore > i.second.core && j.second.lRam > i.second.ram ) {
    //                 if (i.second.serverId == j.first && i.second.where == 0) continue;
    //                 // 策略零
    //                 if (moveServerId == -1) {
    //                     moveServerId = j.first;
    //                     lorR = 0;
    //                 } else {
    //                     // 策略一
    //                 }
    //             }
    //             if (j.second.rCore > i.second.core && j.second.rRam > i.second.ram ) {
    //                 if (i.second.serverId == j.first && i.second.where == 1) continue;
    //                 // 策略零
    //                 if (moveServerId == -1) {
    //                     moveServerId = j.first;
    //                     lorR = 1;
    //                 } else {
    //                     // 策略一
    //                 }
    //             }
    //         }
    //         if (moveServerId != -1) {
    //             moveAction(moveMark(i.first, moveServerId, lorR), day);
    //             maxMoveNum--;
    //         }
    //     } else {
    //         // 策略
    //         for (auto & j : serverList) {
    //             if (j.second.lCore > i.second.core / 2 && j.second.lRam > i.second.ram / 2 &&
    //                 j.second.rCore > i.second.core / 2 && j.second.rRam > i.second.ram / 2 && i.second.serverId != j.first) {
    //                 // 策略零
    //                 if (moveServerId == -1) {
    //                     moveServerId = j.first;
    //                 } else {
    //                     // 策略一
    //                 }
    //             }
    //         }
    //         if (moveServerId != -1) {
    //             moveAction(moveMark(i.first, moveServerId, -1), day);
    //             maxMoveNum--;
    //         }
    //     }
    //     break;
    // }
}

#endif