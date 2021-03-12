#include <iostream>
#include "lib/server.cpp"
#include "lib/virtualServer.cpp"
#include "lib/input.cpp"
#include "lib/request.cpp"

int serverId = 0;
std::vector<std::vector<std::string> > BuyList;
std::vector<std::vector<std::pair<int, int> > > CreateList;

int main() {
	input();
	for (int i = 0; i < T + 5; i++) BuyList.push_back(std::vector<std::string> (0) );
	for (int i = 0; i < T + 5; i++) CreateList.push_back(std::vector<std::pair<int, int> >(0) );
	for (auto & i : requestList) {
		if (i.type == 0) { // 创建请求
			auto x = virtualServerType[i.name];
			// std::cout << x.name << " " << i.name << "\n";
			if (x.isDouble == 0) { // 单核心布置
				bool flag = 0;
				for (auto & j : serverList) { // 查找可以塞的下的
					if (j.second.lCore > x.core && j.second.lRam > x.ram) { 
						j.second.lCore -= x.core;
						j.second.lRam -= x.ram;
						virtualServerList[i.id] = virtualServerType[i.name];
						virtualServerList[i.id].serverId = j.second.id;
						virtualServerList[i.id].where = 0;
						// 记录创建
						CreateList[i.day].push_back(std::make_pair(j.second.id , 0));
						// 记录完毕
						flag = 1;
					} else if (j.second.rCore > x.core && j.second.rRam > x.ram) {
						j.second.rCore -= x.core;
						j.second.rRam -= x.ram;
						virtualServerList[i.id] = virtualServerType[i.name];
						virtualServerList[i.id].serverId = j.second.id;
						virtualServerList[i.id].where = 1;
						// 记录创建
						CreateList[i.day].push_back(std::make_pair(j.second.id , 1));
						// 记录完毕
						flag = 1;
					}
					if (flag) break;
				}
				if (!flag) { // 如果没找到可以塞的下的
					for (auto & j : serverType) { // 找一台新的可以塞的下的
						if (j.second.core / 2 > x.core && j.second.ram / 2 > x.ram) {
							serverList[serverId] = j.second;
							auto &temp = serverList[serverId];
							temp.lCore = temp.core / 2;
							temp.rCore = temp.core / 2;
							temp.lRam = temp.ram / 2;
							temp.rRam = temp.ram / 2;
							temp.id = serverId++;
							temp.createDay = i.day;
							temp.lCore -= x.core;
							temp.lRam -= x.ram;
							virtualServerList[i.id] = virtualServerType[i.name];
							virtualServerList[i.id].serverId = temp.id;
							virtualServerList[i.id].where = 0;
							// 记录购买
							BuyList[temp.createDay].push_back(temp.name);
							// 记录结束
							// 记录创建
							CreateList[i.day].push_back(std::make_pair(temp.id , 0));
							// 记录完毕
							break;
						}
					}
				}
			} else {
				bool flag = 0;
				for (auto & j : serverList) { 
					if (j.second.lCore > x.core && j.second.lRam > x.ram && j.second.rCore > x.core && j.second.rRam > x.ram) {
						j.second.lCore -= x.core;
						j.second.lRam -= x.ram;
						j.second.rCore -= x.core;
						j.second.rRam -= x.ram;
						virtualServerList[i.id] = virtualServerType[i.name];
						virtualServerList[i.id].serverId = j.second.id;
						flag = 1;
						// 记录创建
						CreateList[i.day].push_back(std::make_pair(j.second.id , -1));
						// 记录完毕
					}
					if (flag) break;
				}
				if (!flag) {
					for (auto & j : serverType) {
						if (j.second.core / 2 > x.core && j.second.ram / 2 > x.ram) {
							serverList[serverId] = j.second;
							auto &temp = serverList[serverId];
							temp.lCore = temp.core / 2;
							temp.rCore = temp.core / 2;
							temp.lRam = temp.ram / 2;
							temp.rRam = temp.ram / 2;
							temp.id = serverId++;
							temp.createDay = i.day;
							temp.lCore -= x.core;
							temp.rCore -= x.core;
							temp.lRam -= x.ram;
							temp.rRam -= x.ram;
							virtualServerList[i.id] = virtualServerType[i.name];
							virtualServerList[i.id].serverId = temp.id;
							// 记录购买
							BuyList[temp.createDay].push_back(temp.name);
							// 记录结束
							// 记录创建
							CreateList[i.day].push_back(std::make_pair(temp.id , -1));
							// 记录完毕
							break;
						}
					}
				}
			}
		} else { // 删除
			auto &x = virtualServerList[i.id];
			if (x.isDouble == 1) {
				serverList[x.serverId].lCore += x.core / 2;
				serverList[x.serverId].rCore += x.core / 2;
				serverList[x.serverId].lRam += x.ram / 2;
				serverList[x.serverId].rRam += x.ram / 2;
			} else {
				if (x.where == 0) {
					serverList[x.serverId].lCore += x.core / 2;
					serverList[x.serverId].lRam += x.ram / 2;
				} else {
					serverList[x.serverId].rCore += x.core / 2;
					serverList[x.serverId].rRam += x.ram / 2;
				}
			}
			virtualServerList.erase(i.id);
		}
	}
	
	
	for (int i = 1; i <= T; i++) {
		std::map<std::string , int> temp;
		for (auto & j : BuyList[i]) {
			if (temp.count(j) == 0) {
				temp[j] = 1;
			} else {
				temp[j]++;
			}
		}
		// 服务器编号理解错了
		printf("(purchase, %d)\n", temp.size());
		for (auto & j : temp) {
			std::cout << "(" << j.first << ", " << j.second << ")\n";
		}
		std::cout << "(migration, 0)\n";
		for (auto & j : CreateList[i]) {
			if (j.second == -1) {
				printf("(%d)\n" , j.first);
			} else {
				printf("(%d, %c)\n" , j.first , (j.second == 0 ? 'A' : 'B')  );
			}
		}
	}
	return 0;
}
