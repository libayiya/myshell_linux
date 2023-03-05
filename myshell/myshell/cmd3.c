#include<stdio.h>
#include<windows.h>
//#include<assert.h>
void menu()
{
    printf("**************************\n");
    printf("****欢迎来到计算器页面****\n");
    printf("**** 1.add    2.mul  *****\n");
    printf("*****3.sub    4.div1 *****\n");
    printf("*****0.exit          *****\n");
    printf("**************************\n");
}

int add(int x, int y)
{
    return x + y;
}

int mul(int x, int y)
{
    return x * y;
}

int sub(int x, int y)
{
    return x - y;
}

int div1(int x, int y)
{
    return x / y;
}

int main()
{
    int n = 1;
    menu();
    while (n)
    {
        printf("请输入选项:\n");
        scanf("%d", &n);
        int x = 0, y = 0;
        /*printf("请输入两个整数!\n");
        scanf("%d%d", &x, &y);*/
        //若将两个数字输入放在此处会出现输入错误时不能退出系统，而是显示“输入两个整数”的提示
        switch (n)
        {
        case 1:
            printf("请输入两个整数!\n");
            scanf("%d%d", &x, &y);
            printf("%d\n", add(x, y));
            break;
        case 2:
            printf("请输入两个整数!\n");
            scanf("%d%d", &x, &y);
            printf("%d\n", mul(x, y));
            break;
        case 3:
            printf("请输入两个整数!\n");
            scanf("%d%d", &x, &y);
            printf("%d\n", sub(x, y));
            break;
        case 4:
            printf("请输入两个整数!\n");
            scanf("%d%d", &x, &y);
            printf("%d\n", div1(x, y));
            break;
        case 0:
            printf("退出计算器！\n");
            break;
        default:
            printf("输入错误，请重新输入!\n");
        }
    }
}