#ifndef __output__
#define __output__
#include "input.cpp"

/*
    因为在实际题目中 一台服务器的编号是按照当日购买顺序依次分配的
    这和我在算需求时给的id不一样, 因为必须知道当天某种类型买了多少个
    所以在output做一个id的转换 , 在计算完一天每种需要购买的类型后 , 依次分配实际id
    分配方式就是在一个map里面存一下 , 在输出虚拟机的对应id的时候做一个转换即可
*/

int RealId = 0; // 实际服务器id
std::unordered_map<int, int> virtualId_RealId; // 虚拟id --- 实际id

void output(int day) {
    // 对于每一天所购买的服务器 , 进行记录数量 , 用map记录
    std::unordered_map<std::string , int> temp;
    for (auto & i : BuyList[day]) {
        if (temp.count(i) == 0) {
            temp[i] = 1;
        } else {
            temp[i]++;
        }
    }

    printf("(purchase, %d)\n", (int)temp.size());
    // std::cout << "(purchase, " << temp.size() << ")" << std::endl;
    // 对map里记录的数据进行输出
    for (auto & i : temp) {
        std::cout << "(" << i.first << ", " << i.second << ")\n";
        // 查找当天买的服务器 && name是当前输出的名字(即保证实际id的顺序) 
        // 分配实际id
        for (auto & k : serverList) {
            if (k.second.createDay == day && k.second.name == i.first) {
                virtualId_RealId[k.second.id] = RealId++;
            }
        }
    }
    // 输出交换list
    printf("(migration, %d)\n" , (int)moveList[day].size());
    // std::cout << "(migration, "<< moveList[day].size() << ")" << std::endl;
    for (auto & i : moveList[day]) {
        if (i.where == -1) {
            printf("(%d, %d)\n" , i.virSerId, virtualId_RealId[i.serId]);
            // std::cout << "(" << i.virSerId << ", " << virtualId_RealId[i.serId] <<")" << std::endl;
        } else {
            printf("(%d, %d, %c)\n" , i.virSerId, virtualId_RealId[i.serId], (i.where == 0 ? 'A' : 'B') );
            // std::cout << "("<<i.virSerId<<","<< virtualId_RealId[i.serId]<<", "<<(i.where == 0 ? 'A' : 'B')<<")" << std::endl;
        }
    }

    // 输出虚拟机的配置 , 需要根据map做一个实际服务器id的转换
    std::sort(CreateList[day].begin(), CreateList[day].end());
    for (auto & i : CreateList[day]) {
        if (i._type == -1) {
            printf("(%d)\n" , virtualId_RealId[i._serverId]);
            // std::cout << "("<<virtualId_RealId[i._serverId]<<")" << std::endl;
        } else {
            printf("(%d, %c)\n" , virtualId_RealId[i._serverId] , (i._type == 0 ? 'A' : 'B')  );
            // std::cout << "(" << virtualId_RealId[i._serverId] << ", " << (i._type == 0 ? 'A' : 'B') << ")" << std::endl;
        }
    }
    std::fflush(stdout);
}
#endif