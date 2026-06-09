# Landlords（斗地主）

基于 Qt6 的欢乐斗地主单机游戏，由[爱编程的大丙](https://subingwen.cn/)老师的 Qt5 斗地主课程改编而来，进行了代码优化并适配 Qt6。

## 特性

- 🎴 完整的斗地主玩法：叫地主、抢地主、出牌、炸弹、飞机、顺子等
- 🤖 智能 AI 对手，支持叫牌和出牌策略
- 🎨 多套随机背景，丰富的卡牌动画效果（炸弹、飞机、顺子、连对）
- 🎵 背景音乐与音效（出牌、抢地主、倒计时提醒等）
- ⏱️ 出牌倒计时，增加游戏紧张感
- 💯 得分结算面板，支持连续多局对战

## 技术栈

- **语言：** C++17
- **框架：** Qt 6.5+（Core / Widgets / Multimedia）
- **构建：** CMake 3.19+
- **平台：** Windows

## 构建

```bash
# 克隆项目
git clone https://github.com/1613661434/Landlords
cd Landlords

# 配置
cmake -B build -DCMAKE_BUILD_TYPE=Release

# 编译
cmake --build build --config Release
```

> 需要安装 Qt 6.5 及以上版本，并确保 CMake 能找到 Qt 安装路径。

## 运行

编译完成后，双击 `Landlords.exe`，点击"开始游戏"即可开始对战。

## 致谢

本项目基于[爱编程的大丙](https://subingwen.cn/)老师的 Qt5 斗地主课程，在原课程基础上进行了代码优化、语法简化和 Qt6 适配。感谢老师的精彩课程！

## 许可

[LGPL v3.0](LICENSE) © 2026 [ol木子李lo](https://1613661434.github.io/)
