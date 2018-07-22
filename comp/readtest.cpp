#include <cstdio>
int main()
{
	const char *s="test.txt";
	FILE *f=fopen(s,"r");
	int i=0;
	while(1){
		char c=fgetc(f);
		if(c==EOF) break;
		i++;
		printf("%u ",c);
	}
	printf("\n%u\n",i);
	char x=254;
	unsigned char t=254;
	int y=t;
	printf("%d %u\n",y,x);
	return 0;
}
