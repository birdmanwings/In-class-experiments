#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "bdwmsdigithead.h"

numTableItem constant_table[MAX_TAB_SIZE];//������
int constant_size = 0;//�������д洢����

symTableItem symbol_table[MAX_TAB_SIZE];//��ʶ����
int symbol_size=0;//��ʶ����洢������

char *reserve_table[7]={"int","if","else","while","for","read","write"};//�����ֱ�
int reserve_size=7;//����������Ϊ7

int line = 1;

/*****************************************************************************
* �������ƣ� getnbc
* ���������� ���ļ��ж����ַ����������Ƿ�Ϊ�հ��ַ������ǣ��������һ���ַ���ֱ������
*			�ǿհ��ַ�Ϊֹ
******************************************************************************/
char getnbc(FILE *fp1)
{
	char ch = fgetc(fp1);
	while (ch == ' ' || ch == '\t') ch = fgetc(fp1);	//�Թ��ո���Ʊ��
	return ch;
}

/*****************************************************************************
* �������ƣ� concat
* ���������� ��character���ӵ�token�����ĩβ��Ȼ���ٲ���'\0'�����޸�token�ĳ���
******************************************************************************/
void concat(char character, char token[], int *n)
{
	token[*n] = character;
	(*n)++;
	token[*n] = '\0';
}

/*****************************************************************************
* �������ƣ� digit
* ���������� �ж��Ƿ������֣������򷵻�1�����򷵻�0
******************************************************************************/
int digit(char character)
{
	return (character >= '0' && character <= '9');
}

/*****************************************************************************
* �������ƣ� letter
* ���������� �ж��Ƿ�����ĸ�������򷵻�1�����򷵻�0
******************************************************************************/
int letter(char character)
{
    return ((character >= 'A' && character <='Z') || (character >= 'a' && character <= 'z'));
}

/*****************************************************************************
* �������ƣ� retract
* ���������� �����ַ������Ƿǿհ��ַ������ַ�������back�У����ǿհ��ַ���������
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
* �������ƣ� reserve
* ���������� ��token�е��ַ����鱣���ֱ����鵽�����ظñ����ֵ���ţ����򷵻�0
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
* �������ƣ� symbol
* ���������� ��token�е��ַ����鱣��ʶ�������鵽�����ظñ�ʶ������ţ�����
�µı�ʶ��������У��ٷ������
******************************************************************************/

int symbol(char token[],int n)
{
    int i,j;
    if(symbol_size==0){//����ʶ�����ǿյ�ʱ��ֱ�ӽ�tokenɨ��Ĵ����ʶ������
        for(i=0;i<n;i++){
            symbol_table[symbol_size].name[i]=token[i];
        }
        symbol_table[symbol_size].name[n]='\0';//ĩβ��\0
        symbol_table[symbol_size].ln=n;//�洢����
        symbol_table[symbol_size].type=$SYMBOL;//����Ϊ$symbol
        symbol_size+=1;//��ʶ����������һ
        return 0;
    }
    //��Ϊ��ʱ����ɨ���ʶ�����ж��Ƿ��ڱ�ʶ�����У����ڷ�����ţ����ھͼ������
    for(i=0;i<symbol_size;i++){
        if(strcmp(token,symbol_table[i].name)==0){
            return i;
        }
        else{
            for(j=0;j<n;j++){
        symbol_table[symbol_size].name[j]=token[j];
    }
    symbol_table[symbol_size].name[n]='\0';//ĩβ��\0
    symbol_table[symbol_size].ln=n;//�洢����
    symbol_table[symbol_size].type=$SYMBOL;//����Ϊ$symbol
        }
    }
    symbol_size++;//��ʶ����������һ
    return symbol_size;
}

/*****************************************************************************
* �������ƣ� constant
* ���������� ��token�е��ַ����鳣�������鵽�����ظó�������ţ������µĳ�������
���У��ٷ������
******************************************************************************/
int constant(char token[], int n)
{
	int i, j, b;
	int dec, rem, tmp;
	int stack[MAX_BIT];//ջ
	int top = 0;

	// ת����ʮ������
	for (i = 0, dec = 0; i < n; i++) {
		dec = dec * 10 + (token[i] - '0');
	}

	// ת���ɶ����������������ջ�У���λ����ջ��
	rem = dec;
	if (dec == 0) stack[top++] = 0;	//��0����������
	while (rem > 0) {
		stack[top++] = rem % 2;
		rem = rem / 2;
	}

	tmp = top;	//�ݴ�ջ��ָ�룬�������������λ��
	for (i = 0; i < constant_size; i++) {
		b = constant_table[i].bit;	//ȡ����������λ��
		if (top != b) continue;		//λ������ȣ�ֱ����������

		for (j = 0; j < b; j++) {	//��λ�Ƚ�
			if (stack[--top] != constant_table[i].binary[j]) break;
		}
		if (j >= b) return i;	//�ҵ���ͬ������ֱ�ӷ��ظ����ڱ��е�λ��
		top = tmp;	//��ԭtop
	}

	// ���û���ҵ���ͬ�������ͽ�����¼�����
	i = constant_size++;
	constant_table[i].binary = (int *)malloc((top + 1)* sizeof(int));
	constant_table[i].bit = top;	//��λ�洢�ö���������λ��
	for (j = 0; top > 0; j++) {
		constant_table[i].binary[j] = stack[--top];   //˳��洢��������
	}
	return i;
}

/*****************************************************************************
* �������ƣ� error1
* ���������� ��ӡ�������Ϣ
******************************************************************************/
void error1(FILE *fp4, char character, int line)
{
	printf("\nerror: line %.2d: ���зǷ��ַ�:\t%c\n\n", line, character);
	fprintf(fp4, "error: line %.2d: ���зǷ��ַ�:\t%c \n", line, character);
}

/*****************************************************************************
* �������ƣ� analyze
* ���������� ���ʷ�����������ÿ����һ�λ�ȡһ�����ʣ������ӡ����Ļ��������ı�
******************************************************************************/
int analyze(FILE *fp1, FILE *fp2, FILE *fp4, int *back_flag, char *back)
{
	char character;
	char token[16];
	int  n = 0;		//n��¼token�е��ַ�������
	int  type;		//��������
	int  val = 0;		//�������ԣ�ֻ�б�ʶ���ͳ�������0
    int  temp;

	if (*back_flag) {	//��һ���л��ˣ�*back_flagΪ1ʱ���л���
		character = *back;
		*back_flag = 0;
	}
	else character = getnbc(fp1);

	if (digit(character)) {	//�ǳ���
		while (digit(character)) {//һֱ��ȡֱ�����ǳ�������Ϊ��һ����ȡ�������֣��϶����Ǳ�ʶ���������ֵȣ��϶��Ǹ�������
			concat(character, token, &n);
			character = fgetc(fp1);
		}
		val = constant(token, n);
		type = $CONSTANT;
		*back_flag = retract(character, back);
	}

	else if(letter(character)){//���ַ�����ĸ���϶��Ǳ�ʶ�����߱�����
        while(digit(character) || letter(character)){
            concat(character, token, &n);
            character = fgetc(fp1);
        }
        if((temp=reserve(token,n)) == 10){//���Ǳ����֣��Ž��б�ʶ�����ж�
            val = symbol(token, n);
            type = $SYMBOL;
            *back_flag = retract(character, back);//��ʱcharacter�ַ��洢�������һ�ζ�ȡʱ���ַ����䲢�������ж�����
        }                                         //������ǿհ��ַ��ͽ���洢��*back��������һ�ֵ�ɨ��ĵ�һ���ַ�
        else{//���б����ֵ�ʶ��
            val = reserve(token, n);
            type = val+3;
            *back_flag = retract(character, back);
        }
	}

	else {//���ַ��Ȳ������֣�Ҳ������ĸ
		switch (character) {
		case '+':	type = $ADD;	concat(character, token, &n);	break;
		case '-':	type = $SUB;	concat(character, token, &n);	break;
		case '*':	type = $MUL;	concat(character, token, &n);	break;
		case '/':	type = $DIV;	concat(character, token, &n);	break;
		case '<'://����<,<=
		    {
		        concat(character, token, &n);
		        character=fgetc(fp1);
		        if(character=='='){
                    type = $LE;     concat(character, token, &n);
		        }
		        else{
                    type = $L;      concat(character, token, &n);
                    *back_flag = retract(character, back);//ƥ��=ʧ�ܽ��л���
		        }
		        break;
		    }
        case '>'://����>,>=
            {
                concat(character, token, &n);
		        character=fgetc(fp1);
		        if(character=='='){
                    type = $GE;     concat(character, token, &n);
		        }
		        else{
                    type = $G;      concat(character, token, &n);
                    *back_flag = retract(character, back);//ƥ��=ʧ�ܽ��л���
		        }
		        break;
            }
        case '='://����=,==
            {
                concat(character, token, &n);
		        character=fgetc(fp1);
		        if(character=='='){
                    type = $E;     concat(character, token, &n);
		        }
		        else{
                    type = $ASSIGN;      concat(character, token, &n);
                    *back_flag = retract(character, back);//ƥ��=ʧ�ܽ��л���
		        }
		        break;
            }
        case '!'://����!=
            {
                concat(character, token, &n);
		        character=fgetc(fp1);
		        if(character=='='){
                    type = $E;     concat(character, token, &n);
		        }
		        else{
                    error1(fp4,'!',line);//ƥ��ʧ�ܣ���������!,��!����fp4��
                    *back_flag = retract(character, back);//ƥ��=ʧ�ܽ��л���
		        }
		        break;
            }
        case '(':	type = $LPAR;	concat(character, token, &n);	break;
        case ')':	type = $RPAR;	concat(character, token, &n);	break;
        case ',':	type = $COM;	concat(character, token, &n);	break;
        case ';':	type = $SEM;	concat(character, token, &n);	break;
		case EOF:	type = $EOF;	concat('#', token, &n);	        break;
		case '\n':	type = $EOLN;	concat(character, token, &n);	break;
		default:    type = $ERROR;   error1(fp4, character, line);//ע����ƥ��ʧ�ܵ�ʱ��Ҫ��type��ֵ���иı䣬���򻹻�ִ��һ���ϴε����
		//return -1;
		}
	}

	// ����Ļ�ϴ�ӡ��������ı�
	if (type == $CONSTANT) {
		printf("line %.2d: (%.2d, %d) ��  ����%s\n", line, type, val, token);
		fprintf(fp2, "line %.2d: (%.2d, %d) ��  ����%s\n", line, type, val, token);
	}
	else if (type == $SYMBOL){
        printf("line %.2d: (%.2d, %d) ��ʶ����%s\n", line, type, val, token);
		fprintf(fp2, "line %.2d: (%.2d, %d) ��ʶ����%s\n", line, type, val, token);
	}
	else if (type >= 3 && type <= 9){
        printf("line %.2d: (%.2d, %d) �����֣�%s\n", line, type, val, token);
		fprintf(fp2, "line %.2d: (%.2d, %d) �����֣�%s\n", line, type, val, token);
	}
	else if (type >= 10 && type <= 20){
		printf("line %.2d: (%.2d, %d) �������%s\n", line, type, val, token);
		fprintf(fp2, "line %.2d: (%.2d, %d) �������%s\n", line, type, val, token);
	}
	else if (type >=21 && type <= 24){
        printf("line %.2d: (%.2d, %d) �����%s\n", line, type, val, token);
		fprintf(fp2, "line %.2d: (%.2d, %d) �����%s\n", line, type, val, token);
	}
	else if ( type == $EOLN){
		printf("line %.2d: (%.2d, %d) ��β����%s\n", line, type, val, token);
		fprintf(fp2, "line %.2d: (%.2d, %d) ��β����%s\n", line, type, val, token);
	}
	else if ( type == $EOF){
		printf("line %.2d: (%.2d, %d) ��β����%s\n", line, type, val, token);
		fprintf(fp2, "line %.2d: (%.2d, %d) ��β����%s\n", line, type, val, token);
	}
	if (type == $EOLN) line++; //����

	return type;
}

int main(void)
{
	char *soure = "D:\\leapyear.txt";
	char *output = "D:\\output.txt";
	char *error = "D:\\error.txt";

	FILE *fp_src = fopen(soure, "r");//��Ҫɨ����ļ�
	if (fp_src == NULL) {
		printf("��Դ�ļ�ʧ�ܣ���ȷ��Դ�ļ���λ�ã�\n");
		return 0;
	}
	else{
        printf("��Դ�ļ��ɹ�\n");
	}

	FILE *fp_output = fopen(output, "w");//��������ļ�
	if (fp_output == NULL) {
		printf("��output.txt�ļ�ʧ�ܣ���ȷ��Դ�ļ���λ�ã�\n");
		return 0;
	}
	else{
        printf("��output.txt�ɹ�\n");
	}

	FILE *fp_err = fopen(error, "w");//�򿪴��汨����Ϣ���ļ�
	if (fp_err == NULL) {
		printf("��error.txt�ļ�ʧ�ܣ���ȷ��Դ�ļ���λ�ã�\n");
		return 0;
	}
	else{
        printf("��error.txt�ɹ�\n");
	}

	int back_flag = 0;	//���˵ı�ǣ�1������һ���л���(�ո���)��0����û��
	char back;		//�����һ�εĻ���ֵ

	printf("��׿Ȼ-2017221304001\n\n");
	fprintf(fp_output  , "��׿Ȼ-2017221304001\n\n");

	while (analyze(fp_src, fp_output, fp_err, &back_flag, &back) != $EOF);

	fclose(fp_src); //�ر��ļ�
    fclose(fp_output);
	fclose(fp_err);
	return 0;
}
