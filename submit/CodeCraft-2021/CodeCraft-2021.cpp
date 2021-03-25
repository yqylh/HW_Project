#include "lib/server.h"
#include "lib/virtualServer.h"
#include "lib/moveMark.h"
#include "lib/request.h"
#include "lib/input.cpp"
#include "lib/output.cpp"
#include "lib/move.cpp"
#include "lib/delete.cpp"
#include "lib/init.cpp"
#include "lib/buyServer.cpp"
#include "lib/findServer.cpp"

int main() {
    input();
    init();
    int day = 1;
    for (auto & req : requestList) {
        if (day != req.day) {
            day = req.day;
            // 迁移
            move(day);
        }
        // 创建请求
        if (req.type == 0) {
            auto &virSerType = virtualServerType[req.name];
            if (findServer(req, virSerType, virSerType.isDouble)  == -1) {
                buyServer(req, virSerType, virSerType.isDouble);
            }
        } else { // 删除
            deleteVitrualServer(req.id);
        }
    }
    output();
    return 0;
}
    // for (auto req = requestList.begin(); req != requestList.end();) {
    //     if (day != req->day) {
    //         day = req->day;
    //         move(day);
    //     }
    //     if (req->type == 1) {
    //         deleteVitrualServer(req->id);
    //         req++;
    //         continue;
    //     }
    //     auto j = req;
    //     for (int k = 0; k < 1000 && j != requestList.end(); j++) {
    //         if (j->day != req->day) break;
    //         if (j->type == 1) break;
    //     }
    //     auto cmp = [&](request &i, request &j) {return i.rate < j.rate;};
    //     auto cmp2 = [&](request &i, request &j) {
    //         return virtualServerType[i.name].core + virtualServerType[i.name].ram > virtualServerType[j.name].core + virtualServerType[j.name].ram;
    //     };
    //     while (req != j) {
    //         std::sort(req , j , cmp2);
    //         for (auto & ser : serverList) {
    //             auto & virSer = virtualServerType[req->name];
    //             bool flag = 0;
    //             if (virSer.isDouble == 0) {
    //                 if (((ser.second.lCore >= virSer.core && ser.second.lRam >= virSer.ram))||((ser.second.rCore >= virSer.core && ser.second.rRam >= virSer.ram))) {
    //                     flag = 1;
    //                 }
    //             } else {
    //                 if (ser.second.lCore >= virSer.core / 2 && ser.second.lRam >= virSer.ram/2 && ser.second.rCore >= virSer.core/2 && ser.second.rRam >= virSer.ram/2) {
    //                     flag = 1;
    //                 }
    //             }
    //             if (flag) {
    //                 for (auto k = req; k != j; k++) {
    //                     k->rate = INT_MAX;
    //                     auto &virSer = virtualServerType[k->name];
    //                     bool flag = 0;
    //                     if (virSer.isDouble == 0) {
    //                         if (((ser.second.lCore >= virSer.core && ser.second.lRam >= virSer.ram))||((ser.second.rCore >= virSer.core && ser.second.rRam >= virSer.ram))) {
    //                             flag = 1;
    //                             int rate = ser.second.lCore + ser.second.lRam + ser.second.rCore + ser.second.rRam - virSer.core - virSer.ram;
    //                             if (rate < k->rate) {
    //                                 k->rate = rate;
    //                             }
    //                         }
    //                     } else {
    //                         if (ser.second.lCore >= virSer.core / 2 && ser.second.lRam >= virSer.ram/2 && ser.second.rCore >= virSer.core/2 && ser.second.rRam >= virSer.ram/2) {
    //                             flag = 1;
    //                             int rate = ser.second.lCore + ser.second.lRam + ser.second.rCore + ser.second.rRam - virSer.core - virSer.ram;
    //                             if (rate < k->rate) {
    //                                 k->rate = rate;
    //                             }
    //                         }
    //                     }
    //                     if (!flag) break;
    //                 }
    //             }
    //         }
    //         std::sort(req , j , cmp);
    //         auto &virSerType = virtualServerType[req->name];
    //         if (findServer(*req, virSerType, virSerType.isDouble)  == -1) {
    //             buyServer(*req, virSerType, virSerType.isDouble);
    //         }
    //         req++;
    //     }
    // }
