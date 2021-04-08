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
    for (int day = 1; day <= K; ++day) inputRequest(day);
    for (int day = 1; day <= T; ++day) {
        
        if (day != 1) move(day);

        for (int i = 0; i < requestList[day].size(); i++) {
            auto req = requestList[day][i];
            if (req.type == 0) {
                auto &virSerType = virtualServerType[req.name];
                if (findServer(req, virSerType, virSerType.isDouble)  == -1) {
                    std::vector<request> arr;
                    // 参数一
                    int limit = 12;
                    while (limit-- && i < requestList[day].size()) {
                        req = requestList[day][i];
                        if (req.type == 1) break;
                        arr.push_back(req);
                        i++;
                    }
                    i--;
                    buyServer(arr, day);
                }
            } else { // 删除
                deleteVitrualServer(req.id);
            }
        }
        #ifdef EBUG
            // int reqnum = 0;
            // for (auto & req : requestList[day]) if (req.type == 0) reqnum++;
            // std::cout << reqnum << " " << CreateList[day].size() << std::endl;
            // if (reqnum != CreateList[day].size()) {
            //     int ans = 0;
            //     for (auto & create : CreateList[day]) {
            //         bool flag = 0;
            //         for (auto & req : requestList[day]) {
            //             if (req.order == create.order) {
            //                 // std::cout << req.type << std::endl;
            //                 ans++;
            //                 flag = 1;
            //                 break;
            //             }
            //         }
            //         if (!flag) {
            //             std::cout << create.order << std::endl;
            //         }
            //     }
            //     // std::cout << ans << std::endl;
            //     throw;
            // }
        #else
            output(day);
        #endif

        if (day <= T - K) inputRequest(day + K);
    }
    return 0;
}