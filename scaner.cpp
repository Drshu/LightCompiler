
#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <string.h>
#include "scaner.h"

#define FILENAME_MAX_ 30
char prog[800], token[20], number[50];
char s[1024];
char ch, f, f1;
int syn, p, m, n;
int flag;
int lines = 0;
char* rwtab[6] = { "function","if","then","while","do","endfunc" };
char* rwtabS[6] = { "<","<=","!=",">",">=","==" };
char filename[FILENAME_MAX];
int statusFile = 0;//输出模式，1的时候使用文件输出
extern bool haveEnter;
extern int status;
void scaner()//词法识别
{
    for (n = 0; n < 20; n++)
    {
        token[n] = NULL;
        number[n] = NULL;
    }
    ch = prog[++p];
    if ((ch >= 'a'&&ch <= 'z') || (ch >= 'A'&&ch <= 'Z'))
    {
        m = 0;
        while ((ch >= 'a'&&ch <= 'z') || (ch >= '0'&&ch <= '9') || (ch >= 'A'&&ch <= 'Z'))
        {
            token[m++] = ch;
            ch = prog[++p];
        }
        token[m] = '\0';
        ch = prog[--p];
        syn = 10;
        for (n = 0; n < 6; n++)
        {
            if (strcmp(token, rwtab[n]) == 0)
            {
                syn = n + 1;
                break;
            }
        }
    }
    else if (ch >= '0'&&ch <= '9')
    {
        int i = 0;
        syn = 100;
        while (ch >= '0'&&ch <= '9')
        {
            // if()
            number[i++] = ch;
            ch = prog[++p];
            if (ch == '.')
            {
                number[i++] = ch;
                ch = prog[++p];
                while (ch >= '0'&&ch <= '9')
                {
                    number[i++] = ch;
                    ch = prog[++p];
                }
            }
            if (ch == 'e')
            {
                number[i++] = ch;
                ch = prog[++p];
                if (ch == '-')
                {
                    number[i++] = ch;
                    ch = prog[++p];
                }
                if (ch == '+')
                    ch = prog[++p];
                while (ch >= '0'&&ch <= '9')
                {
                    number[i++] = ch;
                    ch = prog[++p];
                }
            }
            if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))//处理数字加其它字母的情况
            {
                number[i++] = ch;
                syn = -1;
                break;
            }
        }
        ch = prog[--p];
        if (syn != -1)
            syn = 11;
    }
    else
    {
        switch (ch)
        {
            case'<':m = 0;
                token[m++] = ch;
                ch = prog[++p];
                if (ch == '=')
                {
                    syn = 21;
                    token[m] = ch;
                }
                else
                {
                    syn = 20;
                    ch = prog[--p];
                }
                break;

            case'>':m = 0;
                token[m++] = ch;
                ch = prog[++p];
                if (ch == '=')
                {
                    syn = 24;
                    token[m] = ch;
                }
                else
                {
                    syn = 23;
                    ch = prog[--p];
                }
                break;

            case'=':m = 0;
                token[m++] = ch;
                ch = prog[++p];
                if (ch == '=')
                {
                    syn = 25;
                    token[m] = ch;
                }
                else
                {
                    syn = 18;
                    ch = prog[--p];
                }
                break;
            case'!':
                m = 0;
                token[m++] = ch;
                ch = prog[++p];
                if (ch == '=')
                {
                    syn = 22;
                    token[m] = ch;
                }
                else
                    syn = -1;
                break;
            case'+': {
                int j = 2;
                ch = prog[++p];
                while (ch == ' ' || ch == '\t' || ch == '\n')
                {
                    ch = prog[++p];
                }
                if (ch >= '0'&&ch <= '9')
                {
                    p--;
                    ch = prog[--p];
                    while (ch == ' ' || ch == '\t' || ch == '\n')
                    {
                        ch = prog[--p];
                        j++;
                    }
                    if ((ch >= 'a'&&ch <= 'z') || (ch >= '0'&&ch <= '9') || (ch >= 'A'&&ch <= 'Z'))
                    {
                        syn = 13;
                        token[0] = '+';
                    }
                    else
                    {
                        f = '+';
                        syn = 30;
                    }
                    p += j;
                }
                else {
                    syn = 13;
                    token[0] = '+';
                }
                p--;
                break;
            }
            case'-':
            {
                int j = 2;
                ch = prog[++p];
                while (ch == ' ' || ch == '\t' || ch == '\n')
                {
                    ch = prog[++p];
                }
                /**/
                if ((ch >= '0'&&ch <= '9')|| (ch >= 'a'&&ch <= 'z') || (ch >= 'A'&&ch <= 'Z'))
                {
                    p--;
                    ch = prog[--p];
                    while (ch == ' ' || ch == '\t' || ch == '\n')
                    {
                        ch = prog[--p];
                        j++;
                    }
                    if ((ch >= 'a'&&ch <= 'z') || (ch >= '0'&&ch <= '9') || (ch >= 'A'&&ch <= 'Z'))
                    {
                        syn = 14;
                        token[0] = '-';
                    }
                    else
                    {
                        f = '-';
                        syn = 30;
                    }
                    p += j;
                }
                else {
                    syn = 14;
                    token[0] = '-';
                }
                p--;
                break;
            }
            case'*':syn = 15; token[0] = ch; break;
            case'/':syn = 16; token[0] = ch; break;
            case';':syn = 26; token[0] = ch; break;
            case'(':syn = 27; token[0] = ch; break;
            case')':syn = 28; token[0] = ch; break;
            case'\t':syn = 30; break;
            case'\n':syn = 31; haveEnter = true;  lines++; break;
            case' ':syn = 30; break;//这三个都是不需要处理的字符，直接跳过就可以，在之后可使用syn是否为30判断是不是有效字符
            case'#':syn = 0; token[0] = ch; break;
            default:syn = -1;
        }
    }
}
void readfromfile()
{
    FILE *fp;
    char str;
    printf("请输入文件路径\n");
    scanf("%s", filename);
    if ((fp = fopen(filename, "r")) == NULL)
    {
        printf("文件打开失败\n");
        return;
    }
    int i = 1;
    while ((str = fgetc(fp)) != EOF)
    {
        prog[i] = str;
        //     putchar(str);
        i++;
    }
    prog[i] = '\0';
    fclose(fp);
}
void morphologyAnalysis()//词法分析
{
    printf("1.从键盘输入\n2.从文件输入\n");
    scanf("%d", &flag);
    switch (flag)
    {
        case 1:
            p = 0;
            printf("\n please input string:\n");
            getchar();
            do {
                scanf("%c", &ch);
                prog[++p] = ch;
            } while (ch != '#');
            break;
        case 2:
            readfromfile();
            break;
    }
    output();

}


void output() {
    printf("1.从屏幕输出\n2.从文件输出\n");
    scanf("%d", &flag);
    switch (flag)
    {
        case 1:
            p = 0;
            do
            {
                scaner();
                switch (syn)
                {
                    case 11:
                        if (f == '-')
                        {
                            printf("\n(%d,-%s)", syn, number);
                            f = '\0';
                        }
                        else
                            printf("\n(%d,%s)", syn, number);
                        break;
                    case 30:
                        break;
                    case -1:
                        printf("error\n事故现场：%d\n", lines + 1);
                        status = 1;
                        break;
                    default:
                        printf("\n(%d,%s)", syn, token);
                        //printf("");
                }
            } while (syn != 0);
            break;
        case 2:
            /*{
                FILE *fp;
                printf("请输入文件路径\n");
                scanf("%s", filename);
                if ((fp = fopen(filename, "w")) == NULL)
                {
                    printf("文件打开失败\n");
                    return;
                }
                statusFile = 1;
                p = 0;
                do
                {
                    scaner();
                    switch (syn)
                    {
                    case 11:
                        if (f == '-')
                        {
                            fprintf(fp, "(%d,-%s)\n", syn, number);
                            f = '\0';
                        }
                        else
                            fprintf(fp, "(%d,%s)\n", syn, number);
                        break;
                    case 30:
                        break;
                    case -1:
                        fprintf(fp, "erron\n");
                        break;
                    default:
                        fprintf(fp, "(%d,%s)\n", syn, token);
                    }
                } while (syn != 0);
                fclose(fp);
                break;
            }
            }*/
            FILE *fp;
            printf("请输入文件路径\n");
            scanf("%s", filename);
            freopen("filename", "a+", stdout);
            p = 0;
            do
            {
                scaner();
                switch (syn)
                {
                    case 11:
                        if (f == '-')
                        {
                            printf("\n(%d,-%s)", syn, number);
                            f = '\0';
                        }
                        else
                            printf("\n(%d,%s)", syn, number);
                        break;
                    case 30:
                        break;
                    case -1:
                        printf("error\n事故现场：%d\n", lines + 1);
                        status = 1;
                        break;
                    default:
                        printf("\n(%d,%s)", syn, token);
                        //printf("");
                }
            } while (syn != 0);
            break;
    }
}