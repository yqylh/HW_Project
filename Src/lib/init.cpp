#ifndef __init__
#define __init__
#include "input.cpp"

/* 初始化 , 把购买的List和创建的List清空 */
void init() {
    for (int i = 0; i < T + 2; i++) BuyList.push_back(std::vector<std::string> (0) );
    for (int i = 0; i < T + 2; i++) CreateList.push_back(std::vector< CreateType >(0) );
    for (int i = 0; i < T + 2; i++) moveList.push_back(std::vector<moveMark>(0) );
    for (int i = 0; i < T + 2; i++) serverRequest.push_back(std::make_pair(0,0) );
    for (int i = 0; i < T + 2; i++) requestList.push_back(std::vector<request> (0));
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