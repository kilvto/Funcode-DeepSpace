# 🎮 Funcode-DeepSpace

> 一款基于 **FunCode 平台** 开发的小游戏，融合了 **平台跳跃** 与 **解谜玩法**，玩家将在太空主题世界中操控角色完成闯关、触发机关、解决谜题。

---

## 🚀 项目简介

`Funcode-DeepSpace` 是一个面向学习和创作的游戏作品，涵盖动画演出、交互机制、关卡构建等多个方面，适合作为 C语言 和 FunCode 平台的实践案例。

---

## 🧩 核心玩法

- 🕹️ **平台跳跃**：角色可以跳跃、移动、触发平台机制，完成地形挑战
- 🧠 **解谜要素**：关卡中包含机关、电路、激光等可互动组件，需要合理连接与操作
- 🎬 **剧情过场**：多个 Intro/Outro 演出，营造太空冒险氛围
- 🎵 **音效与反馈**：背景音乐与音效同步，提升沉浸体验

---

## 📁 项目结构

Funcode-DeepSpace/
├── project.funProj # FunCode 项目文件
├── SourceCode/ # 项目源码区
│ ├── Header/ # 头文件目录
│ │ ├── CommonAPI.h
│ │ ├── LessonX.h
│ │ └── mylib.h
│ ├── Src/ # 源代码实现
│ │ ├── CommonAPI.a / .lib # 平台库文件
│ │ ├── LessonX.cpp
│ │ ├── Main.cpp
│ │ ├── Main.h
│ │ └── mylib.cpp / mylib.h
│
├── VCProject2008/2010/2012 # 各版本 VS 工程（兼容 FunCode 平台）
├── Icon.ico # 游戏图标
├── Icon.rc # 资源文件
└── README.md # 本说明文件

---

## 🔧 使用方法

1. 克隆项目：
   ```bash
   git clone git@github.com:kilvto/Funcode-DeepSpace.git
-打开 FunCode 平台，导入该项目所在文件夹
-运行程序，即可开始

---

💡技术点说明
使用 C 语言中的函数封装、静态变量、作用域等编程特性

管理游戏状态机与物理判定（如是否在地面、是否连通）

通过宏与常量控制演出流程、音效播放等细节

---
🧑‍💻 作者
Kilvto

---
📜 License
本项目为学习作品，禁止商用，禁止未经授权转载或发布。

---
❤️ 特别说明
欢迎参考本项目学习 FunCode 平台的进阶用法，也欢迎提出优化建议！
