#include<unistd.h>
#include<stdio.h>
#include<sys/types.h>
#include<fcntl.h>
/*ʵ���ļ�����*/
int main()
{
	char filename[20];
	printf("��������Ҫ���Ƶ��ļ�����");
	scanf("%s", filename);
	int srcfd = open(filename, O_RDONLY);
	if (srcfd == -1) {
		perror("open");
		return - 1;
	}
	//����һ���µ��ļ��������ļ���
	int destfd = open("cpy.txt", O_WRONLY | O_CREAT, 0664);
	if (destfd == -1) {
		perror("open");
		return -1;
	}
	//Ƶ���Ķ�д����������������
	char buf[1024] = { 0 };
	int len = 0;
	while ((len = read(srcfd, buf, sizeof(buf))) > 0) {
		write(destfd, buf, len);
	}
	//�ر��ļ�
	close(srcfd);
	close(destfd);
	return 0;
}