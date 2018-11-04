// 拓展的欧几里得算法（支持大数运算）.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include<iostream>  
#include<string>  
using namespace std;
/*输入都为带符号数，用空格分割两次输入，最大为128位*/

int compare1(string s1, string s2)  //比较字符串（两个数）数字的大小，大于等于返回0，小于返回1。  
{
	if (s1.length()>s2.length()) return 0;  //先比较长度，哪个字符串长，对应的那个数就大  
	if (s1.length()<s2.length()) return 1;
	for (int i = 1; i <= s1.length(); i++)  //长度相同时，就一位一位比较。  
	{
		if (s1[i]>s2[i]) return 0;
		if (s1[i]<s2[i]) return 1;
	}
	return 0;   //如果长度相同，每一位也一样，就返回0，说明相等  
}

string Addition(string str1,string str2)	//高精度加法
{
	int a[130], b[130], len;				//数组的大小决定了计算的高精度最大位数  
	int i;
	char ss[130];							//声明一个char数组用来储存去0正序化后的结果
	string result;							//声明result这个字符串用于返回结果
	memset(a, 0, sizeof(a));
	memset(b, 0, sizeof(b));
	a[0] = str1.length();					//取得第一个字符串的长度  
	for (i = 1; i < a[0]; i++)				//把第一个字符串转换为整数，存放在数组a中  
		a[i] = str1[a[0] - i] - '0';		//颠倒过来储存的注意
	b[0] = str2.length();					//取得第二个字符串长度  
	for (i = 1; i < b[0]; i++)				//把第二个字符串中的每一位转换为整数，存放在数组B中  
		b[i] = str2[b[0] - i] - '0';
	len = (a[0]>b[0] ? a[0] : b[0])-1;		//取两个字符串最大的长度  
	for (i = 1; i <= len; i++)				//做按位加法，同时处理进位  
	{
		a[i] += b[i];
		a[i + 1] += a[i] / 10;
		a[i] %= 10;
	}
	len++;      //可能最高位会进位
	while ((a[len] == 0) && (len>1)) len--; //下面是去掉最高位的0，然后输出。
	for (i = len; i >= 1; i--) {
		ss[len - i+1] = a[i] + '0';
	}
	if (str1[0] == '+')						//判断正负，获得结果的符号
		ss[0] = '+';
	else
		ss[0] = '-';
	ss[len+1] = '\0';						//在数组结尾加一个结束符
	result = ss;							//c++中string类型重载了=，可以直接将char数组转换为string类型
	return result;
}

string Subtraction(string str1, string str2) //高精度减法
{
	string result;
	char ss[130];
	int a[130], b[130], len;
	int i;
	memset(a, 0, sizeof(a));
	memset(b, 0, sizeof(b));
	a[0] = str1.length();
	for (i = 1; i < a[0]; i++)
		a[i] = str1[a[0] - i] - '0';
	b[0] = str2.length();
	for (i = 1; i < b[0]; i++)
		b[i] = str2[b[0] - i] - '0';
	if ((compare1(str1, str2)) == 0)  //大于等于，做按位减，并处理借位。  
	{
		for (i = 1; i < a[0]; i++)
		{
			a[i] -= b[i];
			if (a[i]<0) { a[i + 1]--; a[i] += 10; }
		}
		a[0]++;
		while ((a[a[0]-1] == 0) && (a[0]>1)) a[0]--;	//去掉前面减掉后剩下的0，直接从有效位开始输出
		for (i = a[0]; i >= 1; i--)
			ss[a[0] - i+1] = a[i-1] + '0';
		if (str1[0] == '+')					//判断结果的符号
			ss[0] = '+';
		else
			ss[0] = '-';
		ss[a[0]] = '\0';
		result = ss;
		return result;
	}
	else
	{
		if (str1[0] == '+')				//判断结果的正负号
			ss[0] = '-';
		else
			ss[0] = '+';
		for (i = 1; i < b[0]; i++)  //做按位减，大的减小的  
		{
			b[i] -= a[i];
			if (b[i]<0) { b[i + 1]--; b[i] += 10; }
		}
		b[0]++;
		while ((b[b[0]-1] == 0) && (b[0]>1)) b[0]--;
		for (i = b[0]; i >= 1; i--)
			ss[b[0] - i + 1] = b[i-1] + '0';
		ss[b[0]] = '\0';
		result = ss;
		return result;
	}
}

string Multiplication(string str1,string str2)								//多精度乘法
{
	int a[130], b[130], c[260], len;    //a,b数组用来储存读入的两个数，c数组用来储存结果
	int i, j;
	char ss[260];
	string result;
	memset(a, 0, sizeof(a));
	memset(b, 0, sizeof(b));
	a[0] = str1.length();//a[0]存储的是带符号的str1的长度
	for (i = 1; i < a[0]; i++)
		a[i] = str1[a[0] - i] - '0';
	b[0] = str2.length();//b[0]存储的是带符号的str2的长度
	for (i = 1; i < b[0]; i++)
		b[i] = str2[b[0] - i] - '0';
	memset(c, 0, sizeof(c));
	for (i = 1; i < a[0]; i++)   //做按位乘法同时处理进位，注意循环内语句的写法。  
		for (j = 1; j < b[0]; j++)
		{
			c[i + j - 1] += a[i] * b[j];		//放置单独两位的乘法结果
			c[i + j] += c[i + j - 1] / 10;		//进位
			c[i + j - 1] %= 10;					//本位
		}
	len = a[0]-1 + b[0]-1 + 1;  //去掉最高位的0，然后输出  
	while ((c[len] == 0) && (len>1)) len--;
	for (i = len; i >= 1; i--)
		ss[len - i + 1] = c[i] + '0';
	if (str1[0] == str2[0]) {	//判断结果的正负号
		ss[0] = '+';
	}
	else {
		ss[0] = '-';
	}
	ss[len+1] = '\0';
	result = ss;
	return result;
}

int compare(int a[], int b[])//用于高精度除法的比较a、b，若a>b为1；若a<b为-1；若a=b为0
{
	int i;
	if (a[0]>b[0])
		return 1;
	if (a[0]<b[0])
		return -1;
	for (i = a[0]; i>0; i--)//从高位到低位比较
	{
		if (a[i]>b[i])
			return 1;
		if (a[i]<b[i])
			return -1;
	}
	return 0;
}

void subduction(int a[], int b[])//用于高精度除法的计算a=a-b
{
	int flag;
	int i;

	flag = compare(a, b);
	if (flag == 0)//相等
	{
		a[0] = 0;
		return;
	}
	if (flag == 1)//大于
	{
		for (i = 1; i <= a[0]; i++)
		{
			if (a[i]<b[i])//若不够向上借位
			{
				a[i + 1]--;
				a[i] += 10;
			}
			a[i] -= b[i];
		}
		while (a[0]>0 && a[a[0]] == 0)//删除前导0
			a[0]--;
		return;
	}
}
string Division(string str1, string str2)//高精度除法，返回值为商
{
	int i, j;
	string result;
	char ss[100];
	int a[130], b[130], c[130];
	memset(a, 0, sizeof(a));
	memset(b, 0, sizeof(b));
	memset(c, 0, sizeof(c));

	a[0] = str1.length() - 1;//a[0]存储串1的位数
	b[0] = str2.length() - 1;//b[0]存储串2的位数
	for (i = 1; i <= a[0]; i++)
		a[i] = str1[a[0] - i + 1] - '0';
	for (i = 1; i <= b[0]; i++)
		b[i] = str2[b[0] - i + 1] - '0';


	int temp[100];
	c[0] = a[0] - b[0] + 1;
	for (i = c[0]; i>0; i--)	//从高位开始，进行试减
	{
		memset(temp, 0, sizeof(temp));	//中间量清零

		for (j = 1; j <= b[0]; j++)//从i开始的地方，复制数组b到数组temp
			temp[j + i - 1] = b[j];
		temp[0] = b[0] + i - 1;		//temp[0]存储其temp长度

		while (compare(a, temp) >= 0)//用减法模拟，如果a>temp
		{
			c[i]++;					//加1，尝试
			subduction(a, temp);	//作a-temp赋值给a
		}
	}

	while (c[0]>0 && c[c[0]] == 0)//删除前面多余的0
		c[0]--;

	if (c[0] == 0) {//输出结果
		ss[0] = '+';
		ss[1] = '0';
		ss[2] = '\0';
		result = ss;
		return result;
	}
	else
	{
		for (i = c[0]; i > 0; i--)
			ss[c[0] - i + 1] = c[i] + '0';
		ss[0] = '+';
		ss[c[0] + 1] = '\0';
		result = ss;
		return result;
	}
}

void Copy(string *str1, string str2) {	//用于string的复制
	char temp[130];
	int len;
	len = str2.length();
	for (int i = 0; i < len; i++) {
		temp[i] = str2[i];
	}
	temp[len] = '\0';
	*str1 = temp;
}

void Ex_gcd(string a, string b) {			//拓展的欧几里得
	string d,x,y,x1,x2,y1,y2,q,r;
	if (b[1] == '0') {
		Copy(&d,a);
		x = "+1\0";
		y = "+0\0";
		cout << d << endl;
		cout << x << endl;
		cout << y << endl;
	}
	else {
		x2 = "+1\0";
		x1 = "+0\0";
		y2 = "+0\0";
		y1 = "+1\0";
		while (b[1] > '0') {
			Copy(&q, Division(a, b));
			Copy(&r, Subtraction(a, Multiplication(q, b)));
			if (x2[0] == x1[0]) {
				Copy(&x, Subtraction(x2, Multiplication(q, x1)));
			}
			else {
				Copy(&x, Addition(x2, Multiplication(q, x1)));
			}
			if (y2[0] == y1[0]) {
				Copy(&y, Subtraction(y2, Multiplication(q, y1)));
			}
			else {
				Copy(&y, Addition(y2, Multiplication(q, y1)));
			}

			Copy(&a, b);
			Copy(&b, r);
			Copy(&x2, x1);
			Copy(&x1, x);
			Copy(&y2, y1);
			Copy(&y1, y);
		}
		Copy(&d, a);
		Copy(&x, x2);
		Copy(&y, y2);
		cout << d << endl;
		cout << x << endl;
		cout << y << endl;
	}
}

int main() {
	string str1, str2;		//统一用string类型来操作
	cin >> str1 >> str2;	//输入两个字符串 
	Ex_gcd(str1, str2);
}