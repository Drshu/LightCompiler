#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include "scaner.h"
#include "parser.h"

extern int status;
extern char filename[FILENAME_MAX];
extern int statusFile;

int main()
{
    morphologyAnalysis();//词法分析
    if (statusFile != 0)
        freopen("/Users/Shu/ClionProjects/LightCompiler/output.txt", "a+", stdout);
    if (status == 0)
        recursiveDescentParser();//语法分析
    else
        printf("词法分析有错，不进行语法分析\n");
    return 0;
}







