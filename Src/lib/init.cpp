#ifndef __init__
#define __init__
#include "input.cpp"

/* 初始化 , 把购买的List和创建的List清空 */
void init() {
    for (int i = 0; i < T + 5; i++) BuyList.push_back(std::vector<std::string> (0) );
    for (int i = 0; i < T + 5; i++) CreateList.push_back(std::vector< CreateType >(0) );
    for (int i = 0; i < T + 5; i++) moveList.push_back(std::vector<moveMark>(0) );
    for (int i = 0; i < T + 5; i++) serverRequest.push_back(std::make_pair(0,0) );

    auto cmp = [&](request &i, request &j) {
        return virtualServerType[i.name].core + virtualServerType[i.name].ram > virtualServerType[j.name].core + virtualServerType[j.name].ram;
    };
    for (auto i = requestList.begin(); i != requestList.end();) {
        auto j = i;
        int order = 0;
        for (; j != requestList.end(); j++) {
            j->order = order++;
            if (j->day != i->day) break;
        }
        i = j;
    }
    for (auto i = requestList.begin(); i != requestList.end();) {
        if (i->type == 1) {
            i++;
            continue;
        }
        auto j = i;
        for (; j != requestList.end(); j++) {
            if (j->day != i->day) break;
            if (j->type == 1) break;
        }
        std::sort(i , j , cmp);
        i = j;
    }

}

// 记录购买一台服务器
void recordBuy(int _day, std::string _name) {
    BuyList[_day].push_back(_name);
}
// 记录创建一台虚拟机
void recordCreate(int _day , int _serverId , int _type, int order) {
    CreateList[_day].push_back(CreateType(_serverId , _type, order));
}
#endif