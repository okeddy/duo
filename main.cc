/*
* Copyright (c) 2021, kd.
* All rights reserved.
*
* 文件名称：filename.h
* 摘 要：
*
* 当前版本：1.0
* 作 者：kd
* 完成日期：2021年2月19日
*/

#include <iostream>
#include <vector>

using namespace std;

int longestOnes(vector<int>& A, int K) {
    int left = 0, zeros = 0, ret = 0;
    int n = A.size();
    for (int right = 0; right < n; right++) {
        if (A[right] == 0) ++zeros;
        while (zeros > K) {
            if (A[left++] == 0) --zeros;
        }
        ret = max(ret, right - left + 1);
    }
    return ret;
}

int main()
{
    vector<int> arr = { 1,1,1,0,0,0,1,1,1,1,0 };
    cout << longestOnes(arr, 2);
}
