#include<stdio.h>
int main()
{
	int n,i,sum=0;
	printf(" enter a number ");
		scanf("%d",&n);
		for(i=1;i<=n;i++)
		{
			sum=sum+i;
		}
		printf("sum upto given number from 1 is %d",sum);
		return 0;
}
