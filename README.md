# CHomework-Chess

> C 语言作业，一个国际象棋游戏。

## 目录

- [项目简介](#项目简介)
- [功能](#功能)
- [安装](#安装)
- [使用方法](#使用方法)

## 项目简介

这是《程序设计与算法基础 I》的 C 语言作业，实现了一个国际象棋游戏，使用了 SDL3 库提供图形界面。

## 功能

- 支持本地双人对战
- 保存和加载游戏进度
- 记录统计数据

## 安装

### 克隆项目

```bash
git clone https://github.com/SOgz12Z3Ce/CHomework-Chess
cd CHomework-Chess
```

### 构建

```bash
mkdir build
cd build
cmake -G "MinGW Makefiles" .. -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

### 运行

```bash
../release/chess.exe
```

## 使用方法

### 主界面

- 点击“开始游戏”即可开始游戏
- 点击“继续游戏”即可读取存档，存档位于游戏程序同一目录下的 `save.csv`
- 点击右上角的红色“×”按钮可退出游戏。

### 游戏界面

- 欲移动棋子，只需用鼠标拖拽棋子到另一位置
  - 如果走棋不合法，则棋子会被弹回
  - 当兵移动至底线时，需要按下右侧的升变按钮选择升变的棋子。
- 按下“保存当前进度”即可存档
