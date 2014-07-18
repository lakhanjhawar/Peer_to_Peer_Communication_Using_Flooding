#include <stdio.h>
#include <stdlib.h>
#include <thread.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#define N 10
int r_num,nw[6][6];
char line[100];
char token[] = {" ."};
char *ptoken;
int main(int argc, char *argv[])
{
	FILE *fp;
	char buff[20];
	int i,j,count,row,col,c_num;
	fp=fopen(argv[1],"r");
	r_num=atoi(fgets(buff,20,fp));
	printf("rnum=%d\n",r_num);
	for(i=1;i<=r_num;i++)
	{
		do{
			fgets(buff,20,fp);
            printf("%s",buff);
			ptoken = strtok(buff,token);
			row=i-1;
			c_num=atoi(ptoken);
			printf("No of peers:%d\n",c_num);

			for (j=0;j<c_num;j++)
			{
				ptoken = strtok(NULL,token);
				col = atoi(ptoken);
				printf("Received Col value:%d\n",col);
				col--;
				nw[row][col]=1;
				printf("%d %d %d\n",row,col,nw[row][col]);
			}
		} while (feof(fp)!=0);

	}

	for (i=0;i<6;i++){
		for(j=0;j<6;j++){
			if (nw[i][j]==1)
				nw[j][i]=1;
		}
	}


	for (i=0;i<6;i++){
		for(j=0;j<6;j++){
			printf("%d\t",nw[i][j]);
		}
		printf("\n");
	}



	return 0;
}

