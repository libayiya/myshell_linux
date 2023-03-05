///*cat ����*/
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
//    /* �������ģʽ */
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
//        if ((fp = fopen(*argv, "r")) == NULL)  /* ���ļ�*/
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
//ӳ���ϸ����Ҫ�ο� ASCII ���룬����˼·�Ǹ��ݵ�ǰ�ַ� ch ��ֵ���ڿɴ�ӡ�ַ���Χ�ڵ�ֱ�������
//���ɴ�ӡ�ַ����ȸ���Ԥ���Ĺ���ӳ�䵽һ��ɴ�ӡ�ַ����ڼ�һ��ǰ׺�����
//[0, 32): ^M (ch + 64)����һ��Χ�ǿ����ַ���ӳ�䵽 ch+64 ���ַ��������� ^ ǰ׺��������� \r��Ӧ�� ASCII ����13����ʾ�������� ^M��M�� ASCII���� 13 + 64 = 77��ֻ�� \t �� \n�����⡣
//[32, 127): �����Χ���ַ��ǿɴ�ӡ�ַ���ֱ��������ɡ�
//127: ����ַ��������� del��ӳ��� ^?
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
//    for (prev = '\n'; (ch = getc(fp)) != EOF; prev = ch) /* ѭ��ֱ�������ļ�ĩλ */
//    {
//        /* ���й��ܵĿ��أ�����������״̬����������߼��� */
//        if (prev == '\n')
//        {
//            /* -s �������������Ŀ��У����������ļ�������3�еĿ��У���ֻ���һ�����С�
//            ������ԵĿ���Ҳ�Ͳ������к��ˣ���β��$��ȻҲ��������ˡ�
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
//            /* ��һ������������\n�����������ÿ�������ǰ��֮ǰ�������һ���к� */
//            if (nflag)  /* -n��������ÿ����������к� */
//            {
//                if (!bflag || ch != '\n') /* -b���Կ��м����к� */
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
//        if (ch == '\n') /* ���ָ���� -E ����������ÿ����β�����һ��$���߼��ϣ���ÿ�����\nǰ�������һ��$�������\n */
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
//    /* �޲� */
//    if (*argv == NULL)
//    {
//        raw_cat(fileno(stdin), "stdin");
//        return;
//    }
//    /* �в� */
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
//    /* cat ���������������и���, Ĭ���Ǳ�׼����(stdin), ������Ǳ�׼���(stdout) */
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
    printf("��������Ҫ�鿴���ļ����ƣ�");
    char filename[20];
    scanf("%s", filename);
    FILE *fp = fopen(filename, "r");   // ��ֻ����ʽ���ļ���argv[1]�������Ҫ��ʾ���ļ�·����
    int read_ret;
    if(fp == NULL)  //����򿪳ɹ��Ļ��ͰѸ��ļ�����Ϣ�ṹ���ַ�����ļ�ָ��fp������򿪲��ɹ��Ļ��ͽ��ļ�ָ��fp��Ϊ��Ч(NULL)
    {
        printf("open file failed!\n");
        return -1;
    }
    while(1)  //�ɹ���
    {
        read_ret = fgetc(fp);  //�����ַ���д
        if(feof(fp))  // feof�ж��ļ���������fp�Ѿ�ָ���ļ�ĩβ����feof��fp������ֵΪ���桱�������ط���ֵ
        {
            printf("read file endl\n");   //��ʾ��д����
            break;
        }
        fputc(read_ret,stdout);  //�ѵ����ַ��������Ļ
    }
}
