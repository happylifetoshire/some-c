#include<stdio.h>


int vc[]={1,2,-2,7,8,9,11,12};

//int vc[]={3,4,-1,1};

//int vc[]={1,2,0};

/*找出最小正数*/

/*
对时间复杂度有要求，根据循环排序的特性，数组索引的值刚好要等于索引
基于这个特性，把数组的值一一放到一个数组中，本题中，申请了一个100的数组，
如需要是需要用哈希表替换的，但是思路是正确的，就是从1开始遍历这个数组，直到
出现数组的值为0的情况，就是所求最小的正数。
*/


int main()
{
	int i;
	int len = sizeof(vc)/sizeof(int);
	int vv[100]={0};
	for(i=0;i<len;i++)
	{
		vv[vc[i]] = vc[i];
	}
	i=1;
	while(vv[i])
	{
		
		i++;
	}	
	printf("%d\n",i);
	return 0;
}