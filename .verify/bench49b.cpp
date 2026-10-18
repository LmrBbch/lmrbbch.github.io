#include <algorithm>
#include <array>
#include <chrono>
#include <cstdio>
#include <map>
#include <random>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

static auto now() { return chrono::steady_clock::now(); }
static double ms_since(chrono::steady_clock::time_point t) {
    return chrono::duration<double, milli>(now() - t).count();
}

// A0: 原版（照抄，唯一改动是去掉 new vector 的写法以便对照）
struct A0 {
    static const char* name() { return "A0 原版: for(auto s:) + m.insert(pair)"; }
    vector<vector<string>> run(vector<string>& strs) {
        vector<vector<string>> result;
        map<multiset<char>, int> m;
        for (auto s : strs) {
            multiset<char> ms;
            for (auto c : s) ms.insert(c);
            auto it = m.find(ms);
            if (it != m.end()) result[it->second].push_back(s);
            else { int idx = result.size(); m.insert({ms, idx}); result.push_back(vector<string>{s}); }
        }
        return result;
    }
};

// A1: 只去掉两处字符串拷贝（仍然 multiset 建键 + map）
struct A1 {
    static const char* name() { return "A1 去掉拷贝: for(const auto&:) + emplace"; }
    vector<vector<string>> run(vector<string>& strs) {
        vector<vector<string>> result;
        map<multiset<char>, int> m;
        result.reserve(strs.size());
        for (const auto& s : strs) {
            multiset<char> ms(s.begin(), s.end());
            auto it = m.find(ms);
            if (it != m.end()) result[it->second].push_back(s);
            else { int idx = int(result.size()); m.emplace(ms, idx); result.push_back(vector<string>{s}); }
        }
        return result;
    }
};

// A2: 彻底换成排序键 + map（测 map 与 unordered_map 的差别，以及 multiset 的常数）
struct A2 {
    static const char* name() { return "A2 排序键 + map（仍是红黑树）"; }
    vector<vector<string>> run(vector<string>& strs) {
        map<string, int> m;
        vector<vector<string>> result;
        result.reserve(strs.size());
        for (const auto& s : strs) {
            string t = s;
            sort(t.begin(), t.end());
            auto it = m.find(t);
            if (it != m.end()) result[it->second].push_back(s);
            else { int idx = int(result.size()); m.emplace(std::move(t), idx); result.push_back(vector<string>{s}); }
        }
        return result;
    }
};

// B: 题解
struct B {
    static const char* name() { return "B  题解: unordered_map<string,vector> + 排序键"; }
    vector<vector<string>> run(vector<string>& strs) {
        vector<vector<string>> result;
        unordered_map<string, vector<string>> m;
        for (auto s : strs) {
            string t = s;
            sort(t.begin(), t.end(), [](char x, char y) { return x > y; });
            m[t].emplace_back(s);
        }
        for (auto e : m) result.emplace_back(e.second);
        return result;
    }
};

// C: 计数
struct C {
    static const char* name() { return "C  计数: unordered_map<array<int,26>>"; }
    struct H {
        size_t operator()(const array<int,26>& a) const {
            size_t h = 0;
            for (int v : a) h = h * 131 + (size_t)v;
            return h;
        }
    };
    vector<vector<string>> run(vector<string>& strs) {
        unordered_map<array<int,26>, vector<string>, H> m;
        for (const auto& s : strs) {
            array<int,26> cnt{};
            for (char c : s) ++cnt[c - 'a'];
            m[cnt].push_back(s);
        }
        vector<vector<string>> res;
        res.reserve(m.size());
        for (auto& kv : m) res.push_back(std::move(kv.second));
        return res;
    }
};

template <class Sol>
double bench(vector<string> data, int reps, bool& ok) {
    Sol s;
    vector<vector<string>> first = s.run(data);
    for (auto& v : first) sort(v.begin(), v.end());
    sort(first.begin(), first.end());
    ok = true;
    auto t0 = now();
    for (int r = 0; r < reps; ++r) {
        auto g = s.run(data);
        for (auto& v : g) sort(v.begin(), v.end());
        sort(g.begin(), g.end());
        if (g != first) ok = false;
    }
    return ms_since(t0) / reps;
}

int main() {
    mt19937 rng(20261018);
    vector<string> distinct;
    for (int i = 0; i < 10000; ++i) {
        string s(100, 'a');
        for (char& c : s) c = char('a' + rng() % 26);
        distinct.push_back(s);
    }
    vector<string> same(10000, string(100, 'a'));

    struct Item { const char* name; vector<string>* data; };
    for (auto* data : {&distinct, &same}) {
        printf("\n=== %s ===\n", data == &distinct ? "1万互不相同 k=100 (m=n)" : "1万全相同 k=100 (m=1)");
        bool a0, a1, a2, b, c;
        double t0 = bench<A0>(*data, 5, a0);
        double t1 = bench<A1>(*data, 5, a1);
        double t2 = bench<A2>(*data, 5, a2);
        double tb = bench<B>(*data, 5, b);
        double tc = bench<C>(*data, 5, c);
        printf("  %-46s %8.3f ms\n", A0::name(), t0);
        printf("  %-46s %8.3f ms  (%.1f%% of A0)\n", A1::name(), t1, t1 / t0 * 100);
        printf("  %-46s %8.3f ms  (%.1f%% of A0)\n", A2::name(), t2, t2 / t0 * 100);
        printf("  %-46s %8.3f ms\n", B::name(), tb);
        printf("  %-46s %8.3f ms\n", C::name(), tc);
    }
    return 0;
}
