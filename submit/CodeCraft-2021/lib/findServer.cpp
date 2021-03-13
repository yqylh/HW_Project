#include "input.cpp"
#include "init.cpp"

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
					// 策略二 : 找核心和内存使用比例高的服务器
					// if (lorR == 0) {
					// 	if ( (double)(i.second.lCore - virSerType.core) / (double)(i.second.core) / 2 
					// 		< (double)(serverList[retServerId].lCore - virSerType.core) / (double)(serverList[retServerId].core) / 2 &&
					// 		(double)(i.second.lRam - virSerType.ram) / (double)(i.second.ram)  / 2
					// 		< (double)(serverList[retServerId].lRam - virSerType.ram) / (double)(serverList[retServerId].ram) / 2 ){

					// 		retServerId = i.first;
					// 		lorR = 0;
					// 	}
					// } else {
					// 	if ( (double)(i.second.lCore - virSerType.core) / (double)(i.second.core) / 2 
					// 		< (double)(serverList[retServerId].rCore - virSerType.core) / (double)(serverList[retServerId].core) / 2 &&
					// 		(double)(i.second.lRam - virSerType.ram) / (double)(i.second.ram)  / 2
					// 		< (double)(serverList[retServerId].rRam - virSerType.ram) / (double)(serverList[retServerId].ram) / 2 ){
					// 		retServerId = i.first;
					// 		lorR = 0;
					// 	}
					// }
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
					// 策略二 : 找核心和内存使用比例高的服务器
					// if (lorR == 0) {
					// 	if ( (double)(i.second.rCore - virSerType.core) / (double)(i.second.core) / 2 
					// 		< (double)(serverList[retServerId].lCore - virSerType.core) / (double)(serverList[retServerId].core) / 2 &&
					// 		(double)(i.second.rRam - virSerType.ram) / (double)(i.second.ram)  / 2
					// 		< (double)(serverList[retServerId].lRam - virSerType.ram) / (double)(serverList[retServerId].ram) / 2 ){

					// 		retServerId = i.first;
					// 		lorR = 1;
					// 	}
					// } else {
					// 	if ( (double)(i.second.rCore - virSerType.core) / (double)(i.second.core) / 2 
					// 		< (double)(serverList[retServerId].rCore - virSerType.core) / (double)(serverList[retServerId].core) / 2 &&
					// 		(double)(i.second.rRam - virSerType.ram) / (double)(i.second.ram)  / 2
					// 		< (double)(serverList[retServerId].rRam - virSerType.ram) / (double)(serverList[retServerId].ram) / 2 ){
					// 		retServerId = i.first;
					// 		lorR = 1;
					// 	}
					// }
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
			virtualServerList[req.id].id = req.id;
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
					// 策略二 : 找核心和内存使用比例高的服务器
					// if ( (double)(i.second.lCore + i.second.rCore - virSerType.core) / (double)(i.second.core) 
					// 		< (double)(serverList[retServerId].lCore + serverList[retServerId].rCore - virSerType.core) / (double)(serverList[retServerId].core) &&
					// 	(double)(i.second.lRam + i.second.rRam - virSerType.ram) / (double)(i.second.ram) 
					// 		< (double)(serverList[retServerId].lRam + serverList[retServerId].rRam - virSerType.ram) / (double)(serverList[retServerId].ram) ){

					// 	retServerId = i.first;
					// }
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
			recordCreate(req.day , retServerId, -1);
		}
	}
	return retServerId;
}
