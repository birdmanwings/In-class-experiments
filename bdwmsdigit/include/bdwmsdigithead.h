#include <stdio.h>

/*****���Ƿ��Ķ���*****/
#define $SYMBOL	    1       //��ʶ��
#define $CONSTANT	2       //����

#define $INT 		3       //������
#define $IF 		4
#define $ELSE 		5
#define $WHILE 		6
#define $FOR 		7
#define $READ		8
#define $WRITE		9

#define $ADD		10      //�����
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

#define $LPAR       21      //���
#define $RPAR       22
#define $COM        23
#define $SEM        24

#define $EOLN		26
#define $EOF		27
#define $ERROR      28

#define MAX_TAB_SIZE 	50		//����������
#define MAX_BIT  	128		//���λ��
#define MAX_ID_LENGTH 	128		//�����������

// ���ű�洢���Ͷ���
typedef struct symTableItem{
	char name[MAX_ID_LENGTH];
	int  type;
	int  ln;
} symTableItem;

// ������洢���Ͷ���
typedef struct numTableItem {
	int *binary;	//�����ƣ����飩
	int bit;		//������λ��
} numTableItem;

// �����ֱ�
extern char *reserve_table[7];
extern int reserve_size;

// ���ű�����������
extern symTableItem symbol_table[MAX_TAB_SIZE];
extern int symbol_size;

// ������
extern numTableItem constant_table[MAX_TAB_SIZE]; //�����ƴ洢
extern int constant_size;

extern int line;	//��¼�к�

// �ʷ��������õ��ĺ�������
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
