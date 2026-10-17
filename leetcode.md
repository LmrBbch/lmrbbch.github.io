---
layout: page
title: 刷题笔记
permalink: /leetcode/
# 分类顺序。改这个列表就能调整页面上各算法标签的先后顺序；
# 没写在这里的标签会自动排在后面（按字母序），所以新增标签不用急着来改这里。
topic_order: [哈希表, 双指针, 滑动窗口, 二分查找, 字符串, 链表, 栈, 队列, 树, 图, 回溯, 动态规划, 贪心, 堆, 位运算, 数学, 数组]
---

按算法标签分组，组内按题号排序。每道题一个文件，放在 `_leetcode/` 目录下。

{%- assign live = site.leetcode | where_exp: "item", "item.published != false" %}
{%- assign total = live | size %}

{%- comment %}
  把每道题的 topics 摊平成一个池子，再剔除空值、去重。
  Liquid 没有 flatten，所以用嵌套 for + push 手工展开。
  "" | split: "," 会留下一个空字符串元素，而 Liquid 里空字符串是「真值」，
  所以 where_exp 必须写成显式的 t != ""，只写 "t" 是过滤不掉的。
  标签是自动发现的：新增标签会自己出现，不用维护清单。
{%- endcomment %}
{%- assign pool = "" | split: "," %}
{%- for doc in live %}
{%- for t in doc.topics %}
{%- if t != blank %}
{%- assign pool = pool | push: t %}
{%- endif %}
{%- endfor %}
{%- endfor %}
{%- assign found = pool | uniq %}

{%- comment %}
  最终顺序 = topic_order 里写了的（按写的顺序）+ 没写到的（字母序）。
{%- endcomment %}
{%- assign ordered = "" | split: "," %}
{%- for t in page.topic_order %}
{%- if found contains t %}
{%- assign ordered = ordered | push: t %}
{%- endif %}
{%- endfor %}
{%- assign rest = found | sort %}
{%- for t in rest %}
{%- unless ordered contains t %}
{%- assign ordered = ordered | push: t %}
{%- endunless %}
{%- endfor %}
{%- assign ordered = ordered | where_exp: "x", "x != blank" %}

{%- assign unclassified = 0 %}
{%- for doc in live %}
{%- unless doc.topics %}
{%- assign unclassified = unclassified | plus: 1 %}
{%- endunless %}
{%- endfor %}

共收录 **{{ total }}** 题，分 **{{ ordered | size }}** 个标签。
{% for t in ordered %}
<a href="#{{ t | url_encode }}"><code>{{ t }}</code></a>
{%- endfor %}
{%- if unclassified > 0 %}
<a href="#unclassified"><code>未分类</code></a>
{%- endif %}

{% for t in ordered %}
{%- assign list = live | where_exp: "d", "d.topics contains t" | sort: "num" %}

## {{ t }}（{{ list | size }}）

| 题目 | 难度 | 全部标签 |
| :--- | :---: | :--- |
{% for p in list -%}
{% capture badge %}{% case p.difficulty %}{% when "easy" %}<span class="badge badge-easy">简单</span>{% when "medium" %}<span class="badge badge-medium">中等</span>{% when "hard" %}<span class="badge badge-hard">困难</span>{% else %}<span class="badge">—</span>{% endcase %}{% endcapture -%}
| [{{ p.num }}. {{ p.title }}]({{ p.url | relative_url }}) | {{ badge | strip }} | <span class="tag-list">{{ p.topics | join: " / " }}</span> |
{% endfor -%}
{%- endfor %}
{%- if unclassified > 0 %}

## 未分类（{{ unclassified }}）

这些题没有写 `topics`，不会出现在上面的分组里。

| 题目 | 难度 |
| :--- | :---: |
{% assign untagged = live | where_exp: "d", "d.topics == nil or d.topics.size == 0" | sort: "num" -%}
{% for p in untagged -%}
{% capture badge %}{% case p.difficulty %}{% when "easy" %}<span class="badge badge-easy">简单</span>{% when "medium" %}<span class="badge badge-medium">中等</span>{% when "hard" %}<span class="badge badge-hard">困难</span>{% else %}<span class="badge">—</span>{% endcase %}{% endcapture -%}
| [{{ p.num }}. {{ p.title }}]({{ p.url | relative_url }}) | {{ badge | strip }} |
{% endfor -%}
{%- endif %}
