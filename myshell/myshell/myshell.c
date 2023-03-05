#include<stdio.h> 
#include<unistd.h> 
#include<wait.h> 
#include<stdlib.h> 
#include<string.h> 
#include<ctype.h> 
char* argv[8];//��ִ���ļ����ļ��� 
int argc = 0;
void do_parse(char* buf)//��������
{ 
	int i; 
	int status = 0;//�����ִ�е�״̬ 
	for(i = 0; buf[i] != 0; i++) { 
		if(!isspace(buf[i]) && status == 0)//���ַ����ո� 
		{ 
			argv[argc++] = buf + i; 
			status = i; 
		} 
		else if(isspace(buf[i]))//�ӿո��ַ� 
		{ 
				status = 0; 
				buf[i] = 0; 
		} 
	}
	argv[argc] = NULL; 
} 
void do_execute()//ִ������ 
{ 
	pid_t pid = fork(); 
	int wpid; 
	if(pid > 0)//������ 
	{ 
		printf("This is parent process,id is %d\n",getpid());
		int st; 
		while (wait(&st) != pid); 
	}
	else if (pid == 0) { //�ӽ���
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
		 fflush(stdout);//ˢ�»����� 
		 fgets(buf, sizeof(buf), stdin); 
		 //������� exit�����˳����� 
		 if(!strcmp(buf, "exit\n"))
			 exit(0); 
		 //�����������ⲿ����
		 else if (strcmp(buf, "./cmd4") == 0 || strcmp(buf, "./cmd3") == 0 || strcmp(buf, "./cmd2") == 0 || strcmp(buf, "./cmd1") == 0) {
			 do_parse(buf);//���������� 
			 do_execute();//ִ������ 
		 }
		 //�����������ڲ�����
		 else { 
			 buf[strlen(buf) - 1] = '\0';	//��Ϊ�ַ���������־��������ĩβ��һ��'\n'
			 char* myArg[30];
			 //������ַ��������е�һ������
			 myArg[0] = strtok(buf, " ");//�Կո�Ϊ�ָ���ȷָ������
			 int i = 1;
			 //�õ�֮��������в���
			 while (myArg[i] = strtok(NULL, " "))//Ĭ�ϴ���һ�ε��Ӵ�����ȡ�����Դ�NULL
			 {
				 i++;
			 }

			 //���ӽ���ִ������
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
			 if (ret > 0 && WIFEXITED(status))	//�ȴ��κ��ӽ���ʶ����Ϊ pid ���ӽ��̡�
			 {
				 printf("signal:%d,", WIFEXITED(status));
				 printf("exit code:%d\n", WEXITSTATUS(status));
			 }
		 } 
	 } 
	 return 0; 
}

/*
��1��Demo1
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
			printf("�������˳�\n");
			printf("\n���̽�����pid:%d\n", getpid());
			exit(0);
		}
		else if(strcmp(input,"cmd3") == 0 || strcmp(input,"cmd2") == 0 || strcmp(input,"cmd1") == 0)
		{
			//�����ӽ���
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
				printf("\n���̽�����pid:%d\n", temp);
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

#define LEN 1024//������ַ��������1024��
#define NUM 32//����+�����в������32��

int main()
{
	char cmd[LEN];
	while (1)
	{
		//��ӡ��ʾ��
		printf("[yh@my_centos dir]& ");

		//����û�����(��ʵ����һ���ַ���)
		fgets(cmd, LEN, stdin);//�ӱ�׼����(һ���Ǽ���)�����û�������ַ���
		cmd[strlen(cmd) - 1] = '\0';//���һ��λ�ñ�����'\n'

		//�����ַ���
		char* myArg[NUM];
		//������ַ��������е�һ������
		myArg[0] = strtok(cmd, " ");//�Կո�Ϊ�ָ���ȷָ������
		int i = 1;
		//�õ�֮��������в���
		while (myArg[i] = strtok(NULL, " "))//Ĭ�ϴ���һ�ε��Ӵ�����ȡ�����Դ�NULL
		{
			i++;
		}

		//���ӽ���ִ������
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