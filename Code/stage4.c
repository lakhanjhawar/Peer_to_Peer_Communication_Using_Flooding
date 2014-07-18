#include <stdio.h>
#include <stdlib.h>
#include <thread.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#define N 10
mutex_t lock_head[N+1],lock_tail[N+1];
thread_t tid, thr[N+1];
int r_num,nw[N+1][N+1];
char line[100];
char token[] = {" ."};
char *ptoken;
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
 void *peer(void *arg);
int main(int argc, char *argv[])
{
	FILE *fp;
	node_ptr y;
	char buff[20];
	int i,j,count,row,col,c_num,n,status;
	fp=fopen(argv[1],"r");
	r_num=atoi(fgets(buff,20,fp));
	printf("rnum=%d\n",r_num);
	for (n=0; n<=r_num+1; n++)
		{
		mutex_init(&lock_head[n], USYNC_THREAD, NULL);
		mutex_init(&lock_tail[n], USYNC_THREAD, NULL);
			}
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

	for(n=0;n<=r_num+1;n++)
	{
		head[n]=NULL;
		tail[n]=NULL;
	}
	for (n=2;n<=r_num+1; n++)
	  thr_create(NULL, 0, peer, (void*)n, THR_BOUND, &thr[n]);
	while (thr_join(0, &tid, (void**)&status)==0)
	  printf("main - status=%d, first_id=%d\n", status, tid);

	for (n=0; n<=r_num+1; n++)
		{
		mutex_destroy(&lock_head[n]);
		mutex_destroy(&lock_tail[n]);
		}
return 0;
}
int sendMSG(int sender1,int receiver1,int msg1,int ttl1)
{
temp=(node*)malloc(sizeof(node));
	if( temp == NULL)
	     printf(" Out of Memory !! Overflow !!!");
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
		mutex_lock(&lock_tail[qid]);
		 if(head[qid] == NULL)
			        {
			        	tail[qid]= head[qid] = temp1;
			        }
			        else
			        {
			        	tail[qid]->next=temp1;
			        	tail[qid]=temp1;
			        }
			        //printf(" the tail details are %d",tail[qid]->msg);
			        //printf("/n the head details are %d",head[qid]->msg);
			        mutex_unlock(&lock_head[qid]);
			        mutex_unlock(&lock_tail[qid]);
		return 0;
	}

node_ptr grabMSG(int x)
{
	node_ptr y;
	y=(node*)malloc(sizeof(node));
	mutex_lock(&lock_head[x]);
			mutex_lock(&lock_tail[x]);
	 if(head[x]==NULL)
	 {
	 printf("Queue is empty\n");
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
	 			        mutex_unlock(&lock_tail[x]);
	 return y;
	  }
}
void *peer(void *arg)
{
	int myid=(int)arg,i,j;
	int myneighbours[N+1];
	for(i=0;i<=r_num;i++)
	myneighbours[i]=nw[myid-1][i];

	printf("\nNeigbours of peer\n = %d are : ", myid);
	for(i=0;i<=r_num;i++)
	{
			if (myneighbours[i] == 1 )
				printf("\n%d\n", i+1);
			}
		sleep(myid);
		return NULL;
}


