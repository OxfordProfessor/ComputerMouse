## [电脑鼠](https://github.com/OxfordProfessor/ComputerMouse.git)

基于STM32F407，使用DFS算法实现最短迷宫路径检索，分为三种模式：1.DEBUG模式，2. 训练模式，3. 主程序模式
，DEBUG模式主要分析bug，测量必要数据，训练模式用于DFS算法训练最短路径，并将最短路径以链表形式存储Flash，
主程序模式从Flash中的链表进行遍历，使电脑鼠根据路径进行前进。

2022-09-20 17:47 V1.0

        1. 完成了链表的初始化与插入
    待实现：
        1. 测距传感器底层驱动
        2. OLED屏显示
        3. main函数框架
        4. 路径检索函数
        5. Flash存储链表
        6. 遍历链表
        7. 动作函数

     
