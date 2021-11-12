#ifndef _COMMAND_HISTORY_H_
#define _COMMAND_HISTORY_H_
//单个命令节点
#include <stdio.h>
#include <unistd.h>
#include <malloc.h>
#include <string.h>

typedef struct command_node
{
    char command_buf[64];
}command_node;


typedef struct command_vector
{
    command_node* head;
    //存储历史命令
    int history_nums;
    //历史命令个数
    int vector_size;
    //数组当前容量
}command_vector;


//该数组的构造函数
void command_vector_init(command_vector* c_v);
//扩容函数
void command_history_nums_dila(command_vector* c_v);
//析构函数
void delete_command_vector(command_vector* c_v);
#endif