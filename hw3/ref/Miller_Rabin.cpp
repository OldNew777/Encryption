#include<cstdio>
#include<cstring>
#include<algorithm>
using namespace std;


int prime[10] = { 2,3,5,7,11,13,17,19,23,29 };
int Quick_Multiply(int a, int b, int c)  //���ٻ����Ϳ����ݲ�ࣩ 
{
	long long ans = 0, res = a;
	while (b)
	{
		if (b & 1)
			ans = (ans + res) % c;
		res = (res + res) % c;
		b >>= 1;
	}
	return (int)ans;
}
int Quick_Power(int a, int b, int c)     //�����ݣ�����Ͳ�׸���� 
{
	int ans = 1, res = a;
	while (b)
	{
		if (b & 1)
			ans = Quick_Multiply(ans, res, c);
		res = Quick_Multiply(res, res, c);
		b >>= 1;
	}
	return ans;
}
bool Miller_Rabin(int x)     //�ж����� 
{
	int i, j, k;
	int s = 0, t = x - 1;
	if (x == 2)  return true;   //2������ 
	if (x < 2 || !(x & 1))  return false;     //���x��ż��������0,1�������������� 
	while (!(t & 1))  //��x�ֽ��(2^s)*t������ 
	{
		s++;
		t >>= 1;
	}
	for (i = 0; i < 10 && prime[i] < x; ++i)      //���ѡһ���������в��� 
	{
		int a = prime[i];
		int b = Quick_Power(a, t, x);      //�����a^t
		for (j = 1; j <= s; ++j)    //Ȼ�����s��ƽ�� 
		{
			k = Quick_Multiply(b, b, x);   //��b��ƽ�� 
			if (k == 1 && b != 1 && b != x - 1)     //�ö���̽���ж� 
				return false;
			b = k;
		}
		if (b != 1)  return false;   //�÷���С�����ж� 
	}
	return true;   //������ж�β��Զ��ǶԵģ���ôx�ͺ��п��������� 
}