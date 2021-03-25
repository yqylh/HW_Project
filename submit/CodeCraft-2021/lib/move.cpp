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

    // 策略二 : 把剩余空间大的服务器上的虚拟机迁移到剩余空间小的服务器上
    std::vector<rateSolve> rate(serverList.size()); // 数值越大空间越高;
    for (auto & i : serverList) {
        rate[i.first] = rateSolve(i.first, i.second.lCore + i.second.rCore + i.second.lRam + i.second.rRam );
        // rate[i.first] = rateSolve(i.first, i.second.lCore + i.second.rCore + i.second.lRam + i.second.rRam - i.second.ram - i.second.core);
    }
    for (auto & i : virtualServerList) {
        rate[i.second.serverId].virSerIds.push_back(i.first);
    }

    std::sort(rate.begin(), rate.end()); // begin是剩余空间最小的, rbegin是剩余空间最大的
    // 从剩余空间从大到小去把服务器上的虚拟机id加到list里面
    std::vector<int> canMoveList;
    // int dontCareLess = rate.size() * 0.8;
    for (auto i = rate.rbegin(); i != rate.rend() && maxMoveNum /*&& dontCareLess*/; i++/* , dontCareLess--*/) {
        if ((double)(i->rate) / ((double)serverList[i->id].core + serverList[i->id].ram) > 0.8) continue;
        for (auto & j : i->virSerIds) if (maxMoveNum) {
            canMoveList.push_back(j);
            maxMoveNum--;
        }
    }

    auto cmp = [&](int & A, int & B) {
        return virtualServerList[A].core + virtualServerList[A].ram > virtualServerList[B].core + virtualServerList[B].ram;
    };
    std::sort(canMoveList.begin(), canMoveList.end(), cmp);

    // 对于每个可以移动的虚拟机 移动他
    for (auto & i : canMoveList) {
        for (auto j = rate.begin(); j != rate.end(); j++) {
            auto &virSer = virtualServerList[i];
            auto &Ser = serverList[j->id];
            if (virSer.isDouble == 1) {
                if (Ser.lCore > virSer.core / 2 && Ser.lRam > virSer.ram / 2 && Ser.rCore > virSer.core / 2 && Ser.rRam > virSer.ram / 2 && virSer.serverId != Ser.id) {
                    moveAction(moveMark(virSer.id, Ser.id, -1), day);
                    break;
                }
            } else {
                if (Ser.lCore > virSer.core && Ser.lRam > virSer.ram) {
                    if (Ser.id == virSer.serverId && virSer.where == 0) continue;
                    moveAction(moveMark(virSer.id, Ser.id, 0), day);
                    break;
                } else if (Ser.rCore > virSer.core && Ser.rRam > virSer.ram) {
                    if (Ser.id == virSer.serverId && virSer.where == 1) continue;
                    moveAction(moveMark(virSer.id, Ser.id, 1), day);
                    break;
                }
            }
        }
    }

    // int maxMoveNum = virtualServerList.size() * 5 / 1000;
    // std::vector<rateSolve> rate(serverList.size()); // 数值越大空间越高;
    // for (auto & i : serverList) {
    //     rate[i.first] = rateSolve(i.first, -i.second.lCore - i.second.rCore - i.second.lRam - i.second.rRam );
    // }
    // for (auto & i : virtualServerList) {
    //     rate[i.second.serverId].virSerIds.push_back(i.first);
    // }
    // std::sort(rate.begin(), rate.end()); // begin是剩余空间最小的, rbegin是剩余空间最大的
    // for(int i = 0 ; i < serverList.size() / 3 ; i++) { 
    //     for (auto & virSerId : rate[i].virSerIds) {
    //         for (int j = rate.size() - 1; j > i; j--) {
    //             auto &virSer = virtualServerList[virSerId];
    //             auto &Ser = serverList[rate[j].id];
    //             if (virSer.isDouble == 1) {
    //                 if (Ser.lCore > virSer.core / 2 && Ser.lRam > virSer.ram / 2 && Ser.rCore > virSer.core / 2 && Ser.rRam > virSer.ram / 2 && virSer.serverId != Ser.id) {
    //                     moveAction(moveMark(virSer.id, Ser.id, -1), day);
    //                     maxMoveNum--;
    //                     break;
    //                 }
    //             } else {
    //                 if (Ser.lCore > virSer.core && Ser.lRam > virSer.ram) {
    //                     if (Ser.id == virSer.serverId && virSer.where == 0) continue;
    //                     moveAction(moveMark(virSer.id, Ser.id, 0), day);
    //                     maxMoveNum--;
    //                     break;
    //                 } else if (Ser.rCore > virSer.core && Ser.rRam > virSer.ram) {
    //                     if (Ser.id == virSer.serverId && virSer.where == 1) continue;
    //                     moveAction(moveMark(virSer.id, Ser.id, 1), day);
    //                     maxMoveNum--;
    //                     break;
    //                 }
    //             }
    //         }
    //         if (maxMoveNum == 0) break;
    //     }
    //     if (maxMoveNum == 0) break;
    // }
}
#endif