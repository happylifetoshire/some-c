### 回溯法

1，此类问题可能有多种答案，所以需要找出所有解，并在其中选择一组满足要求的解

2，需要建立并列树来帮助理解，找出其分叉的点，就是需要递归回溯的点

3，通过题目所给的约束条件来排除一些不必要的解。

以下是一般通用代码

```
void travel(int len,int t)
{
	int i;
	if(t >= len)
	{		
		return;
	}else{
		
		for(i=0;i<len;i++)
		{
			
			if(约束条件)
			{
				st[t] = sp[i];
				travel(len,t+1);
			}
		}

	}
}

```