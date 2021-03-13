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
			// move(day);
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
