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
#include "base/LogFile.h"

#include <iostream>
#include <vector>
#include <stack>
#include <numeric>

#include <string.h>

#include <unordered_map>

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

vector<int> findNumOfValidWords(vector<string>& words, vector<string>& puzzles) {
    unordered_map<int, int> frequency;

    for (const string& word : words) {
        int mask = 0;
        for (char ch : word) {
            mask |= (1 << (ch - 'a'));
        }
        if (__builtin_popcount(mask) <= 7) {
            ++frequency[mask];
        }
    }

    vector<int> ans;
    for (const string& puzzle : puzzles) {
        int total = 0;

        int mask = 0;
        for (int i = 0; i < 7; ++i) {
            mask |= (1 << (puzzle[i] - 'a'));
        }

        for (unordered_map<int, int>::iterator it = frequency.begin();
            it != frequency.end(); it++) {
            if ((1 << (puzzle[0] - 'a')) & (it->first)) {
                if ((mask | it->first) == mask) {
                    total += it->second;
                }
            }
        }

        ans.push_back(total);
    }
    return ans;
}

int dfs(string& s, int l, int r, int& k) {
    vector<int> count(26, 0);
    for (int i = l; i <= r; ++i) {
        count[s[i] - 'a']++;
    }

    char split = 0;
    for (int i = 0; i < 26; ++i) {
        if (count[i] > 0 && count[i] < k) {
            split = 'a' + i;
            break;
        }
    }
    if (split == 0) {
        return r - l + 1;
    }

    int ret = 0;
    while (l <= r) {
        while (l <= r && s[l] == split) ++l;
        if (l > r) {
            break;
        }

        int start = l;
        while (l <= r && s[l] != split) ++l;

        int length = dfs(s, start, l - 1, k);
        ret = max(ret, length);
    }
    return ret;
}

int longestSubstring(string s, int k) {
    int N = s.length();
    return dfs(s, 0, N - 1, k);
}

bool isMonotonic(vector<int>& A) {
    int flag = 0, N = A.size() - 1;
    for (int i = 0; i < N; ++i) {
        if (!flag && A[i] > A[i + 1]) {
            flag = 1;
        } else if (!flag && A[i] < A[i + 1]) {
            flag = 2;
        } else if (flag == 1 && A[i] < A[i + 1]) {
            return false;
        } else if (flag == 2 && A[i] > A[i + 1]) {
            return false;
        } else {
            continue;
        }
    }
    return true;
}

vector<vector<int>> sums;
void NumMatrix(vector<vector<int>>& matrix) {
    int n = matrix.size();
    if (n > 0) {
        int m = matrix[0].size();
        sums.resize(n + 1, vector<int>(m + 1));
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                sums[i + 1][j + 1] = sums[i][j + 1] + sums[i + 1][j] - sums[i][j] + matrix[i][j];
            }
        }
    }
}

int sumRegion(int row1, int col1, int row2, int col2) {
    return sums[row2 + 1][col2 + 1] - sums[row2 + 1][col1] - sums[row1][col2 + 1] + sums[row1][col1];
}

// int calculate(string s) {
//     stack<char> ops;
//     ops.push(1);
//     int N = s.length(), ret = 0, sign = 1;

//     int i = 0;
//     while (i < N) {
//         if (s[i] == ' ') {
//             ++i;
//         } else if (s[i] == '+') {
//             sign = ops.top();
//             ++i;
//         } else if (s[i] == '-') {
//             sign = -ops.top();
//             ++i;
//         } else if (s[i] == '(') {
//             ops.push(sign);
//             ++i;
//         } else if (s[i] == ')') {
//             ops.pop();
//             ++i;
//         } else {
//             long num = 0;
//             while (i < N && s[i] >= '0' && s[i] <= '9') {
//                 num = num * 10 + s[i] - '0';
//                 i++;
//             }
//             ret += sign * num;
//         }
//     }
//     return ret;
// }

int calculate(string s) {
    vector<int> stk;
    char preSign = '+';
    int num = 0;
    int n = s.length();
    for (int i = 0; i < n; ++i) {
        if (isdigit(s[i])) {
            num = num * 10 + int(s[i] - '0');
        }
        if (!isdigit(s[i]) && s[i] != ' ' || i == n - 1) {
            switch (preSign) {
            case '+':
                stk.push_back(num);
                break;
            case '-':
                stk.push_back(-num);
                break;
            case '*':
                stk.back() *= num;
                break;
            default:
                stk.back() /= num;
            }
            preSign = s[i];
            num = 0;
        }
    }
    return accumulate(stk.begin(), stk.end(), 0);
}

int main(int argc, char* argv[]) {
    cout << calculate("2*(1 + 1)") << endl;

    return 0;
}
