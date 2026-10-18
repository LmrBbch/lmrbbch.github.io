---
layout: page
title: "两数之和"
num: 1
difficulty: easy
topics: [哈希表]
source: https://leetcode.cn/problems/two-sum/
---

## 题目

给定一个整数数组 `nums` 和一个目标值 `target`，找出数组中和为 `target` 的两个数，返回它们的下标。

- 每种输入只会对应一个答案，同一个元素不能重复使用
- 可以按任意顺序返回答案

## 用到的知识
-**pair** 返回一对数用{x，y}，这会返回一个pair


## 思路

**暴力解**：双重循环枚举所有数对，第一层固定 `i`，第二层找 `j > i` 是否满足 `nums[i] + nums[j] == target`。时间 O(n²)。

**正解（哈希表，一次遍历）**：核心是遍历到一个数时去哈希表中查询其补数，若哈希表中已存在其补数，则返回其下标和补数下标；若哈希表中不存在其补数，则将自身与自身下标存入哈希表中。

这样只需遍历一次数组即可完成任务，且每个数的查询与插入仅需要0(1)的复杂度。故在总体复杂度为O（n）

## 代码

```cpp
class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {
       std::unordered_map<int,int> m;
       for(int i=0;i<(int)nums.size();i++){
        int n=nums[i];
        if(m.find(target-n)!=m.end())
            return {i,m[target-n]};
        m.insert({n,i});
       }
       return {-1,-1};
    }
};
```

## 复杂度

- **时间 O(n)**：只遍历一次数组，每次循环里哈希表的 `find` 和插入平均 O(1)
- **空间 O(n)**：最坏情况（答案在最后两个位置）要存下前 n-2 个数

哈希表最坏情况是 O(n) 单次操作（大量哈希冲突），但均摊下来视为 O(1)，
所以总时间写作均摊 O(n)。

## 易错点 / 边界

-**unordered_map与map**unordered_map底层是哈希表，插入查询复杂度平均为0(1)，map底层为红黑树插入查询复杂度平均为O(log k)k为当前书中元素数
-**pair**返回pair和tuple均用{}即可
## 延伸

- 如果数组已经**有序**：双指针 O(n) 时间、O(1) 空间，比哈希表更优
- 如果要返回**所有**数对而不只是第一对：不能用「边遍历边存」的提前返回，得改成先统计频次
