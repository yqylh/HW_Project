#include "input.cpp"

/*
    因为在实际题目中 一台服务器的编号是按照当日购买顺序依次分配的
    这和我在算需求时给的id不一样, 因为必须知道当天某种类型买了多少个
    所以在output做一个id的转换 , 在计算完一天每种需要购买的类型后 , 依次分配实际id
    分配方式就是在一个map里面存一下 , 在输出虚拟机的对应id的时候做一个转换即可
*/

int RealId = 0; // 实际服务器id
std::unordered_map<int, int> virtualId_RealId; // 虚拟id --- 实际id

void output() {
	for (int i = 1; i <= T; i++) {
        // 对于每一天所购买的服务器 , 进行记录数量 , 用map记录
		std::unordered_map<std::string , int> temp;
		for (auto & j : BuyList[i]) {
			if (temp.count(j) == 0) {
				temp[j] = 1;
			} else {
				temp[j]++;
			}
		}
		printf("(purchase, %d)\n", (int)temp.size());
        // 对map里记录的数据进行输出
		for (auto & j : temp) {
			std::cout << "(" << j.first << ", " << j.second << ")\n";
            // 查找当天买的服务器 && name是当前输出的名字(即保证实际id的顺序) 
            // 分配实际id
			for (auto & k : serverList) {
				if (k.second.createDay == i && k.second.name == j.first) {
					virtualId_RealId[k.second.id] = RealId++;
				}
			}
		}
		// 输出交换list
		printf("(migration, %d)\n" , (int)moveList[i].size());
		for (auto & j : moveList[i]) {
			if (j.where == -1) {
				printf("(%d, %d)\n" , j.virSerId, virtualId_RealId[j.serId]);
			} else {
				printf("(%d, %d, %c)\n" , j.virSerId, virtualId_RealId[j.serId], (j.where == 0 ? 'A' : 'B') );
			}
		}

        // 输出虚拟机的配置 , 需要根据map做一个实际服务器id的转换
		for (auto & j : CreateList[i]) {
			if (j.second == -1) {
				printf("(%d)\n" , virtualId_RealId[j.first]);
			} else {
				printf("(%d, %c)\n" , virtualId_RealId[j.first] , (j.second == 0 ? 'A' : 'B')  );
			}
		}
	}
}