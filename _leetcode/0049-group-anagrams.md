---
layout: page
title: "字母异位词分组"
num: 49
topics: [哈希表]
difficulty: medium
source: https://leetcode.cn/problems/group-anagrams/
---

## 题目

给你一个字符串数组，请你将 **字母异位词** 组合在一起。可以按任意顺序返回结果列表。

**字母异位词** 是由重新排列源单词的所有字母得到的一个新单词。

- 示例 1：`["eat","tea","tan","ate","nat","bat"]` → `[["bat"],["nat","tan"],["ate","eat","tea"]]`
- 示例 2：`[""]` → `[[""]]`
- 示例 3：`["a"]` → `[["a"]]`

**约束**（这几条直接决定了后面哪些做法可行）：

- `1 <= strs.length <= 10^4`
- `0 <= strs[i].length <= 100` —— **注意长度可以为 0**，空串是合法输入
- `strs[i]` **仅包含小写字母** —— 这是"字符计数"解法能用定长 26 数组的前提

## 用到的知识

**`map[key]` 的隐式插入**：对 `map<K,V> m`，`m[key]` 在 `key` 不存在时会**自动插入** `{key, V()}` 并返回它的引用。

> 注意：`V` 必须**默认构造得出来**。`vector<string>` 可以，所以 `m[s].push_back(x)` 这种写法成立；
> 如果 `V` 是个没有默认构造函数的自定义类型，`operator[]` 直接编译不过，只能改用 `find` / `emplace`。

**`emplace_back` 和 `push_back`**：

> 修正一下原笔记的说法：`push_back` 并不要求"现成元素"，给它一个右值它也会移动构造。
> 两者真正的区别是：`push_back(T&&)` 参数类型写死为 `T`，传参时可能先构造一个临时 `T`；
> `emplace_back(Args&&...)` 把参数**原样透传到容器内部原地构造**，可以省掉临时对象，
> 也能调用 `T` 的 **explicit** 构造函数（`push_back` 的隐式转换调不了 explicit）。

**`sort` 与比较函数**：`sort(first, last, comp)` 对可随机访问范围原地排序，默认升序。

```cpp
// comp(a, b) 返回 true 表示 a 应该排在 b 前面
[](int x, int y) { return x > y; }   // true 时 x 在前 —— 降序
[](int x, int y) { return x < y; }   // 升序（等价于默认）
```

> 原笔记这里写成 `sort(x.first(), x.end(), ...)`，是笔误，应为 `x.begin()` / `x.begin(), x.end()`。
>
> **比较函数必须满足严格弱序**：只能用 `>`，写成 `>=` 时 `comp(a, a)` 返回 true，
> 排序区间不满足严格弱序是**未定义行为**（可能崩溃、可能死循环、可能结果错乱）。

**用「规范化签名」做哈希键** —— 这是本题的核心套路：

> 把每个字符串映射到一个**只保留本质信息**的签名，异位词映射到同一签名，再把签名当哈希表的键。
> 签名怎么选决定了复杂度：
>
> | 签名方式 | 时间 | 说明 |
> |---|---|---|
> | 排序后的字符串 | O(n·k·log k) | 最通用，什么字符集都行 |
> | 每个字母的出现次数 | O(n·k) | 需要字母表有限（本题是小写字母，26 个） |

## 我的做法

**我的思路**：把 `string` 里的所有 `char` 塞进一个 `multiset<char>`，用它区分是否为异位词；
再以 `multiset` 为键、结果数组的下标为值，即 `map<multiset<char>, int>`。

**我的代码**

```cpp
class Solution {
public:
    vector<vector<string>> groupAnagrams(vector<string>& strs) {
        vector<vector<string>> result;
        map<multiset<char>,int> m;
        for(auto s:strs){
            multiset<char> ms;
            for(auto c:s){
                ms.insert(c);
            }
            auto it=m.find(ms);
            if(it!=m.end()){
                result[it->second].push_back(s);
            }
            else{
                int index=result.size();
                m.insert({ms,index});
                result.push_back(vector<string>{s});
            }
        }
        return result;
    }
};
```

**这个写法为什么是对的**：`multiset<char>` 的 `operator<` 是逐元素字典序比较，两个异位词排出来的
有序序列完全相同，所以签名相同；不是异位词则序列长度或内容不同，`find` 自然匹配不上。

> 这个思路很扎实：**用「有序的多重集」代替「排序后的字符串」，抽象层次更高**，
> 而且顺带支持了任意字符集（不依赖"只有 26 个小写字母"这个约束）。

**可以改进的三个点**（不影响正确性）：

```cpp
// 1. for(auto s : strs) 每次都拷贝一份字符串。k 最大 100、n 最大 10^4，
//    最坏情况白拷贝 10^6 个字符。读的时候用 const 引用：
for (const auto& s : strs) {

// 2. m.insert({ms, index}) 里的 {ms, index} 会构造一个 pair 临时对象并拷贝 multiset。
//    直接放临时值更省：
m.emplace(ms, index);

// 3. 空字符串也要能正确分组。ms 为空 multiset 时 find 依然能匹配上，
//    所以原代码对 "" 是正确的（已验证），这一点不用担心。
```

## 题解做法

**题解思路**：所有异位词排序后会得到同样的 `sorted_str`，用它当键，
值是"装着所有异位词的 `vector<string>`"，即 `unordered_map<string, vector<string>>`。

**题解代码**

```cpp
class Solution {
public:
    vector<vector<string>> groupAnagrams(vector<string>& strs) {
        vector<vector<string>> result;
        unordered_map<string,vector<string>> m;
        for(auto s:strs){
            string sorted_s=s;
            sort(sorted_s.begin(),sorted_s.end(),[](char x,char y){return x>y;});
            m[sorted_s].emplace_back(s);
        }
        for(auto e:m){
            result.emplace_back(e.second);
        }
        return result;
    }
};
```

> 三点说明：
>
> 1. **降序排序没有错，但没必要**。分组只要求"同一个异位词组映射到同一个键"，
>    升序降序都满足。写成默认升序（省掉 comparator）更能表达意图，也不容易让人误以为顺序有含义。
> 2. `for (auto e : m)` 会把每个 `vector<string>` 整份拷贝一次，`result.emplace_back(e.second)`
>    又拷贝一次。改成 `for (auto& e : m) result.emplace_back(std::move(e.second));` 可以省掉两轮拷贝。
> 3. `for (auto s : strs)` 同样多拷贝了一次——本来 `string sorted_s = s;` 已经要拷贝了，
>    直接用 `string sorted_s = s;` 排序即可，循环变量用 `const auto&`。

## 更优解：字符计数（O(n·k)）

既然题目保证只有 26 个小写字母，就不用排序了——**数每个字母出现几次**就是签名：

```cpp
class Solution {
public:
    vector<vector<string>> groupAnagrams(vector<string>& strs) {
        unordered_map<string, vector<string>> m;
        for (const auto& s : strs) {
            array<int, 26> cnt{};
            for (char c : s) ++cnt[c - 'a'];

            // 把 26 个计数拼成 string 当键，键长固定 26 字节。
            // 也可以直接写 string(cnt.begin(), cnt.end())（实测可用），
            // 但那样键里会出现 \0 字节，调试时看不见，所以这里显式转换。
            string key;
            key.reserve(26);
            for (int i = 0; i < 26; ++i) key.push_back(static_cast<char>(cnt[i]));
            m[key].push_back(s);
        }
        vector<vector<string>> res;
        res.reserve(m.size());
        for (auto& kv : m) res.push_back(std::move(kv.second));
        return res;
    }
};
```

**时间 O(n·k)**（k 为字符串长度，n 为字符串个数）：每个字符只处理一次，建键是固定的 26 次操作，
哈希操作均摊 O(1)。**空间 O(n·k)**，键里存了每组的计数。

> 这个优化的前提是「字符集有限且很小」。如果题目改成"含大小写和数字"（62 个）或 Unicode，
> 数组长度要跟着变；如果字符集不定，就只能回到排序法。

## 复杂度

按三种做法分别看（n = 字符串个数，k = 最长字符串长度，m = 分出的组数）：

| 做法 | 时间 | 空间 |
|---|---|---|
| 我的做法（`map` + `multiset` 键） | O(n·k·log m) | O(n·k) |
| 题解做法（`unordered_map` + 排序键） | O(n·k·log k) | O(n·k) |
| 字符计数（`unordered_map` + 计数键） | O(n·k) | O(n·k) |

几个容易写错的地方：

- **我的做法为什么是 O(n·k·log m)**：每插入/查找一次要 O(log m) 次键比较，
  而 `multiset` 的比较不是 O(1) 而是 **O(k)**（要逐元素比），所以单次操作是 O(k·log m)。
  ⚠️ 注意这**不一定是更快**：k 大而 m 小时（比如一堆 100 字符的串但只有几个组），
  log m 很小但 k 很大会被放大；k 小而 m 大时又不如排序法。两者是不同 trade-off，不是简单的一优一劣。
- **空间为什么是 O(n·k) 而不是 O(n)**：哈希表里每个键都完整存了一份签名，
  最坏情况（所有串互不相同）键的总长度就是所有字符总数 O(n·k)。
  原笔记写的 "空间 O(n)：需要先存下所有字符串" 不准确——`strs` 是入参不占额外空间，
  额外空间主要是这些键。
- **结果本身也算空间**：返回值里装的是所有字符串的拷贝，这部分 O(n·k) 是题目要求的输出，
  不是你的算法开销，但面试时说清"额外空间"和"总空间"的区别会加分。

## 易错点 / 边界

**算法层面**

- **长度不同的字符串不可能是异位词**。长度不同时签名天然不同，不用特判——但如果加了特判，
  记得别把空串和空串判成不同。
- **空字符串必须能正确分组**。`strs = [""]` 要返回 `[[""]]`；`strs = ["",""]` 要返回 `[["",""]]`。
  用 `multiset` 或排序法都天然正确；用计数法时，空串得到全 0 的计数，键是全 `\0` 字符串，
  也能正确归到一组。**但空串这一组和"字符恰好全是 `\0` 的串"会撞键**——本题只有小写字母，不会出现。
- **`strs` 至少有一个元素**（`1 <= strs.length`），不用处理空数组；但工程代码里要处理，返回空 `vector`。
- **不要修改输入**。`sort` 要作用在副本上（`string sorted_s = s;`），
  直接 `sort(s.begin(), s.end())` 就把入参改了——虽然力扣不检查，但这是明确的坏习惯。
- **结果里每组内部的顺序、组之间的顺序都任意**，不用排序。

**C++ 层面（这几条最容易在面试里踩）**

- **`operator[]` 要求 `V` 默认构造得出来**。`m[s].push_back(x)` 能编译是因为 `vector<string>` 可默认构造。
  换成 `m[s].emplace_back(x)` 也一样。但如果你把 `V` 换成没有默认构造函数的类型，这行直接编译失败。
- **`for (auto e : m)` 是拷贝**。遍历 map 时想修改或避免拷贝，用 `auto&`；只读用 `const auto&`。
- **`sort` 的比较函数必须严格弱序**。`return x >= y;` 是未定义行为（`comp(a,a)` 返回 true）。
  正确写法是用 `>` 或 `<`。
- **`string(cnt.begin(), cnt.end())` 能用，但要知道它做了什么**。这个区间构造函数会把
  `int` 逐个**窄化**成 `char` 存进字符串，所以键里会出现值为 `\0`（计数 0）的字节。
  因为 `std::string` 比较是**认长度**的，带 `\0` 不影响分组，实测也能正常分组。
  真正的风险有两个：① 计数超过 255 时会被截断（本题每条串最长 100，不会发生，但换个约束就危险）；
  ② 任何依赖 C 字符串语义的处理——`strcmp`、`printf("%s")`、把 `key.c_str()` 传给按 `\0` 截断的 API
  ——都会把第一个 `\0` 当结尾，导致误判。要打印调试就用 `key.size()` 显式控制长度，
  或者干脆把计数映射成可读字符（比如 `cnt[i] + 'a'`）。
- **`unordered_map` 的键要用可哈希的类型**。`string`、`int` 没问题；`multiset`、`vector` 没有默认哈希，
  所以用 `multiset` 当键时必须配 `map`（红黑树，只要求可比较），不能用 `unordered_map`。
- **`m.insert({ms, index})` 会拷贝 `ms`**，用 `m.emplace(ms, index)` 或 `m[ms] = index` 语义更直接。

## 延伸

- **换签名就是另一道题**。本题的本质是「规范化签名 + 哈希分组」，同一套模板可以解：
  - [242. 有效的字母异位词](https://leetcode.cn/problems/valid-anagram/)：两个串签名是否相同
  - [438. 找到字符串中所有字母异位词](https://leetcode.cn/problems/find-all-anagrams-in-a-string/)：
    定长滑动窗口 + 计数签名，窗口每滑一次只更新两个计数
  - [249. 移位字符串分组](https://leetcode.cn/problems/group-shifted-strings/)（力扣会员题）：
    签名换成「相邻字符差值的序列」
- **如果要求组内按字典序、组间按首元素排序**：在返回前加一次排序即可，代价 O(m·log m + n·k·log n)。
- **如果数据规模涨到 10^6**：哈希表键的字符串会占大量内存。可以让计数数组直接做键的一部分
  （用 `array<int,26>` 作键，配自定义 hash），或者只存一个 26 字节的紧凑编码。
- **面试追问方向**：
  - 「不用哈希表能做吗？」——可以排序整个数组（按签名排序）后再线性分组，但签名本身还是要算。
  - 「哈希冲突怎么办？」——本题用的是精确比较（`map` 是树比较，`unordered_map` 冲突时逐字节比键），
    签名相同就是真的异位词，不存在"误判"。真正要担心的是把计数数组转成字符串时的窄化陷阱（见上）。
- **相关题目**：[349. 两个数组的交集](https://leetcode.cn/problems/intersection-of-two-arrays/)（哈希集合）、
  [1. 两数之和](/leetcode/0001-two-sum/)（哈希表查补数）。
