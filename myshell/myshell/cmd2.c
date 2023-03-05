/*ls功能*/
#include<stdio.h>
#include<sys/types.h>        //opendir,reader,closed等函数相关的头文件
#include<dirent.h>            //opendir,reader,closed等函数相关的头文件

void do_ls(char[]);             //函数声明

main()   
{
     do_ls(".");        //调用do_ls，打开.目录
}

void do_ls(char dirname[])
{
    DIR* dir_ptr;                    //DIR结构体类似于FILE，是一个内部结构
    struct dirent* direntp;

    if ((dir_ptr = opendir(dirname)) == NULL)                    //判断目录是否存在，若无此目录则报错
        fprintf(stderr, "ls1:cannot open %s\t", dirname);
    else
    {            //若判断目录存在，则依次输出打印目录下所有的文件名,direntp即dirent结构体，引用了结构体的成员变量
        while ((direntp = readdir(dir_ptr)) != NULL)
            printf("%s\t", direntp->d_name);
        closedir(dir_ptr);                      /*关闭文件流,closedir()关闭参数dir所指的目录流。关闭成功则返回0，失败返回-1，错误原因存于errno 中。EBADF 参数dir为无效的目录流。
注意：目录文件作为一种文件，在打开必须关闭，否则会由于文件的进程打开文件过多而不能打开新的文件。因此opendir函数和closedir函数同样是配对出现的。*/
    }
}