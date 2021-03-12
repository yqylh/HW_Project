#include <iostream>
#include "lib/server.cpp"
#include "lib/virtualServer.cpp"
#include "lib/input.cpp"
#include "lib/request.cpp"
#include "lib/output.cpp"


/* 初始化 , 把购买的List和创建的List清空 */
void init() {
	for (int i = 0; i < T + 5; i++) BuyList.push_back(std::vector<std::string> (0) );
	for (int i = 0; i < T + 5; i++) CreateList.push_back(std::vector<std::pair<int, int> >(0) );
}
// 记录创建一台虚拟机
void recordCreate(int _day , int _serverId , int _type) {
	CreateList[_day].push_back(std::make_pair(_serverId , _type));
}
// 记录购买一台服务器
void recordBuy(int _day, std::string _name) {
	BuyList[_day].push_back(_name);
}
// 删除一台虚拟机
void deleteVitrualServer(int _id) {
	// 找到这个id对应的实例
	auto &x = virtualServerList[_id];
	// 双节点
	if (x.isDouble == 1) {
		serverList[x.serverId].lCore += x.core / 2;
		serverList[x.serverId].rCore += x.core / 2;
		serverList[x.serverId].lRam += x.ram / 2;
		serverList[x.serverId].rRam += x.ram / 2;
	} else { // 单节点
		if (x.where == 0) {
			serverList[x.serverId].lCore += x.core;
			serverList[x.serverId].lRam += x.ram;
		} else {
			serverList[x.serverId].rCore += x.core;
			serverList[x.serverId].rRam += x.ram;
		}
	}
	virtualServerList.erase(_id);
}
// 查找一台可以放得下该虚拟机的服务器
int findServer(request & req, virtualServer & virSerType , bool isDouble) {
	int retServerId; // 返回变量
	retServerId = -1;
	bool lorR = 0; // 0代表左边 1代表右边
	if (isDouble == 0) {
		for (auto & i : serverList) { // 查找可以塞的下的
			if (i.second.lCore > virSerType.core && i.second.lRam > virSerType.ram) {
				// 策略
				retServerId = i.first;
				lorR = 0;
			} else if (i.second.rCore > virSerType.core && i.second.rRam > virSerType.ram) {
				// 策略
				retServerId = i.first;
				lorR = 1;
			}
		}
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
			recordCreate(req.day , retServerId , lorR);
		}
	} else {
		for (auto & i : serverList) {
			if (i.second.lCore > virSerType.core && i.second.lRam > virSerType.ram && i.second.rCore > virSerType.core && i.second.rRam > virSerType.ram) {
				// 策略
				retServerId = i.first;
			}
		}
		if (retServerId != -1) {
			auto &x = serverList[retServerId];
			x.lCore -= virSerType.core;
			x.lRam -= virSerType.ram;
			x.rCore -= virSerType.core;
			x.rRam -= virSerType.ram;
			virtualServerList[req.id] = virtualServerType[req.name];
			virtualServerList[req.id].serverId = x.id;
			recordCreate(req.day , retServerId, -1);
		}
	}
	return retServerId;
}

// 购买一台可以放得下的服务器
void buyServer(request & req, virtualServer & virSerType , bool isDouble) {
	std::string ans;
	for (auto & i : serverType) { 
		if (i.second.core / 2 > virSerType.core && i.second.ram / 2 > virSerType.ram) {
			// 策略
			ans = i.first;
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
		temp.lCore -= virSerType.core;
		temp.rCore -= virSerType.core;
		temp.lRam -= virSerType.ram;
		temp.rRam -= virSerType.ram;
		virtualServerList[req.id] = virtualServerType[req.name];
		virtualServerList[req.id].serverId = temp.id;
		recordBuy(temp.createDay , temp.name);
		recordCreate(req.day , temp.id , -1);
	}
}

// 创建单核心虚拟机
void createOddServer(request & req, virtualServer & virSerType) {
	int ans = findServer(req, virSerType, 0);
	if (ans == -1)  { // 如果没找到可以塞的下的
		buyServer(req, virSerType, 0);
	}
}
// 创建双核心虚拟机
void createDoubleServer(request & req, virtualServer & virSerType) {
	int ans = findServer(req, virSerType, 1);
	if (ans == -1) {
		buyServer(req, virSerType, 1);
	}
}

// 迁移策略
void move() {
	
}

int main() {
	input();
	init();
	int day = 1;
	for (auto & req : requestList) {
		if (day != req.day) {
			day = req.day;
			move();
		}
		// 创建请求
		if (req.type == 0) {
			auto &virSerType = virtualServerType[req.name];
			if (virSerType.isDouble == 0) { // 单核心布置
				createOddServer(req , virSerType);
			} else { // 双核心配置
				createDoubleServer(req , virSerType);
			}
		} else { // 删除
			deleteVitrualServer(req.id);
		}
	}
	output();
	return 0;
}
