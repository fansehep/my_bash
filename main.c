#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>
#include <dirent.h>
#include <stdbool.h>
#include <linux/limits.h>


#include "command_input.h"
// 一 》》输入命令 。。。
//       1.判断命令是否存在   bool  command_judge();
//该shell脚本目前仅仅支持
//    1.ls
//    2.cd
//    3.chmod
//    4.mv
//    5.rm
//    6.cp
//    7.pwd
//    8.mkdir
//    9.touch
//    10.cat
int main(int argc,char* argv[])
{
    printf("欢迎来到shell!\n");
    //初始化命令数组
    command_vector my_cod_nums;
    command_vector_init(&my_cod_nums);
    //不断进入命令循环
    while(1)
    {
        print_auther_device_path();
        fflush(stdin);
        command_input(&my_cod_nums);
        fflush(stdin);
        if(command_judge(my_cod_nums.head->command_buf) == false)
        {
            delete_command_vector(&my_cod_nums);
            return 0;
        }  
        fflush(stdin);

    }
    return 0;
}