// 多精度加法.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include<iostream>  
#include <math.h>
#include<string>  
using namespace std;
int main()
{
	string str1, str2,result;
	int a[130], b[130], len;   //数组的大小决定了计算的高精度最大位数  
	int i;
	memset(a, 0, sizeof(a));
	memset(b, 0, sizeof(b));
	cin >> str1 >> str2;   //输入两个字符串  
	a[0] = str1.length();  //取得第一个字符串的长度  
	for (i = 1; i <= a[0]; i++)  //把第一个字符串转换为整数，存放在数组a中  
		a[i] = str1[a[0] - i] - '0';//颠倒过来储存的，即低位放在下标小的位
	b[0] = str2.length();   //取得第二个字符串长度  
	for (i = 1; i <= b[0]; i++)   //把第二个字符串中的每一位转换为整数，存放在数组B中  
		b[i] = str2[b[0] - i] - '0';
	len = (a[0]>b[0] ? a[0] : b[0]);   //取两个字符串最大的长度  
	for (i = 1; i <= len; i++)   //做按位加法，同时处理进位  
	{
		a[i] += b[i];
		a[i + 1] += a[i] / 10;
		a[i] %= 10;
	}
	len++;      //可能最高位会进位
	while ((a[len] == 0) && (len>1)) len--;//下面是去掉最高位的0，然后输出。
	char ss[100];
	for (i = len; i >= 1; i--) {
		ss[len - i] = a[i]+'0';
		
	}
	result = ss;
	for (i = 0; i < len; i++)
		cout << result[i];
	return 0;
}

 


