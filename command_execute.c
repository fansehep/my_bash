#include "command_execute.h"
#include "command_input.h"
//命令判断函数
// ready_command传入待比较的命令 ，n_comm传入需要比较的命令，ready_size传入前缀已经匹配的字符串

bool command_explain(const char *ready_command, const char *n_comm)
{
    int r_size = strlen(ready_command);
    int n_size = strlen(n_comm);
    if (r_size != n_size)
    {
        return false;
    }
    int i = 0;
    for (i = 0; i < r_size; i++)
    {
        if (ready_command[i] != n_comm[i])
        {
            return false;
        }
    }
    return true;
}
void listdir_choice(const char *command)
{
    if (command_explain("ls", command) || command_explain("ls -a", command) || command_explain("ls -c", command))
    {
        ls();
    }
    else if (command_explain("ls -al", command))
    {
        ls__al();
    }
    else if ((!command_explain("ls -al ", command)))
    {
        ls_al_filename(command);
    }
}
void ls()
{
    DIR *dir;
    struct dirent *ptr;
    char ch = '0';
    char buf[32] = {0};
    char pathname_buf[256];
    getcwd(pathname_buf, sizeof(pathname_buf));
    //获取当前路径
    dir = opendir(pathname_buf);
    struct stat s_buf;
    int stat_buf;
    int i = 0;
    while ((ptr = readdir(dir)) != NULL)
    {
        stat_buf = stat(ptr->d_name, &s_buf);
        if (S_ISDIR(s_buf.st_mode))
        {
            //目录文件打印
            color_set(BLUE);
            printf("%-20s", ptr->d_name);
            i++;
        }
        else if (s_buf.st_mode & S_IXGRP)
        {
            //可执行文件打印
            color_set(GREEN);
            printf("%-20s", ptr->d_name);
            i++;
        }
        else if (S_ISREG(s_buf.st_mode))
        {
            //普通文件打印
            color_set(WHITE);
            printf("%-20s", ptr->d_name);
            i++;
        }
        if (i == 5)
        {
            printf("\n");
            i = 0;
        }
    }
    printf("\n");
    closedir(dir);
}
void ls__al()
{
    struct stat s_buf;
    char name[128];
    DIR *dir;

    char pathname_buf[256];
    getcwd(pathname_buf, sizeof(pathname_buf));
    //获取当前路径
    dir = opendir(pathname_buf);
    struct dirent *ptr;
    while ((ptr = readdir(dir)) != NULL)
    {
        lstat(ptr->d_name, &s_buf);
        ls_al_ana(&s_buf, ptr->d_name);
        printf("\n");
    }
    closedir(dir);
}
//ls -al命令对文件的仔细分析
void ls_al_ana(struct stat *buf, const char *file_name)
{
    //文件属性
    if (S_ISLNK(buf->st_mode))
    {
        printf("l");
        //链接文件
    }
    else if (S_ISREG(buf->st_mode))
    {
        printf("-");
        //文件
    }
    else if (S_ISCHR(buf->st_mode))
    {
        printf("d");
        //目录文件
    }
    else if (S_ISDIR(buf->st_mode))
    {
        printf("c");
        //串行端口设备
    }
    else if (S_ISFIFO(buf->st_mode))
    {
        printf("f");
        //FIFO文件
    }
    else if (S_ISSOCK(buf->st_mode))
    {
        printf("s");
        //套接字文件
    }
    //文件权限
    //  1.拥有者权限打印 owner
    if (buf->st_mode & S_IRGRP)
    {
        printf("r");
    }
    else
    {
        printf("-");
    }

    if (buf->st_mode & S_IWGRP)
    {
        printf("w");
    }
    else
    {
        printf("-");
    }

    if (buf->st_mode & S_IXGRP)
    {
        printf("x");
    }
    else
    {
        printf("-");
    }

    //  2.用户组权限打印
    if (buf->st_mode & S_IRGRP)
    {
        printf("r");
    }
    else
    {
        printf("-");
    }
    if (buf->st_mode & S_IWGRP)
    {
        printf("w");
    }
    else
    {
        printf("-");
    }
    if (buf->st_mode & S_IXGRP)
    {
        printf("x");
    }
    else
    {
        printf("-");
    }

    // 3.其他用户权限打印
    if (buf->st_mode & S_IROTH)
    {
        printf("r");
    }
    else
    {
        printf("-");
    }
    if (buf->st_mode & S_IWOTH)
    {
        printf("w");
    }
    else
    {
        printf("-");
    }
    if (buf->st_mode & S_IXOTH)
    {
        printf("x");
    }
    else
    {
        printf("-");
    }
    // 空格打印
    printf("  ");

    struct passwd *psd = getpwuid(buf->st_uid);
    struct group *grp = getgrgid(buf->st_gid);

    printf("%-3ld", buf->st_nlink);
    printf("%-8s", psd->pw_name);
    printf("%-8s", grp->gr_name);

    printf("%6ld", buf->st_size);
    char buf_time[32];
    strcpy(buf_time, ctime(&(buf->st_mtime)));
    buf_time[strlen(buf_time) - 1] = '\0';

    printf("   %s", buf_time);
    if (S_ISDIR(buf->st_mode))
    {
        //目录文件判断
        color_set(BLUE);
        printf(" %s", file_name);
        color_set(WHITE);
    }
    else if (buf->st_mode & S_IXGRP)
    {
        color_set(GREEN);
        printf(" %s", file_name);
        color_set(WHITE);
    }
    else if (S_ISREG(buf->st_mode))
    {
        //一般文件打印
        color_set(WHITE);
        printf(" %s", file_name);
        color_set(WHITE);
    }
}
// ls -al filename
void ls_al_filename(const char *command)
{
    struct stat s_buf;
    DIR *dir;
    char pathname_buf[256];
    getcwd(pathname_buf, sizeof(pathname_buf));
    //获取当前路径
    dir = opendir(pathname_buf);
    struct dirent *ptr;
    char filename[1024];
    int i = 0;
    for (; i < 64 - 1; i++)
    {
        filename[i] = command[i + 7];
        if (filename[i] == 10)
        {
            break;
        }
    }
    filename[i] = '\n';
    fflush(stdin);
    while ((ptr = readdir(dir)) != NULL)
    {
        lstat(ptr->d_name, &s_buf);
        if (command_explain(filename, ptr->d_name))
        {
            ls_al_ana(&s_buf, ptr->d_name);
            printf("\n");
            closedir(dir);
            return;
        }
    }
    if (ptr == NULL)
    {
        printf("抱歉，没有找到该文件！\n");
    }
    closedir(dir);
    return;
}

const char* getpathname_for_ls_r(const char* filename,char* pathname)
{
    getcwd(pathname,sizeof(filename));
    int cho = strlen(filename);
    int i=0;
    for( ;i<sizeof(filename);cho++,i++)
    {
        pathname[cho] = filename[i];
    }
    return pathname;
}
void ls_R()
{

}

//    2.cd
void catdir_choice(const char *command)
{
    if (command_explain("cd ..", command))
    {
        cd_next();
        return;
    }
    else if (command_explain("cd ~", command))
    {
        cd_home();
    }
    else if (command_explain("cd /", command))
    {
        cd_the_root(command);
    }
    else if (!command_explain("cd ", command))
    {
        cd_filename(command);
    }
}
//  cd ..
void cd_next()
{
    char pathname_buf[256];
    getcwd(pathname_buf, sizeof(pathname_buf));
    int i;
    for (i = sizeof(pathname_buf); i >= 0; i--)
    {
        if (pathname_buf[i] == '/')
        {
            pathname_buf[i + 1] = '\0';
            chdir(pathname_buf);
            return;
        }
    }
}
//cd ~
void cd_home()
{
    //获取当前路径
    char path_name[256];
    getcwd(path_name, sizeof(path_name));
    int i = 0;
    int cho = 0;
    for (; i < sizeof(path_name); i++)
    {
        if (path_name[i] == '/')
        {
            cho++;
        }
        if (cho == 3)
        {
            break;
        }
    }
    path_name[i + 1] = '\0';
    chdir(path_name);
    return;
}

// cd filename
void cd_filename(const char *command)
{
    char path_name[256];
    memset(path_name, 0, sizeof(path_name));
    getcwd(path_name, sizeof(path_name));
    int i = 0;
    int cho;
    for (i = 0; i < sizeof(path_name); i++)
    {
        if (path_name[i] == 0)
        {
            cho = i;
            break;
        }
    }
    path_name[cho] = '/';
    cho++;
    int j = 3;
    for (; j < strlen(command); j++)
    {
        path_name[cho++] = command[j];
    }
    if (chdir(path_name) == -1)
    {
        printf("抱歉，该目录无法进入!\n");
    }
    return;
}

// cd /
void cd_the_root()
{
    chdir("/");
    return;
}

//    3.chmod ××× filename
void change_mode(const char *command)
{
    int i = 10;
    int j = 0;
    char filename[64];

    for (;; i++, j++)
    {
        filename[j] = command[i];
        if (filename[j] == '\0')
        {
            break;
        }
    }
    int mode_u = command[6] - '0';
    int mode_g = command[7] - '0';
    int mode_o = command[8] - '0';
    // 判断权限
    if (!((mode_u >= 0 && mode_u <= 7) && (mode_u == 1 || mode_u == 2 || mode_u == 4 || mode_u == 3 || mode_u == 5 || mode_u == 6 || mode_u == 7)))
    {
        printf("权限错误！\n");
        return;
    }
    if (!((mode_g >= 0 && mode_g <= 7) && (mode_g == 1 || mode_g == 2 || mode_g == 4 || mode_g == 3 || mode_g == 5 || mode_g == 6 || mode_g == 7)))
    {
        printf("权限错误！\n");
        return;
    }
    if (!((mode_o >= 0 && mode_o <= 7) && (mode_o == 1 || mode_o == 2 || mode_o == 4 || mode_o == 3 || mode_o == 5 || mode_o == 6 || mode_o == 7)))
    {
        printf("权限错误！\n");
        return;
    }
    // chmod函数的第二个参数的传入进制 必须是 8进制
    int mode_t_c = (mode_u * 8 * 8) + (mode_g * 8) + mode_o;

    if (chmod(filename, mode_t_c) == -1)
    {
        printf("抱歉，没有找到该文件!\n");
        return;
    }
}
//    4.mv_choice
void move_dir_choice(const char *command)
{
    // 1.mv filename1 filename2
    int i = 0;
    int cho = 0;
    //如果是对一个文件重命名，那这个command命令中就不会存在/
    for (; i < strlen(command); i++)
    {
        if (command[i] == '/')
        {
            cho++;
        }
    }
    if (cho == 0)
    {
        move_name(command);
    }
    else
    {
        move_file(command);
    }
}

// 移动文件到其他目录位置去

// 1. mv file path
//   跳转到其他路径，将文件复制过去即可

void move_file(const char *command)
{
    char filename[64];
    int i = 0;
    for (i = 0; i < sizeof(filename); i++)
    {
        filename[i] = command[i + 3];
        if (filename[i] == ' ')
        {
            filename[i] = '\0';
            break;
        }
    }
    printf("-----j=%d-----\n", i + 1);
    char pathname[64];
    int j = i + 3;
    int cho = 0;
    for (; cho < sizeof(pathname); cho++, j++)
    {
        pathname[cho] = command[j];
        if (cho == '\0')
        {
            break;
        }
    }
    printf("--------%s------\n", pathname);
    printf("--------%s------\n", filename);
    int open_error = open(filename, O_RDONLY | O_WRONLY);
    if (open_error == -1)
    {
        printf("错误，无法打开文件!\n");
    }
    int chid_error = chdir(pathname);
    if (chid_error == -1)
    {
        printf("抱歉，无法找到该目录!\n");
    }
    int create_error = creat(filename, O_RDONLY | O_WRONLY);
    if (create_error == -1)
    {
        printf("抱歉，无法创建文件!\n");
    }
    char buf[2048];
    int open_fd = open(filename, O_RDONLY | O_WRONLY);
    if (open_fd == -1)
    {
        printf("抱歉，无法打开新创建的文件!\n");
    }
    while (read(open_error, buf, sizeof(buf)) != 0)
    {
        write(open_fd, buf, sizeof(buf));
    }
    close(open_error);
    close(open_fd);
    return;
}
void move_name(const char *command)
{
    // 过去的文件名
    char path1[64];
    // 新的文件名
    char path2[64];
    int i = 0;
    int j = 3;
    for (i = 0; i < sizeof(command); i++, j++)
    {
        path1[i] = command[j];
        if (command[j] == ' ')
        {
            path1[i] = '\0';
            break;
        }
    }
    i = 0;
    j += 1;
    for (; i < sizeof(command); i++, j++)
    {
        path2[i] = command[j];
        if (path2[i] == '\0')
        {
            break;
        }
    }
    int error = rename(path1, path2);
    if (error == -1)
    {
        printf("错误，参数错误!\n");
    }
    return;
}
//    5.rm
//     rm命令：rm ****
void remove_file(const char *command)
{
    char filename[64];
    int i = 0;
    int cho = 0;
    for (; i < strlen(command); i++)
    {
        if (command[i] == ' ')
        {
            cho = i;
            break;
        }
    }
    cho += 1;
    i = 0;
    for (; i < strlen(command); i++, cho++)
    {
        filename[i] = command[cho];
        if (filename[i] == '\0')
        {
            break;
        }
    }
    int error = remove(filename);
    if (error == -1)
    {
        printf("错误，没有找到该文件！\n");
    }
    return;
}
//    6.cp
//    命令大概是 cp file path
void copy_file(const char *command)
{
    char filename[64];
    char pathname[128];
    char old_pathname[64];
    getcwd(old_pathname, sizeof(old_pathname));
    int i = 0;
    int j = 0;
    int cho = 0;
    for (; i < sizeof(filename); i++)
    {
        if (command[i] == ' ')
        {
            i++;
            for (; j < sizeof(filename); j++, i++)
            {
                filename[j] = command[i];
                if (filename[j] == ' ')
                {
                    filename[j] = '\0';
                    break;
                }
            }
            break;
        }
    }
    i++;
    j = 0;
    for (; j < sizeof(pathname); j++, i++)
    {
        pathname[j] = command[i];
        if (pathname[j] == '\0')
        {
            break;
        }
    }
    int open_file = open(filename, O_RDONLY | O_WRONLY);
    if (open_file == -1)
    {
        printf("错误，没有找到该文件！\n");
        return;
    }
    int chdir_error = chdir(pathname);
    if (chdir_error == -1)
    {
        printf("打开文件失败！\n");
        return;
    }
    int create_file = open(filename, O_CREAT | O_RDONLY | O_WRONLY);
    if (create_file == -1)
    {
        printf("错误，无法创建文件");
        return;
    }
    char buf[256];
    int read_error;
    while( (read_error =read(open_file,buf,sizeof(buf)))!=0 && read_error!=-1)
    {
        write(create_file,buf,sizeof(buf));
    }
    close(open_file);
    close(create_file);
    chdir(old_pathname);
    return;
}
//    7.pwd
void print_work_dir()
{
    char pathname[64];
    getcwd(pathname,sizeof(pathname));
    color_set(WHITE);
    printf("%s\n",pathname);
    return ;
}
//    8.mkdir filename
void make_dir(const char* command)
{
    char filename[64];
    int i=6;
    int j=0;
    for( ; ;i++,j++)
    {
        filename[j] = command[i];
        if( filename[j]=='\0')
        {
            break;
        }
    }
    printf("-----%s-----\n",filename);
    int mkdir_error = mkdir(filename,0777);
    if(mkdir_error == -1)
    {
        printf("创建文件夹失败！\n");
    }
    return ;
}

//    9.touch ***
void touch_file(const char* command)
{
    int i=0;
    int j=6;
    char filename[64];
    for( ;i<sizeof(filename);i++,j++)
    {
        filename[i] = command[j];
        if( filename[i] == '\0')
        {
            break;
        }
    }
    int open_error = open(filename,O_CREAT|O_RDONLY|O_WRONLY);
    if( open_error ==-1 )
    {
        printf("错误，无法创建文件！\n");
    }
}
//    10.cat ****
void print_file(const char* command)
{
    int i=0;
    int j=4;
    char filename[64];
    for( ; ;i++,j++)
    {
        filename[i] = command[j];
        if(filename[i] == '\0')
        {
            break;
        }
    }
    int open_fd = open(filename,O_RDONLY|O_WRONLY);
    if( open_fd <0 )
    {
        printf("抱歉，无法找到该文件！\n");
        return ;
    }
    char buf[256];
    int read_error;
    while((read_error =read(open_fd,buf,sizeof(buf))) !=0&&read_error!=0)
    {
        printf("%s",buf);
    }
    close(open_fd);
}
//chown
//chown auther filename
void chown_file_owner(const char *command)
{
    char auther[64];
    char filename[64];
    int i=6;
    int j=0;
    for( ;j<sizeof(auther);i++,j++ )
    {
        auther[j] = command[i];
        if(auther[j] == ' ')
        {
            auther[j]='\0';
            break;
        }
    }
    i+=1;
    j=0;
    for( ;j<sizeof(filename);i++,j++ )
    {
        filename[j] = command[i];
        if( filename[j] == '\0')
        {
            break;
        }
    }
    struct passwd* auther_uid = getpwnam(auther);
    printf("---------auther:%s------\n",auther);
    printf("---------filename:%s----\n",filename);
    if(auther_uid == NULL)
    {
        printf("错误，没有找到该用户！\n");
        return ;
    }
    int open_fd = open(filename,O_RDONLY|O_WRONLY);
    if( open_fd == -1)
    {
        printf("错误，无法打开该文件！\n");
        return ;
    }
    int chown_error = fchown(open_fd,auther_uid->pw_uid,-1);
    if( chown_error == -1)
    {
        printf("抱歉，修改文件错误，请重试！\n");
        return;
    }
}
//    12.chgrp
void chgrp_file_group(const char *command)
{
    char auther[64];
    char filename[64];
    int i=6;
    int j=0;
    for( ;j<sizeof(auther);i++,j++ )
    {
        auther[j] = command[i];
        if(auther[j] == ' ')
        {
            auther[j]='\0';
            break;
        }
    }
    i+=1;
    j=0;
    for( ;j<sizeof(filename);i++,j++ )
    {
        filename[j] = command[i];
        if( filename[j] == '\0')
        {
            break;
        }
    }
    struct passwd* auther_uid = getpwnam(auther);
    printf("---------auther:%s------\n",auther);
    printf("---------filename:%s----\n",filename);
    if(auther_uid == NULL)
    {
        printf("错误，没有找到该用户！\n");
        return ;
    }
    int open_fd = open(filename,O_RDONLY|O_WRONLY);
    if( open_fd == -1)
    {
        printf("错误，无法打开该文件！\n");
        return ;
    }
    int chown_error = fchown(open_fd,-1,auther_uid->pw_gid);
    if( chown_error == -1)
    {
        printf("抱歉，修改文件错误，请重试！\n");
        return;
    }
}