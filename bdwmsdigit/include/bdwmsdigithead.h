#include <stdio.h>

/*****助记符的定义*****/
#define $SYMBOL	    1       //标识符
#define $CONSTANT	2       //常量

#define $INT 		3       //保留字
#define $IF 		4
#define $ELSE 		5
#define $WHILE 		6
#define $FOR 		7
#define $READ		8
#define $WRITE		9

#define $ADD		10      //运算符
#define $SUB 		11
#define $MUL		12
#define $DIV    	13
#define $L          14
#define $LE         15
#define $G          16
#define $GE         17
#define $NE         18
#define $E          19
#define $ASSIGN     20

#define $LPAR       21      //界符
#define $RPAR       22
#define $COM        23
#define $SEM        24

#define $EOLN		26
#define $EOF		27
#define $ERROR      28

#define MAX_TAB_SIZE 	50		//表的最大容量
#define MAX_BIT  	128		//最大位数
#define MAX_ID_LENGTH 	128		//常量的最长长度

// 符号表存储类型定义
typedef struct symTableItem{
	char name[MAX_ID_LENGTH];
	int  type;
	int  ln;
} symTableItem;

// 常量表存储类型定义
typedef struct numTableItem {
	int *binary;	//二进制（数组）
	int bit;		//二进制位数
} numTableItem;

// 保留字表
extern char *reserve_table[7];
extern int reserve_size;

// 符号表，不规则数组
extern symTableItem symbol_table[MAX_TAB_SIZE];
extern int symbol_size;

// 常量表
extern numTableItem constant_table[MAX_TAB_SIZE]; //二进制存储
extern int constant_size;

extern int line;	//记录行号

// 词法分析所用到的函数声明
char getnbc(FILE *sfp);
void concat(char character, char token[], int *n);
int digit(char character);
int letter(char character);
int retract(char character, char *back);
int reserve(char token[],int n);
int symbol(char token[],int n);
int constant(char token[], int n);
void error1(FILE *fp4, char character, int line);
int analyze(FILE *fp1, FILE *fp2, FILE *fp4, int *back_flag, char *back);
