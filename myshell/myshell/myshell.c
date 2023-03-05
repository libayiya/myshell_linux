#include<stdio.h> 
#include<unistd.h> 
#include<wait.h> 
#include<stdlib.h> 
#include<string.h> 
#include<ctype.h> 
char* argv[8];//可执行文件的文件名 
int argc = 0;
void do_parse(char* buf)//解析命令
{ 
	int i; 
	int status = 0;//命令可执行的状态 
	for(i = 0; buf[i] != 0; i++) { 
		if(!isspace(buf[i]) && status == 0)//从字符到空格 
		{ 
			argv[argc++] = buf + i; 
			status = i; 
		} 
		else if(isspace(buf[i]))//从空格到字符 
		{ 
				status = 0; 
				buf[i] = 0; 
		} 
	}
	argv[argc] = NULL; 
} 
void do_execute()//执行命令 
{ 
	pid_t pid = fork(); 
	int wpid; 
	if(pid > 0)//父进程 
	{ 
		printf("This is parent process,id is %d\n",getpid());
		int st; 
		while (wait(&st) != pid); 
	}
	else if (pid == 0) { //子进程
		printf("This is child process, id is %d\n", getpid());
		execvp(argv[0], argv); 
		perror("execvp"); 
		exit(EXIT_FAILURE); 
	}
	else { 
		/*printf("vfork() error\n");
		exit(-1);*/
		perror("fork!"); 
		exit(EXIT_FAILURE); 
	} 
}
int main() {
	 char buf[1024] = {};
	 while (1) {
		 printf("myshell>"); 
		 fflush(stdout);//刷新缓冲区 
		 fgets(buf, sizeof(buf), stdin); 
		 //如果输入 exit，则退出程序 
		 if(!strcmp(buf, "exit\n"))
			 exit(0); 
		 //如果输入的是外部命令
		 else if (strcmp(buf, "./cmd4") == 0 || strcmp(buf, "./cmd3") == 0 || strcmp(buf, "./cmd2") == 0 || strcmp(buf, "./cmd1") == 0) {
			 do_parse(buf);//解析命令行 
			 do_execute();//执行命令 
		 }
		 //如果输入的是内部命令
		 else { 
			 buf[strlen(buf) - 1] = '\0';	//作为字符串结束标志，本来的末尾是一个'\n'
			 char* myArg[30];
			 //输入的字符串至少有第一个命令
			 myArg[0] = strtok(buf, " ");//以空格为分割符先分割出命令
			 int i = 1;
			 //拿到之后的命令行参数
			 while (myArg[i] = strtok(NULL, " "))//默认从上一次的子串中提取，所以传NULL
			 {
				 i++;
			 }

			 //让子进程执行命令
			 pid_t id = fork();
			 if (id == 0)
			 {
				 //child
				 execvp(myArg[0], myArg);
				 printf("Command not found!\n");
				 break;
				 //exit(11);
			 }

			 int status = 0;
			 pid_t ret = waitpid(id, &status, 0);
			 if (ret > 0 && WIFEXITED(status))	//等待任何子进程识别码为 pid 的子进程。
			 {
				 printf("signal:%d,", WIFEXITED(status));
				 printf("exit code:%d\n", WEXITSTATUS(status));
			 }
		 } 
	 } 
	 return 0; 
}

/*
（1）Demo1
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
	char input[20];
	pid_t pid;

	while(1)
	{
		printf("myshell->");
		scanf("%s", input);

		if(strcmp(input,"exit") == 0)
		{
			printf("父进程退出\n");
			printf("\n进程结束，pid:%d\n", getpid());
			exit(0);
		}
		else if(strcmp(input,"cmd3") == 0 || strcmp(input,"cmd2") == 0 || strcmp(input,"cmd1") == 0)
		{
			//创建子进程
			pid = fork();

			if(pid < 0)
			{
				printf("vfork() error\n");
				exit(-1);
			}
			else if(pid == 0)
			{
				printf("I am the child process, my process id is %d\n",getpid());
				char path[80] = "../t1/";
				char *lastName = ".o";
				strcat(path, input);
				strcat(path, lastName);
				execl(path,"",NULL);
			}
			else
			{
				printf("I am the parent process, my process id is %d\n",getpid());

				pid_t temp = wait(NULL);
				printf("\n进程结束，pid:%d\n", temp);
			}
		}
		else
		{
			printf("Command not found\n");
			continue;
		}

	}

	return 0;
}
*/
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>

#define LEN 1024//读入的字符个数最多1024个
#define NUM 32//命令+命令行参数最多32个

int main()
{
	char cmd[LEN];
	while (1)
	{
		//打印提示符
		printf("[yh@my_centos dir]& ");

		//获得用户输入(其实就是一个字符串)
		fgets(cmd, LEN, stdin);//从标准输入(一般是键盘)读入用户输入的字符串
		cmd[strlen(cmd) - 1] = '\0';//最后一个位置本来是'\n'

		//解析字符串
		char* myArg[NUM];
		//输入的字符串至少有第一个命令
		myArg[0] = strtok(cmd, " ");//以空格为分割符先分割出命令
		int i = 1;
		//拿到之后的命令行参数
		while (myArg[i] = strtok(NULL, " "))//默认从上一次的子串中提取，所以传NULL
		{
			i++;
		}

		//让子进程执行命令
		pid_t id = fork();
		if (id == 0)
		{
			//child
			execvp(myArg[0], myArg);
			exit(11);
		}

		int status = 0;
		pid_t ret = waitpid(id, &status, 0);
		if (ret > 0 && WIFEXITED(status))
		{
			printf("signal:%d,", WIFEXITED(status));
			printf("exit code:%d\n", WEXITSTATUS(status));
		}
	}
	return 0;
}