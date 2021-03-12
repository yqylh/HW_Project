#ifndef __input__
#define __input__
#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <sstream>
#include "server.cpp"
#include "virtualServer.cpp"
#include "request.cpp"
#include "moveMark.cpp"

int N; // 服务器类型数量
std::unordered_map<std::string, server> serverType; // 服务器类型数组
std::unordered_map<int, server> serverList; // 服务器数组
int M; // 虚拟机类型数
std::unordered_map<std::string, virtualServer> virtualServerType; // 虚拟服务器类型数组
std::unordered_map<int, virtualServer> virtualServerList; // 虚拟服务器数组
int T; // T天的用户数据
std::vector<request> requestList; // 请求列表
int serverId = 0; // 服务器id
std::vector<std::vector<std::string> > BuyList; // T天 每天一个List 存需要买的服务器的名字
std::vector<std::vector<std::pair<int, int> > > CreateList; // T天 每天一个List 存的是虚拟机实际存储的位置

// T天 每天一个List 存的是当天虚拟机迁移的序列 
// 第一个参数代表虚拟机id, 第二个参数代表目标服务器id 第三个参数代表0左节点还是1右节点 , 如果是双节点虚拟机则是-1;
std::vector<std::vector< moveMark > > moveList; 


void solveServerInput() {
    std::string s = "";
    while (s == "") std::getline(std::cin, s);
    s.erase(s.begin());
    *s.rbegin() = ',';

    std::vector<std::string> inputarr;
    std::string input;
    for (auto & i : s) {
        if (i == ',') {
            inputarr.push_back(input);
            input.clear();
        } else input += i;
    }

    auto x = server();
    x.name = inputarr[0];
    x.core = std::stoi(inputarr[1]);
    x.ram  = std::stoi(inputarr[2]);
    x.cost = std::stoi(inputarr[3]);
    x.dayCost = std::stoi(inputarr[4]);
    serverType[x.name] = x;
}
void solvevirtualServerInput() {
    std::string s = "";
    while (s == "") std::getline(std::cin, s);
    s.erase(s.begin());
    *s.rbegin() = ',';

    std::vector<std::string> inputarr;
    std::string input;
    for (auto & i : s) {
        if (i == ',') {
            inputarr.push_back(input);
            input.clear();
        } else input += i;
    }
    auto x = virtualServer();
    x.name = inputarr[0];
    x.core = std::stoi(inputarr[1]);
    x.ram  = std::stoi(inputarr[2]);
    x.isDouble = std::stoi(inputarr[3]);
    virtualServerType[x.name] = x;
}
void solveRequestInput(int day) {
    std::string s = "";
    while (s == "") std::getline(std::cin, s);
    s.erase(s.begin());
    *s.rbegin() = ',';

    std::vector<std::string> inputarr;
    std::string input;
    for (auto & i : s) {
        if (i == ',') {
            inputarr.push_back(input);
            input.clear();
        } else input += i;
    }

    requestList.push_back(request());
    auto &x = requestList.back();
    x.day = day;

    if (inputarr[0] == "add") {
        x.type = 0;
        x.name = inputarr[1];
        x.name.erase(x.name.begin()); // 辣鸡空格
        x.id = std::stoi(inputarr[2]);
    } else { // delete
        x.type = 1;
        x.id = std::stoi(inputarr[1]);
    }
}
void input() {
    std::cin >> N;
    for (int i = 1; i <= N; ++i) {
        solveServerInput();
    }
    std::cin >> M;
    for (int i = 1; i <= M; ++i) {
        solvevirtualServerInput();
    }
    std::cin >> T;
    for (int i = 1; i <= T; ++i) {
        int R; //
        std::cin >> R;
        for (int j = 1; j <= R; ++j) {
            solveRequestInput(i);
        }
    }
}

#endif