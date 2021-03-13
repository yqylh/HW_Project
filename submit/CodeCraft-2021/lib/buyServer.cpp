#include "input.cpp"
#include "init.cpp"

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

			int nowCost = i.second.cost + i.second.dayCost * (T - req.day);
			int oldCost = serverType[ans].cost + serverType[ans].dayCost * (T - req.day);
			// 策略一 买总消耗最便宜的 13亿 消耗越小大概率越匹配 产生的碎片可能无法利用
			if (nowCost < oldCost) ans = i.first;

			// 策略二 买单核心均价最便宜的 43亿
			// if ((double)nowCost / (double)i.second.core < (double)oldCost / (double)serverType[ans].core) ans = i.first;

			// 策略三 按照一天的量进行考虑
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
		virtualServerList[req.id].id = req.id;
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
		virtualServerList[req.id].id = req.id;
		recordBuy(temp.createDay , temp.name);
		recordCreate(req.day , temp.id , -1);
	}
}
