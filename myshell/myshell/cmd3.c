#include<stdio.h>
#include<windows.h>
//#include<assert.h>
void menu()
{
    printf("**************************\n");
    printf("****��ӭ����������ҳ��****\n");
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
        printf("������ѡ��:\n");
        scanf("%d", &n);
        int x = 0, y = 0;
        /*printf("��������������!\n");
        scanf("%d%d", &x, &y);*/
        //������������������ڴ˴�������������ʱ�����˳�ϵͳ��������ʾ��������������������ʾ
        switch (n)
        {
        case 1:
            printf("��������������!\n");
            scanf("%d%d", &x, &y);
            printf("%d\n", add(x, y));
            break;
        case 2:
            printf("��������������!\n");
            scanf("%d%d", &x, &y);
            printf("%d\n", mul(x, y));
            break;
        case 3:
            printf("��������������!\n");
            scanf("%d%d", &x, &y);
            printf("%d\n", sub(x, y));
            break;
        case 4:
            printf("��������������!\n");
            scanf("%d%d", &x, &y);
            printf("%d\n", div1(x, y));
            break;
        case 0:
            printf("�˳���������\n");
            break;
        default:
            printf("�����������������!\n");
        }
    }
}