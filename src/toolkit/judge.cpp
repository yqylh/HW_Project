//
// Created by Peter on 2021/4/9.
//

#include "OfflineJudger.hpp"

int main()
{
    std::pair<int, int> temp1, temp2;
    temp1 = judge("training-1.txt", "output1.txt");
    temp2 = judge("training-2.txt", "output2.txt");

    std::cout << "total cost: " << temp1.first + temp2.first << '\n';
    std::cout << "migration num: " << temp1.second + temp2.second << '\n';
    return 0;
}