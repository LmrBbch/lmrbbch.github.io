---
layout: page
title: 项目技术细节
permalink: /projects/
---

按项目分组，组内按 `order` 排序。一个部分一篇，文件放在 `_projects/` 目录下。

{%- assign slugs = site.projects | map: "project" | uniq %}

共 **{{ slugs | size }}** 个项目、**{{ site.projects | size }}** 篇文章。
{% for slug in slugs %}
{%- assign chapters = site.projects | where_exp: "item", "item.project == slug" | sort: "order" %}
{%- assign head = chapters | first %}

<section class="project-section">
<div class="project-section-head">
<h2>{{ head.project_name | default: slug }}</h2>
<span class="chapter-count">{{ chapters | size }} 篇</span>
</div>
{% for c in chapters %}
{%- assign label = c.section | default: c.title %}
- [{{ label }}]({{ c.url | relative_url }}){% if c.tags %} <span class="tag-list">{{ c.tags | join: " / " }}</span>{% endif %}
{%- endfor %}
</section>
{%- endfor %}

> 项目之间的顺序按 slug 字母序，这是纯 Liquid 的固有限制。
> 想要自定义顺序，就在 `_data/projects.yml` 里手写一份顺序表，然后改成遍历它。
