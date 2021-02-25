/*
* Copyright (c) 2021, kd.
* All rights reserved.
*
* 文件名称：main.cc
* 摘 要：
*
* 当前版本：1.0
* 作 者：kd
* 完成日期：2021年2月19日
*/

#include "base/Logging.h"

#include <iostream>
#include <vector>

#include <string.h>

using namespace std;

int maxSatisfied(vector<int>& customers, vector<int>& grumpy, int X) {
    int total = 0, N = customers.size();
    for (int i = 0; i < N; i++) {
        if (!grumpy[i]) {
            total += customers[i];
        }
    }

    int satisfied = 0, max_satisfied = 0;
    for (int i = 0; i < X; i++) {
        satisfied += customers[i] * grumpy[i];
    }

    max_satisfied = satisfied;
    for (int i = X; i < N; i++) {
        satisfied = satisfied - customers[i - X] * grumpy[i - X] + customers[i] * grumpy[i];
        max_satisfied = max(satisfied, max_satisfied);
    }
    
    return total + max_satisfied;
}

int main() {
    vector<int> temp1 = { 1,0,1,2,1,1,7,5 };
    vector<int> temp2 = { 0,1,0,1,0,1,0,1 };
    cout << maxSatisfied(temp1, temp2, 3);
    
    return 0;
}
