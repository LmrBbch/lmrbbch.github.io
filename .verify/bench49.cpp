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

// ---------- A: 用户做法 ----------
struct A {
    static const char* name() { return "A  用户做法 map<multiset<char>,int>"; }
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

// ---------- B: 题解做法 ----------
struct B {
    static const char* name() { return "B  题解 unordered_map<string,vector> 排序键"; }
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

// ---------- C: 计数法 ----------
struct C {
    static const char* name() { return "C  计数法 unordered_map<array<int,26>>"; }
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

// ---------- 校验 ----------
vector<vector<string>> canon(vector<vector<string>> g) {
    for (auto& v : g) sort(v.begin(), v.end());
    sort(g.begin(), g.end());
    return g;
}

template <class Sol>
double bench(vector<string> data, int reps, bool& ok) {
    Sol s;
    vector<vector<string>> first = s.run(data);
    ok = true;
    for (auto& v : first) sort(v.begin(), v.end());
    sort(first.begin(), first.end());
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
    struct Case { const char* name; vector<string> data; };
    vector<Case> cases;

    {   // 1) 全部互不相同，k=100  —— m = n 最大，键比较最贵
        vector<string> d;
        for (int i = 0; i < 10000; ++i) {
            string s(100, 'a');
            for (char& c : s) c = char('a' + rng() % 26);
            d.push_back(s);
        }
        cases.push_back({"1万互不相同 k=100 (m=n)", d});
    }
    {   // 2) 全部相同  —— m = 1，树的 log 项最小
        cases.push_back({"1万全相同 k=100 (m=1)", vector<string>(10000, string(100, 'a'))});
    }
    {   // 3) 100 个组，每组 100 个异位词 —— 中间态
        vector<string> d;
        for (int g = 0; g < 100; ++g) {
            string base(100, 'a');
            for (char& c : base) c = char('a' + rng() % 26);
            for (int j = 0; j < 100; ++j) {
                string s = base;
                shuffle(s.begin(), s.end(), rng);
                d.push_back(s);
            }
        }
        cases.push_back({"1万条/100组 k=100 (m=100)", d});
    }
    {   // 4) 全都长度 1  —— k=1，log k = 0
        vector<string> d;
        for (int i = 0; i < 10000; ++i) d.push_back(string(1, char('a' + rng() % 26)));
        cases.push_back({"1万条 k=1 (m<=26)", d});
    }

    for (auto& c : cases) {
        printf("\n=== %s ===\n", c.name);
        bool okA, okB, okC;
        double ta = bench<A>(c.data, 5, okA);
        double tb = bench<B>(c.data, 5, okB);
        double tc = bench<C>(c.data, 5, okC);
        printf("  A 用户做法      %8.3f ms  %s\n", ta, okA ? "" : "(结果不一致!)");
        printf("  B 题解排序法    %8.3f ms  %s\n", tb, okB ? "" : "(结果不一致!)");
        printf("  C 计数法        %8.3f ms  %s\n", tc, okC ? "" : "(结果不一致!)");
        printf("  A/B = %.2fx   A/C = %.2fx\n", ta / tb, ta / tc);
    }
    return 0;
}
