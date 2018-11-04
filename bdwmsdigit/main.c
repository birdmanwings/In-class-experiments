#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "bdwmsdigithead.h"

numTableItem constant_table[MAX_TAB_SIZE];//常量表
int constant_size = 0;//常量表中存储数量

symTableItem symbol_table[MAX_TAB_SIZE];//标识符表
int symbol_size=0;//标识符表存储的数量

char *reserve_table[7]={"int","if","else","while","for","read","write"};//保留字表
int reserve_size=7;//保留字数量为7

int line = 1;

/*****************************************************************************
* 函数名称： getnbc
* 功能描述： 从文件中读入字符，检查变量是否为空白字符，若是，则读入下一个字符，直到读到
*			非空白字符为止
******************************************************************************/
char getnbc(FILE *fp1)
{
	char ch = fgetc(fp1);
	while (ch == ' ' || ch == '\t') ch = fgetc(fp1);	//略过空格和制表符
	return ch;
}

/*****************************************************************************
* 函数名称： concat
* 功能描述： 将character连接到token数组的末尾，然后再插入'\0'，并修改token的长度
******************************************************************************/
void concat(char character, char token[], int *n)
{
	token[*n] = character;
	(*n)++;
	token[*n] = '\0';
}

/*****************************************************************************
* 函数名称： digit
* 功能描述： 判断是否是数字，若是则返回1，否则返回0
******************************************************************************/
int digit(char character)
{
	return (character >= '0' && character <= '9');
}

/*****************************************************************************
* 函数名称： letter
* 功能描述： 判断是否是字母，若是则返回1，否则返回0
******************************************************************************/
int letter(char character)
{
    return ((character >= 'A' && character <='Z') || (character >= 'a' && character <= 'z'));
}

/*****************************************************************************
* 函数名称： retract
* 功能描述： 回退字符，若是非空白字符，将字符保存在back中；若是空白字符，不回退
******************************************************************************/
int retract(char character, char *back)
{
	if (character != ' ') {
		*back = character;
		return 1;
	}
	return 0;
}

/*****************************************************************************
* 函数名称： reserve
* 功能描述： 对token中的字符串查保留字表，若查到，返回该保留字的序号，否则返回0
******************************************************************************/

int reserve(char token[],int n)
{
    int i;
    for(i=0;i<reserve_size;i++){
        if(strcmp(token,reserve_table[i])==0)
            return i;
    }
    return 10;
}

/*****************************************************************************
* 函数名称： symbol
* 功能描述： 对token中的字符串查保标识符表，若查到，返回该标识符的序号，否则将
新的标识符插入表中，再返回序号
******************************************************************************/

int symbol(char token[],int n)
{
    int i,j;
    if(symbol_size==0){//当标识符表是空的时候，直接将token扫描的存入标识符表中
        for(i=0;i<n;i++){
            symbol_table[symbol_size].name[i]=token[i];
        }
        symbol_table[symbol_size].name[n]='\0';//末尾加\0
        symbol_table[symbol_size].ln=n;//存储长度
        symbol_table[symbol_size].type=$SYMBOL;//类型为$symbol
        symbol_size+=1;//标识符表数量加一
        return 0;
    }
    //不为空时进行扫描标识符表，判断是否在标识符表中，若在返回序号，不在就加入表中
    for(i=0;i<symbol_size;i++){
        if(strcmp(token,symbol_table[i].name)==0){
            return i;
        }
        else{
            for(j=0;j<n;j++){
        symbol_table[symbol_size].name[j]=token[j];
    }
    symbol_table[symbol_size].name[n]='\0';//末尾加\0
    symbol_table[symbol_size].ln=n;//存储长度
    symbol_table[symbol_size].type=$SYMBOL;//类型为$symbol
        }
    }
    symbol_size++;//标识符表数量加一
    return symbol_size;
}

/*****************************************************************************
* 函数名称： constant
* 功能描述： 对token中的字符串查常数表，若查到，返回该常数的序号，否则将新的常数插入
表中，再返回序号
******************************************************************************/
int constant(char token[], int n)
{
	int i, j, b;
	int dec, rem, tmp;
	int stack[MAX_BIT];//栈
	int top = 0;

	// 转换成十进制数
	for (i = 0, dec = 0; i < n; i++) {
		dec = dec * 10 + (token[i] - '0');
	}

	// 转换成二进制数，倒序放在栈中，高位放在栈底
	rem = dec;
	if (dec == 0) stack[top++] = 0;	//“0”单独处理
	while (rem > 0) {
		stack[top++] = rem % 2;
		rem = rem / 2;
	}

	tmp = top;	//暂存栈顶指针，代表二进制数的位数
	for (i = 0; i < constant_size; i++) {
		b = constant_table[i].bit;	//取二进制数的位数
		if (top != b) continue;		//位数不相等，直接跳过该数

		for (j = 0; j < b; j++) {	//逐位比较
			if (stack[--top] != constant_table[i].binary[j]) break;
		}
		if (j >= b) return i;	//找到相同的数，直接返回该数在表中的位置
		top = tmp;	//还原top
	}

	// 如果没有找到相同的数，就将该数录入表中
	i = constant_size++;
	constant_table[i].binary = (int *)malloc((top + 1)* sizeof(int));
	constant_table[i].bit = top;	//首位存储该二进制数的位数
	for (j = 0; top > 0; j++) {
		constant_table[i].binary[j] = stack[--top];   //顺序存储进数组中
	}
	return i;
}

/*****************************************************************************
* 函数名称： error1
* 功能描述： 打印出错的信息
******************************************************************************/
void error1(FILE *fp4, char character, int line)
{
	printf("\nerror: line %.2d: 含有非法字符:\t%c\n\n", line, character);
	fprintf(fp4, "error: line %.2d: 含有非法字符:\t%c \n", line, character);
}

/*****************************************************************************
* 函数名称： analyze
* 功能描述： 主词法分析函数，每调用一次获取一个单词，将其打印到屏幕并输出到文本
******************************************************************************/
int analyze(FILE *fp1, FILE *fp2, FILE *fp4, int *back_flag, char *back)
{
	char character;
	char token[16];
	int  n = 0;		//n记录token中的字符串长度
	int  type;		//单词类型
	int  val = 0;		//单词属性，只有标识符和常数不是0
    int  temp;

	if (*back_flag) {	//上一次有回退，*back_flag为1时是有回退
		character = *back;
		*back_flag = 0;
	}
	else character = getnbc(fp1);

	if (digit(character)) {	//是常数
		while (digit(character)) {//一直读取直到不是常数（因为第一个读取的是数字，肯定不是标识符，保留字等，肯定是个常量）
			concat(character, token, &n);
			character = fgetc(fp1);
		}
		val = constant(token, n);
		type = $CONSTANT;
		*back_flag = retract(character, back);
	}

	else if(letter(character)){//首字符是字母，肯定是标识符或者保留字
        while(digit(character) || letter(character)){
            concat(character, token, &n);
            character = fgetc(fp1);
        }
        if((temp=reserve(token,n)) == 10){//不是保留字，才进行标识符的判断
            val = symbol(token, n);
            type = $SYMBOL;
            *back_flag = retract(character, back);//此时character字符存储的是最后一次读取时的字符，其并不符合判断条件
        }                                         //如果不是空白字符就将其存储到*back中用于下一轮的扫描的第一个字符
        else{//进行保留字的识别
            val = reserve(token, n);
            type = val+3;
            *back_flag = retract(character, back);
        }
	}

	else {//首字符既不是数字，也不是字母
		switch (character) {
		case '+':	type = $ADD;	concat(character, token, &n);	break;
		case '-':	type = $SUB;	concat(character, token, &n);	break;
		case '*':	type = $MUL;	concat(character, token, &n);	break;
		case '/':	type = $DIV;	concat(character, token, &n);	break;
		case '<'://分析<,<=
		    {
		        concat(character, token, &n);
		        character=fgetc(fp1);
		        if(character=='='){
                    type = $LE;     concat(character, token, &n);
		        }
		        else{
                    type = $L;      concat(character, token, &n);
                    *back_flag = retract(character, back);//匹配=失败进行回退
		        }
		        break;
		    }
        case '>'://分析>,>=
            {
                concat(character, token, &n);
		        character=fgetc(fp1);
		        if(character=='='){
                    type = $GE;     concat(character, token, &n);
		        }
		        else{
                    type = $G;      concat(character, token, &n);
                    *back_flag = retract(character, back);//匹配=失败进行回退
		        }
		        break;
            }
        case '='://分析=,==
            {
                concat(character, token, &n);
		        character=fgetc(fp1);
		        if(character=='='){
                    type = $E;     concat(character, token, &n);
		        }
		        else{
                    type = $ASSIGN;      concat(character, token, &n);
                    *back_flag = retract(character, back);//匹配=失败进行回退
		        }
		        break;
            }
        case '!'://分析!=
            {
                concat(character, token, &n);
		        character=fgetc(fp1);
		        if(character=='='){
                    type = $E;     concat(character, token, &n);
		        }
		        else{
                    error1(fp4,'!',line);//匹配失败，错误输入!,将!打入fp4中
                    *back_flag = retract(character, back);//匹配=失败进行回退
		        }
		        break;
            }
        case '(':	type = $LPAR;	concat(character, token, &n);	break;
        case ')':	type = $RPAR;	concat(character, token, &n);	break;
        case ',':	type = $COM;	concat(character, token, &n);	break;
        case ';':	type = $SEM;	concat(character, token, &n);	break;
		case EOF:	type = $EOF;	concat('#', token, &n);	        break;
		case '\n':	type = $EOLN;	concat(character, token, &n);	break;
		default:    type = $ERROR;   error1(fp4, character, line);//注意在匹配失败的时候，要将type的值进行改变，否则还会执行一次上次的输出
		//return -1;
		}
	}

	// 在屏幕上打印并输出到文本
	if (type == $CONSTANT) {
		printf("line %.2d: (%.2d, %d) 常  数：%s\n", line, type, val, token);
		fprintf(fp2, "line %.2d: (%.2d, %d) 常  数：%s\n", line, type, val, token);
	}
	else if (type == $SYMBOL){
        printf("line %.2d: (%.2d, %d) 标识符：%s\n", line, type, val, token);
		fprintf(fp2, "line %.2d: (%.2d, %d) 标识符：%s\n", line, type, val, token);
	}
	else if (type >= 3 && type <= 9){
        printf("line %.2d: (%.2d, %d) 保留字：%s\n", line, type, val, token);
		fprintf(fp2, "line %.2d: (%.2d, %d) 保留字：%s\n", line, type, val, token);
	}
	else if (type >= 10 && type <= 20){
		printf("line %.2d: (%.2d, %d) 运算符：%s\n", line, type, val, token);
		fprintf(fp2, "line %.2d: (%.2d, %d) 运算符：%s\n", line, type, val, token);
	}
	else if (type >=21 && type <= 24){
        printf("line %.2d: (%.2d, %d) 界符：%s\n", line, type, val, token);
		fprintf(fp2, "line %.2d: (%.2d, %d) 界符：%s\n", line, type, val, token);
	}
	else if ( type == $EOLN){
		printf("line %.2d: (%.2d, %d) 行尾符：%s\n", line, type, val, token);
		fprintf(fp2, "line %.2d: (%.2d, %d) 行尾符：%s\n", line, type, val, token);
	}
	else if ( type == $EOF){
		printf("line %.2d: (%.2d, %d) 结尾符：%s\n", line, type, val, token);
		fprintf(fp2, "line %.2d: (%.2d, %d) 结尾符：%s\n", line, type, val, token);
	}
	if (type == $EOLN) line++; //换行

	return type;
}

int main(void)
{
	char *soure = "D:\\leapyear.txt";
	char *output = "D:\\output.txt";
	char *error = "D:\\error.txt";

	FILE *fp_src = fopen(soure, "r");//打开要扫描的文件
	if (fp_src == NULL) {
		printf("打开源文件失败！请确定源文件的位置！\n");
		return 0;
	}
	else{
        printf("打开源文件成功\n");
	}

	FILE *fp_output = fopen(output, "w");//打开输出的文件
	if (fp_output == NULL) {
		printf("打开output.txt文件失败！请确定源文件的位置！\n");
		return 0;
	}
	else{
        printf("打开output.txt成功\n");
	}

	FILE *fp_err = fopen(error, "w");//打开储存报错信息的文件
	if (fp_err == NULL) {
		printf("打开error.txt文件失败！请确定源文件的位置！\n");
		return 0;
	}
	else{
        printf("打开error.txt成功\n");
	}

	int back_flag = 0;	//回退的标记，1代表上一次有回退(空格不算)，0代表没有
	char back;		//存放上一次的回退值

	printf("李卓然-2017221304001\n\n");
	fprintf(fp_output  , "李卓然-2017221304001\n\n");

	while (analyze(fp_src, fp_output, fp_err, &back_flag, &back) != $EOF);

	fclose(fp_src); //关闭文件
    fclose(fp_output);
	fclose(fp_err);
	return 0;
}
