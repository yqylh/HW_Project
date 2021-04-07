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
    for (int day = 1; day <= K; ++day) {
        inputRequest(day);
    }
    for (int day = 1; day <= T - K; ++day) {
        if (day != 1) {
            move(day);
        }
        for (auto req = requestList[day].begin(); req != requestList[day].end(); req++){
            // 创建请求
            if (req->type == 0) {
                auto &virSerType = virtualServerType[req->name];
                if (findServer(*req, virSerType, virSerType.isDouble)  == -1) {
                    buyServer(req, virSerType, virSerType.isDouble);
                }
            } else { // 删除
                deleteVitrualServer(req->id);
            }
        }
        output(day);
        inputRequest(day + K);
    }
    for (int day = T - K + 1; day <= T; ++day) {
        move(day);
        for (auto req = requestList[day].begin(); req != requestList[day].end(); req++){
            // 创建请求
            if (req->type == 0) {
                auto &virSerType = virtualServerType[req->name];
                if (findServer(*req, virSerType, virSerType.isDouble)  == -1) {
                    buyServer(req, virSerType, virSerType.isDouble);
                }
            } else { // 删除
                deleteVitrualServer(req->id);
            }
        }
        output(day);
    }
    return 0;
}