#include <iostream>
#include "lib/server.cpp"
#include "lib/virtualServer.cpp"
#include "lib/moveMark.cpp"
#include "lib/input.cpp"
#include "lib/request.cpp"
#include "lib/output.cpp"


/* 初始化 , 把购买的List和创建的List清空 */
void init() {
	for (int i = 0; i < T + 5; i++) BuyList.push_back(std::vector<std::string> (0) );
	for (int i = 0; i < T + 5; i++) CreateList.push_back(std::vector<std::pair<int, int> >(0) );
	for (int i = 0; i < T + 5; i++) moveList.push_back(std::vector<moveMark>(0) );
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
				// 策略零 : 没有策略
				if (retServerId == -1) {
					retServerId = i.first;
					lorR = 0;
				} else {
					// 策略一 : 找核心和内存都尽量满的服务器
					if (lorR == 0) {
						if (i.second.lCore < serverList[retServerId].lCore && i.second.lRam < serverList[retServerId].lRam) {
							retServerId = i.first;
							lorR = 0;
						}
					} else {
						if (i.second.lCore < serverList[retServerId].rCore && i.second.lRam < serverList[retServerId].rRam) {
							retServerId = i.first;
							lorR = 0;
						}
					}
				}
			}
			if (i.second.rCore > virSerType.core && i.second.rRam > virSerType.ram) {
				// 策略零 : 没有策略
				if (retServerId == -1) {
					retServerId = i.first;
					lorR = 1;
				} else {
					// 策略一 : 找核心和内存都尽量满的服务器
					if (lorR == 0) {
						if (i.second.rCore < serverList[retServerId].lCore && i.second.rRam < serverList[retServerId].lRam) {
							retServerId = i.first;
							lorR = 1;
						}
					} else {
						if (i.second.rCore < serverList[retServerId].rCore && i.second.rRam < serverList[retServerId].rRam) {
							retServerId = i.first;
							lorR = 1;
						}
					}
				}
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
				// 策略零
				if (retServerId == -1) retServerId = i.first;
				else {
					// 策略一 : 找核心和内存都尽量满的服务器
					if (i.second.lCore + i.second.rCore < serverList[retServerId].lCore + serverList[retServerId].rCore && 
					    	i.second.lRam + i.second.rRam < serverList[retServerId].lRam + serverList[retServerId].rRam ) {
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
			recordCreate(req.day , retServerId, -1);
		}
	}
	return retServerId;
}

// 购买一台可以放得下的服务器
void buyServer(request & req, virtualServer & virSerType , bool isDouble) {
	std::string ans = "";
	for (auto & i : serverType) { 
		if (i.second.core / 2 > virSerType.core && i.second.ram / 2 > virSerType.ram) {
			if (ans == "") {
				ans = i.first;
				continue;
			}
			// 策略零 没有策略
			// ans = i.first;

			int nowCost = i.second.cost  /*+ i.second.dayCost * (T - req.day)*/;
			int oldCost = serverType[ans].cost  /*+ serverType[ans].dayCost * (T - req.day)*/;
			// 策略一 买总消耗最便宜的 13亿
			if (nowCost < oldCost) ans = i.first;
			
			// 策略二 买单核心均价最便宜的 43亿
			// if (nowCost / i.second.core < oldCost / serverType[ans].core) ans = i.first;
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
		temp.lCore -= virSerType.core / 2;
		temp.rCore -= virSerType.core / 2;
		temp.lRam -= virSerType.ram / 2;
		temp.rRam -= virSerType.ram / 2;
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
void move(int day) {
	int maxMoveNum = virtualServerList.size() * 5 / 1000;
	for (auto & i : virtualServerList) if (maxMoveNum) {
		for (auto & j : serverList) {
			auto & before = serverList[i.second.serverId];
			if (i.second.isDouble == 0) {
				 // 策略
				 // 左右核心可以互相移动
				 if (j.second.lCore > i.second.core && j.second.lRam > i.second.ram && i.second.serverId != j.first) {
					j.second.lCore -= i.second.core;
					j.second.lRam -= i.second.ram;
					if (i.second.where == 0) {
						before.lCore += i.second.core;
						before.lRam += i.second.ram;
					} else {
						before.rCore += i.second.core;
						before.rRam += i.second.ram;
					}
					i.second.serverId = j.first;
					i.second.where = 0;
					moveList[day].push_back(moveMark(i.first, j.first, 0));
					maxMoveNum--;
					break;
				 }
				 if (j.second.rCore > i.second.core && j.second.rRam > i.second.ram && i.second.serverId != j.first) {
					j.second.rCore -= i.second.core;
					j.second.rRam -= i.second.ram;
					if (i.second.where == 0) {
						before.lCore += i.second.core;
						before.lRam += i.second.ram;
					} else {
						before.rCore += i.second.core;
						before.rRam += i.second.ram;
					}
					i.second.serverId = j.first;
					i.second.where = 1;
					moveList[day].push_back(moveMark(i.first, j.first, 1));
					maxMoveNum--;
					break;
				 }
			} else {
				// 策略
				if (j.second.lCore > i.second.core && j.second.lRam > i.second.ram &&
					j.second.rCore > i.second.core && j.second.rRam > i.second.ram && i.second.serverId != j.first) {

					j.second.lCore -= i.second.core / 2;
					j.second.lRam -= i.second.ram / 2;
					j.second.rCore -= i.second.core / 2;
					j.second.rRam -= i.second.ram / 2;

					before.lCore += i.second.core / 2;
					before.lRam += i.second.ram / 2;
					before.rCore += i.second.core / 2;
					before.rRam += i.second.ram / 2;
					
					i.second.serverId = j.first;
					moveList[day].push_back(moveMark(i.first, j.first, -1));
					maxMoveNum--;
					break;
				}
			}
		}
		//  测试用
		break;
	}
}

int main() {
	input();
	init();
	int day = 1;
	for (auto & req : requestList) {
		// if (day != req.day) {
		// 	day = req.day;
		// 	// 迁移
		// 	move(day);
		// }
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
