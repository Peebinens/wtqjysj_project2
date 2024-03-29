# WTQJYSJ_PROJECT2-FIVE_CHESS
## 背景
如今，五子棋已成为人们日常生活中不可或缺休闲娱乐项目，但如果没有朋友在旁边的话，又无法一个人进行玩耍，本项目基于博弈树模型及α-β剪枝实现AI下棋，并且设有不同难度，使得人们可以随时随地下棋，同时提升自己的技术。
## 需求
1.	图形化界面的五子棋窗口。
2.	窗口可设置背景，棋子样式。
3.	利用决策树模型实现AI对决功能。
4.	通过调参实现不同难度的AI对决。
5.	支持导入待破解棋局。
6.	支持双人对决、人机对决。
7.	支持悔棋、重开。
8.	界面可按比例缩放，支持不同尺寸电脑上正常运行。
9.	将程序打包成可玩的游戏，同时做好内存管理，实现一点即用。
## 实现思路
1.	定义一个类加载棋盘和背景，便于切换背景。
2.	定义一个类用于前端读取落子的位置，设置棋子样式，绘制棋子，并且判断胜负，同时该类可以将当前棋盘状态传递给后端。
3.	后端定义一个类能读取棋盘状态，然后传给博弈树模型，进行运算，之后能将AI下的棋再传递给前端。
4.	博弈树通过α-β剪枝优化，提高决策效率。
5.	通过更改估价函数、搜索范围、搜索深度实现不同难度的AI下棋。
6.	通过控制前端是否将状态传递给博弈树控制双人对决或者人机对决。
7.	可通过双人对战导入待破解棋局，然后开始AI模式，破解棋局。
8.	用一个stack保存棋局状态，通过push，pop实现悔棋的功能。通过清空棋子的集合，清空stack窗口重绘实现重开。
9.	利用size()函数，对所有组件进行缩放操作，从而实现整个窗口的缩放，使程序能在不同电脑中顺利运行。
10.	在测试时，打开任务管理器，检测内存变化，来检测是否正确管理内存，最后利用Enigma Virtual Box软件将exe、dll、image打包成软件，实现随时随地玩耍。

