---
layout: page
title: 刷题笔记
permalink: /leetcode/
---

按难度分组，组内按题号排序。每道题一个文件，放在 `_leetcode/` 目录下。

{%- assign easy = site.leetcode | where: "difficulty", "easy" | sort: "num" %}
{%- assign medium = site.leetcode | where: "difficulty", "medium" | sort: "num" %}
{%- assign hard = site.leetcode | where: "difficulty", "hard" | sort: "num" %}
{%- assign groups = "easy,medium,hard" | split: "," %}

共收录 **{{ site.leetcode | size }}** 题。
{% for key in groups %}
{%- if key == "easy" %}
{%- assign list = easy %}
{%- assign label = "简单" %}
{%- assign cls = "badge-easy" %}
{%- elsif key == "medium" %}
{%- assign list = medium %}
{%- assign label = "中等" %}
{%- assign cls = "badge-medium" %}
{%- else %}
{%- assign list = hard %}
{%- assign label = "困难" %}
{%- assign cls = "badge-hard" %}
{%- endif %}

## {{ label }}（{{ list | size }}）
{% if list.size > 0 %}
| 题目 | 难度 | 标签 |
| :--- | :---: | :--- |
{%- for p in list %}
| [{{ p.num }}. {{ p.title }}]({{ p.url | relative_url }}) | <span class="badge {{ cls }}">{{ label }}</span> | <span class="tag-list">{{ p.tags | join: " / " }}</span> |
{%- endfor %}
{%- else %}

暂无。
{%- endif %}
{%- endfor %}
