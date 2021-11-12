#ifndef _COMMAND_EXECUTE_H_
#define _COMMAND_EXECUTE_H_

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <linux/limits.h>
#include <dirent.h>
#include <sys/ioctl.h>
#include <termio.h>
#include <grp.h>
#include <pwd.h>
#include <errno.h>
#include <time.h>
#include <stdbool.h>

//    1.ls
bool command_explain(const char* ready_command,const char* n_comm);
void listdir_choice(const char* command);
void ls();
void ls__al();
void ls_al_ana(struct stat* buf,const char* file_name);
void ls_al_filename(const char* command);
void ls_R();
const char* getpathname_for_ls_r(const char* filename,char* pathname);
//    2.cd
void catdir_choice(const char* command);
void cd_home();
void cd_next();
void cd_the_root();
void cd_filename(const char* command);
//    3.chmod
void change_mode(const char* command);
//    4.mv
void move_dir_choice(const char* command);
void move_name(const char* command);

//   
void move_file(const char* command);

//    5.rm
void remove_file(const char* command);
//    6.cp
void copy_file(const char* command);
//    7.pwd
void print_work_dir();
//    8.mkdir
void make_dir(const char* command);
//    9.touch
void touch_file(const char* command);
//    10.cat
void print_file(const char* command);
//    11.chown
void chown_file_owner(const char* command);
//    12.chgrp
void chgrp_file_group(const char* command);



#endif