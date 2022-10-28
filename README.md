## [电脑鼠](https://github.com/OxfordProfessor/ComputerMouse.git)

基于STM32F407，使用DFS算法实现最短迷宫路径检索，分为三种模式：1.DEBUG模式，2. 训练模式，3. 主程序模式
，DEBUG模式主要分析bug，测量必要数据，训练模式用于DFS算法训练最短路径，并将最短路径以链表形式存储Flash，
主程序模式从Flash中的链表进行遍历，使电脑鼠根据路径进行前进。



2022-09-24 19:36 V2.1

        1. 完成了链表的初始化与插入
        2. 完成了路径检索函数（DFS算法）
        3. 完成了路径检索逻辑的动作函数配置
    待实现：
        1. 测距传感器底层驱动
        2. OLED屏显示
        3. main函数框架
        4. Flash存储链表
        5. 遍历链表
        6. 动作函数及完善DFS算法（加入动作函数）
        7. 调试时记得打印sizeof(p->data)的值，根据其值改变存取flash中链表信息的启示地址与字数。
            328 STMFLASH_Write(Start_Address+4+i*sizeof(p->data),(u32*)temp,sizeof(p->data)/4+((sizeof(p->data)%4)?1:0));  
            331 count =  (sizeof(p->data)/4+((sizeof(p->data)%4)?1:0))*LengthList(L);
            349 for(s = 0; s < count/4 ; s++)       //计算方法：若sizeof(p->data)为偶数，s < 4count/sizeof(p->data)
            351 STMFLASH_Read(Start_Address+16*s+4,(u32*)temp,4);	//计算方法：STMFLASH_Read(Start_Address+16*sizeof(p->data)+4,(u32*)temp,sizeof(p->data)/4)
            352 memcpy(&data,&temp,16);            //计算方法：memcpy(&data,&temp,sizeof(p->data))

2022-09-24 19:36 V3.1

        1. 完成了链表的初始化与插入
        2. 完成了路径检索函数（DFS算法）
        3. 完成了路径检索逻辑的动作函数配置
        4. 测距传感器底层驱动
        5. OLED屏显示
        6. main函数框架
        7. Flash存储链表
        8. 遍历链表
        9. 动作函数及完善DFS算法（加入动作函数）
    待实现：
        1. 调试时记得打印sizeof(p->data)的值，根据其值改变存取flash中链表信息的启示地址与字数。
            328 STMFLASH_Write(Start_Address+4+i*sizeof(p->data),(u32*)temp,sizeof(p->data)/4+((sizeof(p->data)%4)?1:0));  
            331 count =  (sizeof(p->data)/4+((sizeof(p->data)%4)?1:0))*LengthList(L);
            349 for(s = 0; s < count/4 ; s++)       //计算方法：若sizeof(p->data)为偶数，s < 4count/sizeof(p->data)
            351 STMFLASH_Read(Start_Address+16*s+4,(u32*)temp,4);	//计算方法：STMFLASH_Read(Start_Address+16*sizeof(p->data)+4,(u32*)temp,sizeof(p->data)/4)
            352 memcpy(&data,&temp,16);            //计算方法：memcpy(&data,&temp,sizeof(p->data))
        2. 存储结构体/链表依旧有问题，记得调试

2022-10-28 15:54 V3.2

        1. 改用动态数组来存储迷宫路线，内存分配更加合理
    待实现：
        1. 调试时记得打印sizeof(p->data)的值，根据其值改变存取flash中链表信息的启示地址与字数。
            328 STMFLASH_Write(Start_Address+4+i*sizeof(p->data),(u32*)temp,sizeof(p->data)/4+((sizeof(p->data)%4)?1:0));  
            331 count =  (sizeof(p->data)/4+((sizeof(p->data)%4)?1:0))*LengthList(L);
            349 for(s = 0; s < count/4 ; s++)       //计算方法：若sizeof(p->data)为偶数，s < 4count/sizeof(p->data)
            351 STMFLASH_Read(Start_Address+16*s+4,(u32*)temp,4);	//计算方法：STMFLASH_Read(Start_Address+16*sizeof(p->data)+4,(u32*)temp,sizeof(p->data)/4)
            352 memcpy(&data,&temp,16);            //计算方法：memcpy(&data,&temp,sizeof(p->data))
        2. 存储结构体/链表依旧有问题，记得调试
        3. 调试传感器读取值
        4. 调试动作函数（如走一格，左转，右转）