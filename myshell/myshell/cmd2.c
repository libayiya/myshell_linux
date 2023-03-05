/*ls����*/
#include<stdio.h>
#include<sys/types.h>        //opendir,reader,closed�Ⱥ�����ص�ͷ�ļ�
#include<dirent.h>            //opendir,reader,closed�Ⱥ�����ص�ͷ�ļ�

void do_ls(char[]);             //��������

main()   
{
     do_ls(".");        //����do_ls����.Ŀ¼
}

void do_ls(char dirname[])
{
    DIR* dir_ptr;                    //DIR�ṹ��������FILE����һ���ڲ��ṹ
    struct dirent* direntp;

    if ((dir_ptr = opendir(dirname)) == NULL)                    //�ж�Ŀ¼�Ƿ���ڣ����޴�Ŀ¼�򱨴�
        fprintf(stderr, "ls1:cannot open %s\t", dirname);
    else
    {            //���ж�Ŀ¼���ڣ������������ӡĿ¼�����е��ļ���,direntp��dirent�ṹ�壬�����˽ṹ��ĳ�Ա����
        while ((direntp = readdir(dir_ptr)) != NULL)
            printf("%s\t", direntp->d_name);
        closedir(dir_ptr);                      /*�ر��ļ���,closedir()�رղ���dir��ָ��Ŀ¼�����رճɹ��򷵻�0��ʧ�ܷ���-1������ԭ�����errno �С�EBADF ����dirΪ��Ч��Ŀ¼����
ע�⣺Ŀ¼�ļ���Ϊһ���ļ����ڴ򿪱���رգ�����������ļ��Ľ��̴��ļ���������ܴ��µ��ļ������opendir������closedir����ͬ������Գ��ֵġ�*/
    }
}