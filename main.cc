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
#include <iostream>
#include <vector>

using namespace std;

int find_ret(int l, int r, vector<int>& nums, int flag) { // flag=1为求max
    int ret = nums[l];
    for (l += 1; l <= r; l++) {
        if (flag) {
            if (ret < nums[l]) {
                ret = nums[l];
            }
        } else {
            if (ret > nums[l]) {
                ret = nums[l];
            }
        }
    }
    return ret;
}
int longestSubarray(vector<int>& nums, int limit) {
    int ret = 0, left = 0, right = 0, min_ = nums[0], max_ = min_;
    int N = nums.size();
    for (; right < N; right++) {
        max_ = max(max_, nums[right]);
        min_ = min(min_, nums[right]);
        if (max_ - min_ <= limit) {
            ret = max(ret, right - left + 1);
        } else {
            left++;
            if (max_ == nums[left - 1] || min_ == nums[left - 1]) {
                max_ = find_ret(left, right, nums, 1);
                min_ = find_ret(left, right, nums, 0);
            }
        }
    }
    return ret;
}

int main() {
    vector<int> temp = { 9,10,1,7,9,3,9,9 };
    cout << longestSubarray(temp, 7);
    vector<int> temp1(2);
    
    return 0;
}
