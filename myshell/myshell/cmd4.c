///*cat 功能*/
//int main(int argc, char* argv[])
//{
//    int ch;
//
//    while ((ch = getopt(argc, argv, "benstuv")) != -1)
//    {
//        switch (ch) {
//        case 'b':
//            bflag = nflag = 1; /* -b implies -n */
//            break;
//        case 'e':
//            eflag = vflag = 1; /* -e implies -v */
//            break;
//        case 'n':
//            nflag = 1;
//            break;
//        case 's':
//            sflag = 1;
//            break;
//        case 't':
//            tflag = vflag = 1; /* -t implies -v */
//            break;
//        case 'u':
//            setvbuf(stdout, NULL, _IONBF, 0);
//            break;
//        case 'v':
//            vflag = 1;
//            break;
//        default:
//            fprintf(stderr, "usage: %s [-benstuv] [file ...]\n",
//                *argv);
//            return 1;
//        }
//    }
//    argv += optind;
//    /* 命令参数模式 */
//    if (bflag || eflag || nflag || sflag || tflag || vflag)
//        cook_args(argv);
//    else
//        raw_args(argv);
//    if (fclose(stdout))
//        err(1, "stdout");
//    return rval;
//}
//
//void cook_args(char** argv)
//{
//    FILE* fp;
//
//    if (*argv == NULL)
//    {
//        cook_buf(stdin, "stdin");
//        return;
//    }
//
//    for (; *argv != NULL; argv++)
//    {
//        if (!strcmp(*argv, "-"))
//        {
//            cook_buf(stdin, "stdin");
//            clearerr(stdin);
//            continue;
//        }
//        if ((fp = fopen(*argv, "r")) == NULL)  /* 打开文件*/
//        {
//            warn("%s", *argv);
//            rval = 1;
//            continue;
//        }
//        cook_buf(fp, *argv);
//        fclose(fp);
//    }
//}
///*
//映射的细节需要参考 ASCII 编码，基本思路是根据当前字符 ch 的值，在可打印字符范围内的直接输出，
//不可打印字符则先根据预定的规则映射到一组可打印字符，在加一个前缀输出：
//[0, 32): ^M (ch + 64)，这一范围是控制字符，映射到 ch+64 的字符，并配上 ^ 前缀输出，比如 \r对应的 ASCII 码是13，显示出来就是 ^M，M的 ASCII码是 13 + 64 = 77。只有 \t 和 \n是例外。
//[32, 127): 这个范围的字符是可打印字符，直接输出即可。
//127: 这个字符的意义是 del，映射成 ^?
//(127, 128 + 32): M-^(ch - 128 + 64)
//[128+32, 128 + 127): M-(ch-128)
//128 + 127: M-^?
//*/
//void cook_buf(FILE* fp, const char* filename)
//{
//    unsigned long long line;
//    int ch, gobble, prev;
//
//    line = gobble = 0;
//    for (prev = '\n'; (ch = getc(fp)) != EOF; prev = ch) /* 循环直到输入文件末位 */
//    {
//        /* 空行功能的开关：根据上下文状态调整输出或者计数 */
//        if (prev == '\n')
//        {
//            /* -s 参数跳过连续的空行：比如输入文件有连续3行的空行，但只输出一个空行。
//            自如忽略的空行也就不计算行号了，行尾的$自然也不比输出了。
//            */
//            if (sflag)
//            {
//                if (ch == '\n')
//                {
//                    if (gobble)
//                        continue;
//                    gobble = 1;
//                }
//                else
//                    gobble = 0;
//            }
//            /* 用一个计数器计算\n的输出个数，每次输出当前行之前，先输出一个行号 */
//            if (nflag)  /* -n参数，在每行行首输出行号 */
//            {
//                if (!bflag || ch != '\n') /* -b不对空行计算行号 */
//                {
//                    fprintf(stdout, "%6llu\t", ++line);
//                    if (ferror(stdout))
//                        break;
//                }
//                else if (eflag)
//                {
//                    (void)fprintf(stdout, "%6s\t", "");
//                    if (ferror(stdout))
//                        break;
//                }
//            }
//        }
//        if (ch == '\n') /* 如果指定了 -E 参数，则在每行行尾多输出一个$。逻辑上，在每次输出\n前，先输出一个$，再输出\n */
//        {
//            if (eflag && putchar('$') == EOF)
//                break;
//        }
//        else if (ch == '\t')
//        {
//            if (tflag)
//            {
//                if (putchar('^') == EOF || putchar('I') == EOF)
//                    break;
//                continue;
//            }
//        }
//        else if (vflag)
//        {
//            if (!isascii(ch))
//            {
//                if (putchar('M') == EOF || putchar('-') == EOF)
//                    break;
//                ch = toascii(ch);
//            }
//            if (iscntrl(ch))
//            {
//                if (putchar('^') == EOF ||
//                    putchar(ch == '\177' ? '?' :
//                        ch | 0100) == EOF)
//                    break;
//                continue;
//            }
//        }
//        if (putchar(ch) == EOF)
//            break;
//    }
//    if (ferror(fp))
//    {
//        warn("%s", filename);
//        rval = 1;
//        clearerr(fp);
//    }
//    if (ferror(stdout))
//        err(1, "stdout");
//}
//
//void raw_args(char** argv)
//{
//    int fd;
//    /* 无参 */
//    if (*argv == NULL)
//    {
//        raw_cat(fileno(stdin), "stdin");
//        return;
//    }
//    /* 有参 */
//    for (; *argv != NULL; argv++)
//    {
//        if (!strcmp(*argv, "-"))
//        {
//            raw_cat(fileno(stdin), "stdin");
//            continue;
//        }
//        if ((fd = open(*argv, O_RDONLY, 0)) == -1)
//        {
//            warn("%s", *argv);
//            rval = 1;
//            continue;
//        }
//        raw_cat(fd, *argv);
//        close(fd);
//    }
//}
//
//void raw_cat(int rfd, const char* filename)
//{
//    int wfd;
//    ssize_t nr, nw, off;
//    static size_t bsize;
//    static char* buf = NULL;
//    struct stat sbuf;
//    /* cat 的输入流由命令行给定, 默认是标准输入(stdin), 输出流是标准输出(stdout) */
//    wfd = fileno(stdout);
//    if (buf == NULL)
//    {
//        if (fstat(wfd, &sbuf) == -1)
//            err(1, "stdout");
//        bsize = MAXIMUM(sbuf.st_blksize, BUFSIZ);
//        buf = (char*)malloc(bsize);
//        if (buf == NULL)
//            err(1, NULL);
//    }
//    while ((nr = read(rfd, buf, bsize)) != -1 && nr != 0)
//    {
//        for (off = 0; nr; nr -= nw, off += nw)
//        {
//            if ((nw = write(wfd, buf + off, nr)) == -1 || nw == 0)
//                err(1, "stdout");
//        }
//    }
//    if (nr == -1)
//    {
//        warn("%s", filename);
//        rval = 1;
//    }
//}

#include <stdio.h>
int main()
{
    printf("请输入您要查看的文件名称：");
    char filename[20];
    scanf("%s", filename);
    FILE *fp = fopen(filename, "r");   // 以只读方式打开文件，argv[1]是输入的要显示的文件路径名
    int read_ret;
    if(fp == NULL)  //如果打开成功的话就把该文件的信息结构体地址赋给文件指针fp，如果打开不成功的话就将文件指针fp置为无效(NULL)
    {
        printf("open file failed!\n");
        return -1;
    }
    while(1)  //成功打开
    {
        read_ret = fgetc(fp);  //单个字符读写
        if(feof(fp))  // feof判断文件结束，若fp已经指向文件末尾，则feof（fp）函数值为“真”，即返回非零值
        {
            printf("read file endl\n");   //提示读写结束
            break;
        }
        fputc(read_ret,stdout);  //把单个字符输出到屏幕
    }
}
