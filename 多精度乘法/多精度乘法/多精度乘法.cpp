// 多精度乘法.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include<iostream>  
#include<string>  
using namespace std;
int main()
{
	string str1, str2;
	int a[250], b[250], c[500], len;    //a,b数组用来储存读入的两个数，c数组用来储存结果
	int i, j;
	memset(a, 0, sizeof(a));
	memset(b, 0, sizeof(b));
	cin >> str1 >> str2;
	a[0] = str1.length();
	for (i = 1; i <= a[0]; i++)
		a[i] = str1[a[0] - i] - '0';
	b[0] = str2.length();
	for (i = 1; i <= b[0]; i++)
		b[i] = str2[b[0] - i] - '0';
	memset(c, 0, sizeof(c));
	for (i = 1; i <= a[0]; i++)   //做按位乘法同时处理进位，注意循环内语句的写法。  
		for (j = 1; j <= b[0]; j++)
		{
			c[i + j - 1] += a[i] * b[j];		//放置单独两位的乘法结果
			c[i + j] += c[i + j - 1] / 10;		//进位
			c[i + j - 1] %= 10;					//本位
		}
	len = a[0] + b[0] + 1;  //去掉最高位的0，然后输出  
	while ((c[len] == 0) && (len>1)) len--;     
	for (i = len; i >= 1; i--)
		cout << c[i];
	return 0;
}
