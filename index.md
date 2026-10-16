---
layout: page
title: 首页
---

这里放两类长期维护的笔记。

## 📘 [刷题笔记](/leetcode/)

LeetCode 题解，按难度分组、按题号排序。

已收录 **{{ site.leetcode | where_exp: "item", "item.published != false" | size }}** 题。

## 🛠 [项目技术细节](/projects/)

项目实现过程中的设计取舍、关键模块与踩坑记录，一个部分一篇。

{%- assign live = site.projects | where_exp: "item", "item.published != false" %}
已收录 **{{ live | map: "project" | uniq | size }}** 个项目、**{{ live | size }}** 篇文章。
