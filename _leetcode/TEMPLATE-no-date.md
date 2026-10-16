---
layout: page
title: "模板 · 两数之和"
published: false
permalink: /templates/leetcode/
num: 1
difficulty: easy
tags: [数组, 哈希表]
source: https://leetcode.cn/problems/two-sum/
---

> **这是模板文件，不会出现在列表页，也不该保留 `published: false` 和上面那个
> `permalink`。**
>
> 用法：复制一份改名为 `0001-two-sum.md`（题号补零到 4 位 + 英文 slug），
> 然后**删掉 `published: false` 和 `permalink` 这两行**，再改掉 `title` / `num` /
> `difficulty` / `tags` / `source`。
>
> 文件名不要带日期前缀，否则 URL 里会带上日期。
> 每一步都别漏了删那两行——留着 `published: false` 的话，你新写的题解不会出现在
> 列表页里，这是唯一一个需要手动处理的坑。

## 题目

给定一个整数数组 `nums` 和一个目标值 `target`，找出数组中和为 `target` 的两个数，返回它们的下标。

- 每种输入只会对应一个答案，同一个元素不能重复使用
- 可以按任意顺序返回答案

## 思路

**暴力解**：双重循环枚举所有数对，第一层固定 `i`，第二层找 `j > i` 是否满足 `nums[i] + nums[j] == target`。时间 O(n²)。

**正解（哈希表，一次遍历）**：核心是把「找另一个加数」这件事从**遍历**变成**查询**。
遍历到 `nums[i]` 时，我需要的另一个数是 `target - nums[i]`，那就直接问哈希表：
这个「补数」之前出现过吗？

- 出现过 → 它的下标和当前下标就是答案
- 没出现过 → 把当前值 `nums[i] -> i` 存进表里，继续往下走

关键点：**先查表，再把当前元素存表**。这个顺序天然保证了不会用同一个元素两次——
因为查表时当前元素还没进去。反过来（先存后查）在 `target = 2 * nums[i]` 时会把自己匹配上。

为什么可以边遍历边存、不用先建完整张表？因为答案的两个数是对称的：
只要它们同时存在于数组中，后出现的那个一定能在表里找到先出现的那个。

## 代码

```cpp
class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {
        unordered_map<int, int> seen;  // 值 -> 下标
        for (int i = 0; i < static_cast<int>(nums.size()); ++i) {
            auto it = seen.find(target - nums[i]);
            if (it != seen.end()) {
                return {it->second, i};  // 先出现的在返回结果的前面
            }
            seen[nums[i]] = i;
        }
        return {};  // 题目保证有解，这里只是兜底
    }
};
```

## 复杂度

- **时间 O(n)**：只遍历一次数组，每次循环里哈希表的 `find` 和插入平均 O(1)
- **空间 O(n)**：最坏情况（答案在最后两个位置）要存下前 n-2 个数

哈希表最坏情况是 O(n) 单次操作（大量哈希冲突），但均摊下来视为 O(1)，
所以总时间写作均摊 O(n)。

## 易错点 / 边界

- **顺序**：先 `find` 后插入。写反了会在 `[3,3], target=6` 这类用例上出错
- **重复元素**：`[3,3]` 是合法输入，两个 3 是不同的下标，不能去重
- **返回顺序**：题目不要求，但习惯上返回 `{小下标, 大下标}`
- **负数**：哈希表方案对负数天然正确，不用特殊处理（不需要排序，也就没有负数下标问题）
- **无解**：题目保证有解，但写工程代码时要处理返回值
- **不要用 `nums[i] + nums[j] == target` 判断溢出**：改成 `nums[j] == target - nums[i]` 更稳

## 延伸

- 如果数组已经**有序**：双指针 O(n) 时间、O(1) 空间，比哈希表更优
- 如果要返回**所有**数对而不只是第一对：不能用「边遍历边存」的提前返回，得改成先统计频次
