#include "command_input.h"
#include "command_execute.h"
/*
typedef struct command_node
{
    char command_buf[256];
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
*/

void color_set(int cor)
{
    printf("\033[%dm",cor);
}


void print_auther_device_path()
{   
    struct passwd* pwd;
    //获取当前用户名
    pwd = getpwuid(getuid());
    char hostname_buf[256];
    char pathname_buf[512];
    int check_hostname = gethostname(hostname_buf,sizeof(hostname_buf));
    if(check_hostname == -1)
    {
        printf("获取当前设备失败！请重试");
        exit(-1);
    }
    color_set(RED);
    //获取当前设备名
    printf("%s@%s",pwd->pw_name,hostname_buf);
    color_set(WHITE);
    printf(":");
    //获取当前路径名
    getcwd(pathname_buf,sizeof(pathname_buf));
    color_set(BLUE);
    printf("%s",pathname_buf);
    color_set(WHITE);
    printf("$");  
    return ;
}

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

// 初始命令的判断
bool command_strcmp(const char* ready_comm,const char* real_comm)
{
    int real_length = strlen(ready_comm);
    int i;
    for(i=0 ;i<real_length;i++)
    {
        if(ready_comm[i] != real_comm[i])
        {
            return false;
        }
    }
    return true;
}
bool command_judge(const char* command_buf)
{
    if(command_strcmp("ls",command_buf))
    {
        listdir_choice(command_buf);
        return true;
    }
    else if(command_strcmp("cd",command_buf))
    {
        catdir_choice(command_buf);
        return true;
    }
    else if(command_strcmp("chmod",command_buf))
    {
        change_mode(command_buf);
        return true;
    }
    else if(command_strcmp("mv",command_buf))
    {
        move_dir_choice(command_buf);
        return true;
    }
    else if(command_strcmp("rm",command_buf))
    {
        remove_file(command_buf);
        return true;
    }
    else if(command_strcmp("cp",command_buf))
    {
        copy_file(command_buf);
        return true;
    }
    else if(command_strcmp("pwd",command_buf))
    {
        print_work_dir();
        return true;
    }
    else if(command_strcmp("mkdir",command_buf))
    {
        make_dir(command_buf);
        return true;
    }
    else if(command_strcmp("touch",command_buf))
    {
        touch_file(command_buf);
        return true;
    }
    else if(command_strcmp("cat",command_buf))
    {
        print_file(command_buf);
        return true;
    }
    else if(command_strcmp("chown",command_buf))
    {
        chown_file_owner(command_buf);
        return true;
    }
    else if(command_strcmp("exit",command_buf))
    {
        printf("欢迎您的下一次的使用哦，亲！\n");
        return false;
    }
    else
    {
        printf("本shell暂时不支持该命令，抱歉！\n");
        return true;
    }
}
//命令大致是 ××× -×  ×××  ×××\n
void command_input(command_vector* use_v)
{
    int i=0;
    for( i=0 ; i<256; i++)
    {
        scanf("%c",&((*use_v).head->command_buf[i]));
        if(use_v->head->command_buf[i] == 10)
        {
            break;
        }
    }
    (*use_v).head->command_buf[i] = '\0';
    command_history_nums_dila(&(*use_v));
    use_v->history_nums++;
    return ;
}
