#ifndef _COMMAND_INPUT_H_
#define _COMMAND_INPUT_H_


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pwd.h>
#include <unistd.h>
#include <string.h>

#include "command_history.h"
#include "command_execute.h"

enum Color {
        BLACK = 30,   //黑色
        RED = 31,     //红色
        GREEN = 32,   //绿色
        BROWN = 33,   //棕色
        BLUE = 34,    //蓝色
        WHITE = 0,    //白色
};

//颜色设置
void color_set(int cor);
//前缀 ：用户名和设备名称的打印
void print_auther_device_path();
// 判断这个命令是否存在
bool command_strcmp(const char* ready_comm,const char* real_comm);
//命令的判断 如果该命令为false就退出,这里之所以要传入命令数组，方便在你输入exit之后析构那个命令数组
bool command_judge(const char* command_buf);
//命令的输入
void command_input(command_vector* use_v);


#endif