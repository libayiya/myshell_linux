#include<unistd.h>
#include<stdio.h>
#include<sys/types.h>
#include<fcntl.h>
/*实现文件复制*/
int main()
{
	char filename[20];
	printf("请输入您要复制的文件名：");
	scanf("%s", filename);
	int srcfd = open(filename, O_RDONLY);
	if (srcfd == -1) {
		perror("open");
		return - 1;
	}
	//创建一个新的文件（拷贝文件）
	int destfd = open("cpy.txt", O_WRONLY | O_CREAT, 0664);
	if (destfd == -1) {
		perror("open");
		return -1;
	}
	//频繁的读写操作，创建缓冲区
	char buf[1024] = { 0 };
	int len = 0;
	while ((len = read(srcfd, buf, sizeof(buf))) > 0) {
		write(destfd, buf, len);
	}
	//关闭文件
	close(srcfd);
	close(destfd);
	return 0;
}