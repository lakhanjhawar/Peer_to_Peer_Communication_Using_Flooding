#include <stdio.h>
#include <stdlib.h>
#include <thread.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#define N 10
mutex_t lock_head[N+1];
int r_num,nw[N+1][N+1];
char line[100];
char token[] = {" ."};
char *ptoken;
FILE *print;
typedef struct MSG
 {unsigned int sender;
   unsigned int TTL;
   unsigned int msg;
   struct MSG *next;
                } node, *node_ptr;
 node_ptr head[N+1],tail[N+1],temp;
 int sendMSG(int,int,int,int);
 int enterQ(node_ptr,int);
 node_ptr grabMSG(int);
int main(int argc, char *argv[])
{

	FILE *fp;
	node_ptr y;
	char buff[20];
	int i,j,count,row,col,c_num,n;
	fp=fopen(argv[1],"r");
	print=fopen("stage3_log.txt","w");
	r_num=atoi(fgets(buff,20,fp));
	for (n=0; n<=r_num+1; n++)
		{
		mutex_init(&lock_head[n], USYNC_THREAD, NULL);
			}

	for(i=2;i<=r_num+1;i++)
	{
	do{
		fgets(buff,20,fp);
		ptoken = strtok(buff,token);
		row=i-1;
		c_num=atoi(ptoken);
		for (j=0;j<c_num;j++)
		{
		 ptoken = strtok(NULL,token);
			col = atoi(ptoken);
			nw[row][col]=1;
		}
	} while (feof(fp)!=0);

	}

	for (i=0;i<=r_num+1;i++){
		for(j=0;j<=r_num+1;j++){
			if (nw[i][j]==1)
				nw[j][i]=1;
		}
	}
    fprintf(print,"The connection matrix is\n");
	for (i=0;i<=r_num+1;i++){
		for(j=0;j<=r_num+1;j++){
		fprintf(print,"%d\t",nw[i][j]);
		}
		fprintf(print,"\n");
	}

	for(n=0;n<=r_num+1;n++)
	{
		head[n]=NULL;
		tail[n]=NULL;
	}
	sendMSG(1,2,1000,1);
	sendMSG(1,2,2000,2);
	sendMSG(1,4,2000,4);
	if((y=grabMSG(2))!= NULL)
	fprintf(print,"message for 2 is %d\n",y->msg);
	else
	fprintf(print," 2 has no message\n");
	if((y=grabMSG(3))!= NULL)
		fprintf(print,"message for 3 is%d\n",y->msg);
		else
		fprintf(print," 3 has no message\n");
		if((y=grabMSG(2))!= NULL)
			fprintf(print,"message for 2 is%d\n",y->msg);
			else
			fprintf(print,"no message for 2\n");
	if((y=grabMSG(4))!= NULL)
		fprintf(print,"message for 4 is%d\n",y->msg);
		else
		fprintf(print,"no message for 4\n");
	if((y=grabMSG(2))!= NULL)
		fprintf(print,"message for 2 is %d\n",y->msg);
		else
		fprintf(print," 2 has no message\n");
	for (n=0; n<=r_num+1; n++)
	{
	if(head[n]!=NULL)
		fprintf(print,"the queue %d",n,"is not empty\n");
	}
	fprintf(print,"-------------------------------------------------------------------------------------------------");
	for (n=0; n<=r_num+1; n++)
		{
		mutex_destroy(&lock_head[n]);
		}
	fclose(print);
	fclose(fp);
return 0;
}
int sendMSG(int sender1,int receiver1,int msg1,int ttl1)
{
temp=(node*)malloc(sizeof(node));
	if( temp == NULL)
	     printf("");
	    else
	    {
	        temp->sender=sender1;
	        temp->msg=msg1;
	        temp->TTL=ttl1;
	        temp->next=NULL;
	    }
	        enterQ(temp,receiver1);
return 0;
}
	int enterQ(node_ptr temp1,int qid)
	{
		mutex_lock(&lock_head[qid]);
		 if(head[qid] == NULL)
			        {
			        	tail[qid]= head[qid] = temp1;
			        }
			        else
			        {
			        	tail[qid]->next=temp1;
			        	tail[qid]=temp1;
			        }
			       mutex_unlock(&lock_head[qid]);
		return 0;
	}

node_ptr grabMSG(int x)
{
	node_ptr y;
	y=(node*)malloc(sizeof(node));
	mutex_lock(&lock_head[x]);
	 if(head[x]==NULL)
	 {
	 printf("");
		mutex_unlock(&lock_head[x]);
	 return NULL;
	 }
	 else
	  {
	  y->msg=head[x]->msg;
	  y->sender=head[x]->sender;
	  y->TTL=head[x]->TTL;
	 if(tail[x]==head[x])
	  {
	  tail[x]=NULL;
	  head[x]=NULL;
	  }
	 else
	 head[x]=head[x]->next;
	 mutex_unlock(&lock_head[x]);
	 return y;
	  }
}
