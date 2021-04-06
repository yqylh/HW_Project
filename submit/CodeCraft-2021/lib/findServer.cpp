#include "input.cpp"
#include "init.cpp"

// 查找一台可以放得下该虚拟机的服务器
int findServer(request & req, virtualServer & virSerType , bool isDouble) {
    int retServerId; // 返回变量
    retServerId = -1;
    bool lorR = 0; // 0代表左边 1代表右边
    if (isDouble == 0) {
        for (auto & i : serverList) { // 查找可以塞的下的
            if (i.second.lCore >= virSerType.core && i.second.lRam >= virSerType.ram) {
                // 策略零 : 没有策略
                if (retServerId == -1) {
                    retServerId = i.first;
                    lorR = 0;
                } else {
                    // 策略一 : 找核心和内存都尽量满的服务器
                    if (i.second.lCore + i.second.rCore + i.second.lRam + i.second.rRam >
                        serverList[retServerId].lCore + serverList[retServerId].rCore + serverList[retServerId].lRam + serverList[retServerId].rRam ) continue;
                    if (lorR == 0) {
                        if (i.second.lCore + i.second.lRam < serverList[retServerId].lCore + serverList[retServerId].lRam) {
                            retServerId = i.first;
                            lorR = 0;
                        }
                    } else {
                        if (i.second.lCore + i.second.lRam < serverList[retServerId].rCore + serverList[retServerId].rRam) {
                            retServerId = i.first;
                            lorR = 0;
                        }
                    }
                }
            }
            if (i.second.rCore >= virSerType.core && i.second.rRam >= virSerType.ram) {
                // 策略零 : 没有策略
                if (retServerId == -1) {
                    retServerId = i.first;
                    lorR = 1;
                } else {
                    if (i.second.lCore + i.second.rCore + i.second.lRam + i.second.rRam >
                        serverList[retServerId].lCore + serverList[retServerId].rCore + serverList[retServerId].lRam + serverList[retServerId].rRam ) continue;
                    // 策略一 : 找核心和内存都尽量满的服务器
                    if (lorR == 0) {
                        if (i.second.rCore +  i.second.rRam < serverList[retServerId].lCore + serverList[retServerId].lRam) {
                            retServerId = i.first;
                            lorR = 1;
                        }
                    } else {
                        if (i.second.rCore +  i.second.rRam < serverList[retServerId].rCore + serverList[retServerId].rRam) {
                            retServerId = i.first;
                            lorR = 1;
                        }
                    }
                }
            }
        }
        // 充分比较左右两个子节点
        // for (auto & i : serverList) {
        //     // 查找可以塞的下的
        //     if ( (i.second.lCore >= virSerType.core && i.second.lRam >= virSerType.ram) || (i.second.rCore >= virSerType.core && i.second.rRam >= virSerType.ram)) {
        //         if (retServerId == -1) {
        //             retServerId = i.first;
        //             if (i.second.lCore >= virSerType.core && i.second.lRam >= virSerType.ram) {
        //                 if (i.second.rCore >= virSerType.core && i.second.rRam >= virSerType.ram ) {
        //                     if (i.second.lCore + i.second.lRam > i.second.rCore + i.second.rRam) {
        //                         lorR = 1;
        //                     } else {
        //                         lorR = 0;
        //                     }
        //                 } else {
        //                     lorR = 0;
        //                 }
        //             } else {
        //                 lorR = 1;
        //             }
        //             continue;
        //         }
        //         if (i.second.lCore + i.second.rCore + i.second.lRam + i.second.rRam >= serverList[retServerId].lCore + serverList[retServerId].rCore + serverList[retServerId].lRam + serverList[retServerId].rRam ) continue;

        //         retServerId = i.first;
        //         if (i.second.lCore >= virSerType.core && i.second.lRam >= virSerType.ram) {
        //             if (i.second.rCore >= virSerType.core && i.second.rRam >= virSerType.ram ) {
        //                 if (i.second.lCore + i.second.lRam > i.second.rCore + i.second.rRam) {
        //                     lorR = 1;
        //                 } else {
        //                     lorR = 0;
        //                 }
        //             } else {
        //                 lorR = 0;
        //             }
        //         } else {
        //             lorR = 1;
        //         }
        //     }
        // }

        if (retServerId != -1) {
            auto &x = serverList[retServerId];
            if (lorR == 0) {
                x.lCore -= virSerType.core;
                x.lRam -= virSerType.ram;
            } else {
                x.rCore -= virSerType.core;
                x.rRam -= virSerType.ram;
            }
            virtualServerList[req.id] = virtualServerType[req.name];
            virtualServerList[req.id].serverId = retServerId;
            virtualServerList[req.id].where = lorR;
            virtualServerList[req.id].id = req.id;
            recordCreate(req.day , retServerId , lorR, req.order);
        }
    } else {
        for (auto & i : serverList) {
            if (i.second.lCore >= virSerType.core / 2 && i.second.lRam >= virSerType.ram / 2 && i.second.rCore >= virSerType.core / 2 && i.second.rRam >= virSerType.ram / 2) {
                // 策略零
                if (retServerId == -1) retServerId = i.first;
                else {
                    // 策略一 : 找核心和内存都尽量满的服务器
                    if (i.second.lCore + i.second.rCore + i.second.lRam + i.second.rRam <
                        serverList[retServerId].lCore + serverList[retServerId].rCore + serverList[retServerId].lRam + serverList[retServerId].rRam ) {
                        retServerId = i.first;
                    }
                }
            }
        }
        if (retServerId != -1) {
            auto &x = serverList[retServerId];
            x.lCore -= virSerType.core / 2;
            x.lRam -= virSerType.ram / 2;
            x.rCore -= virSerType.core / 2;
            x.rRam -= virSerType.ram / 2;
            virtualServerList[req.id] = virtualServerType[req.name];
            virtualServerList[req.id].serverId = x.id;
            virtualServerList[req.id].id = req.id;
            recordCreate(req.day , retServerId, -1, req.order);
        }
    }
    return retServerId;
}
