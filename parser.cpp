#define _CRT_SECURE_NO_DEPRECATE
#include "parser.h"

#include "scaner.h"

#define FILENAME_MAX_ 30
extern char prog[800], token[20], number[50];
extern char s[1024];
extern char ch, f, f1;
extern int syn, p, m, n;
extern int flag;
extern char* rwtab[6];
extern char* rwtabS[6];
extern char filename[FILENAME_MAX];
extern int lines;
extern int statusFile;
extern char filename[FILENAME_MAX];
bool haveEnter = false;//查看分割状态，true为有回车
int status = 0;//识别整体的正确状态
int tempSyn = 0;//处理在换行符之后的标示符的种别码
void printSyn() {
    if (status == 0)
    {
        switch (syn) {
            case 11: printf("(%d, %s)\n", syn, number);
                break;
            case -1: printf("error\n事故现场：%d\n", lines + 1);
                status = 1;
                break;
            case  0:
                break;
            default: printf("(%d, %s)\n", syn, token);
                break;
        }
    }
    else
    {
        FILE *fp;
        if ((fp = fopen(filename, "w")) == NULL)
        {
            printf("文件打开失败\n");
            return;
        }
        switch (syn) {
            case 11: fprintf(fp,"(%d, %s)\n", syn, number);
                break;
            case -1: fprintf(fp,"error\n事故现场：%d\n", lines + 1);
                status = 1;
                break;
            case  0:
                break;
            default: fprintf(fp,"(%d, %s)\n", syn, token);
                break;
        }
    }
}

void statement() {

    if (syn == 10)//普通标识符
    {
        secondScaner();
        if (syn == 18)
        {
            printf("出现了“=”，本句为赋值语句\n");
            secondScaner();
            expressionAnalysis();//分析整个表达式
        }
        else if (syn == 2)
        {//分析到if的时候
            printf("分析到“%s”!继续之后的分析\n", rwtab[syn - 1]);
            secondScaner();
            jugdeAnalysis();
        }
        else if (syn >= 20 && syn <= 25) {
            printf("你这是错的，这是一个赋值语句，不是判断句！事故现场：%d\n", lines + 1);
            status = 1;
            secondScaner();
            expressionAnalysis();
        }
        else
        {
            printf("出现错误啦！你这个表达式是错的！没有'='号，事故现场：%d\n", lines);
            status = 1;
            haveEnter = false;
            //statement();

            expressionAnalysis();//分析整个表达式
            //secondScaner();
        }
    }
    else
    {
        if (syn == 6)//遇到endfunc
        {
            return;
        }
        else
        if (syn == 2)
        {//分析到除了function和endfunc之外的其它关键字
            printf("分析到“%s”!继续之后的分析\n", rwtab[syn - 1]);
            secondScaner();
            haveEnter = false;
            jugdeAnalysis();
        }
        else if (syn == 0)
        {
            return;
        }
        else {//分析错误
            printf("出现错误啦！你这个语句是错的！！事故现场：%d\n", lines + 1);
            secondScaner();
            status = 1;
        }
    }
}

void factor() {
    if (syn == 11 || syn == 10)
    {
        secondScaner();
        if (haveEnter == true && syn != 25) {
            tempSyn = syn;
            status = 1;
            syn = 26;
            return;
        }if (syn !=26)
        {
            //printSyn();
        }
        if (syn ==11 && syn == 10)
        {
            printf("缺少运算符，事故现场：%d\n", lines);
            secondScaner();
            //expressionAnalysis();
        }
        if (syn == 25)//当出现‘==’的情况
        {
            secondScaner();
            expressionAnalysis();
            //return;
        }
        if (syn >=20 && syn <= 24)
        {
            secondScaner();
            jugdeAnalysis();
        }
    }
    else
    if (syn == 27 )//出现‘（’
    {

        secondScaner();
        expressionAnalysis();
// 				if (right > left)//处理少左括号的情况
// 				{
// 					printf("出现错误，少一个左括号！事故现场:%d\n", lines + 1);
// 					while (ch == '\n')
// 					{
// 						secondScaner();
// 					}
// 				}
        if (syn == 11)
        {
            printf("缺少操作符,事故现场：%d\n",lines+1);
            //expressionAnalysis();
            secondScaner();
        }

        if (syn == 28)//出现‘）’
        {

            secondScaner();
            //scaner();
            if (haveEnter == true)//判断是否有换行符
            {
                tempSyn = syn;
                status = 1;
                syn = 26;
                //return;
            }
            //printSyn();

        }


        else
        {
            printf("出现错误！未能找到右括号~事故现场：%d\n", lines + 1);
            status = 1;
        }




    }
    else
    {
        printf("出现错误！表达式错了~缺少操作数！事故现场：%d\n", lines + 1);
        status = 1;
    }
}


void  sentenceAnalysis() {//语句分析
    statement();
    while (syn==26)//出现分号，一个语句识别结束，继续之后的识别
    {
        //处理没有分号的情况
        if (haveEnter == true)
        {
            printf("语法分析错误，你忘记了一个分号。是第%d句\n", lines-1);
            haveEnter = false;//重置缺少分号的状态
            syn = tempSyn;
            //printSyn();
            statement();
        }
        else
        {
            printf("Nice!我找到了一个分号，这个语句分析结束！\n");
            secondScaner();
            haveEnter = false;
            statement();
        }
    }
    return;
}

void expressionAnalysis() {//分析表达式
    termAnalysis();
    if (syn >= 20 && syn <= 25) {
        printf("你这是错的，这是一个赋值语句，不是判断句！事故现场：%d\n", lines+1);
        status = 1;
    }
//	if (syn == 28 && left == right) {
    //	printf("出现错误，丢失左括号！\n");
    //	while (syn == 26)
    //	{
    //		secondScaner();
    //	}
    //}
    while (syn == 13 || syn == 14)//出现‘+’和‘-’
    {
        secondScaner();
        factor();
    }

    while (syn == 15 || syn == 16)//读取到乘除法
    {
        secondScaner();
        factor();
    }


}

void jugdeAnalysis() {
    termAnalysis();
    if (syn == 18)
    {
        printf("这是一个判断句，你这成赋值语句了！事故现场：%d\n", lines + 1);
        status = 1;
    }
    if  (syn >= 20 && syn <= 25) {
        secondScaner();
        factor();
    }
}


//分析每一个项
void termAnalysis() {
    factor();

    while (syn == 13 || syn == 14)//出现‘+’和‘-’
    {
        secondScaner();
        factor();
    }
    return;
}



void secondScaner()
{

    scaner();
    while (syn == 30)
    {
        scaner();
    }//处理有空格和换行符等情况
    //printSyn();
}

void recursiveDescentParser() {//递归下降分析法
    printf("\n\n好的，开始语法分析啦！\n");
    if (statusFile != 0)
        freopen("/Users/Shu/ClionProjects/LightCompiler/output.txt", "a+", stdout);
    p = 0;
    status = 0;
    lines = 0;
    secondScaner();

    if (syn == 1)
    {
        printf("我看到开始符号‘function’了！你这看起来好像是对的！\n");
        secondScaner();
        firstScaner();
    }
    else
    {
        printf("语法分析错误，没有开始符号 function,事故现场：%d\n",lines+1);
        status = 1;
        //statement();
        secondScaner();
        if (syn ==18)
        {
            p = 0;
            secondScaner();
            firstScaner();
        }
        else
            firstScaner();

    }

    if (status ==1)
    {
        printf("这段程序有问题啊！以后你要是去写飞机的程序，我们可不敢坐！\n");
    }
    else
    {
        printf("嗯~~~你这是对的！句法没有问题，很好！\n");
    }

}

void firstScaner() {//完成递归下降分析法的第一部，分析语句串
    //secondScaner();

    haveEnter = false;
    sentenceAnalysis();//语句分析

    if (syn == 6)
    {
        secondScaner();
        if (syn == 0)
        {
            printf("发现了'#'\n");
            printf("语法分析完成！\n");
        }
    }
    else
    {
        if (status == 1)
        {
            printf("语法分析错误！没有endfunc，事故现场：%d\n", lines);
            printf("语法分析完成！你多少号呢？\n");
            status = 1;
        }
    }
}