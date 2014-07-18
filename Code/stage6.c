#include <stdio.h>
#include <stdlib.h>
#include <thread.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#define N 10
mutex_t lock_head[N+1];
thread_t thr[N+1];
int r_num,nw[N+1][N+1],flag[N+1],status[N+1][N+1],status1[N+1][N+1][N+1],num_of_entries_in_queue;
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
 node_ptr head[N+1],tail[N+1];
 int sendMSG(int,int,int,int);
 int sendinitMSG(int,int,int,int);
 int enterQ(node_ptr,int);
 node_ptr grabMSG(int);
 void *peer(void *arg);
 void *mainpeer(void *arg);
int main(int argc, char *argv[])
{
	FILE *fp;
	node_ptr y;
	char buff[20];
	int i,j,k,m,count,row=0,col,c_num,n,status,msg[10],ttl[10],fileflag=0,msgcount=0;
	fp=fopen(argv[1],"r");
	print=fopen("stage6_log.txt","w");
	r_num=atoi(fgets(buff,20,fp));
	for (i=0;i<N+1;i++)
		flag[i]=0;
	for (n=0; n<=r_num+1; n++)
		mutex_init(&lock_head[n], USYNC_THREAD, NULL);
	fgets(buff,20,fp);
	do{
		ptoken = strtok(buff,token);
		if (fileflag==0)
		{
		row++;
		c_num=atoi(ptoken);
		for (j=0;j<c_num;j++)
				{
				 ptoken = strtok(NULL,token);
					col = atoi(ptoken);
					nw[row][col]=1;
				}
		}
		else
				{
					msg[msgcount]=atoi(ptoken);
				 ptoken = strtok(NULL,token);
					ttl[msgcount]=atoi(ptoken);
					msgcount++;
				}

		if (row==r_num){
			fileflag=1;}
		fgets(buff,20,fp);
	} while (feof(fp)==0);

	for (i=0;i<=r_num+1;i++){
		for(j=0;j<=r_num+2;j++){
			if (nw[i][j]==1)
				nw[j][i]=1;
		}
	}
	nw[1][2]=1;

	for(n=0;n<=r_num+1;n++)
	{
		head[n]=NULL;
		tail[n]=NULL;
	}
	for(i=0;i<msgcount;i++)
		{
		for(j=0;j<N+1;j++)
			for(k=0;k<N+1;k++)
				for(m=0;m<N+1;m++)
					status1[j][k][m]=0;
	for (n=2;n<=r_num+1; n++)
	  thr_create(NULL,NULL,peer,(void*)n,NULL,&thr[n]);
	sendinitMSG(0,1,msg[i],ttl[i]);
		 mainpeer((void *)thr_self());
	while (thr_join(0, NULL, NULL) == 0)
		 printf("");
		}
	for (n=0; n<=r_num+1; n++)
		mutex_destroy(&lock_head[n]);
	fprintf(print,"-----------------------------------------------------------------------------------------------------");
	fclose(fp);
	fclose(print);
return 0;
}
int sendMSG(int sender1,int receiver1,int msg1,int ttl1)
{
	node_ptr temp;
temp=(node*)malloc(sizeof(node));
	if( temp == NULL)
	     printf("");
	    else
	    {
	        temp->sender=sender1;
	        temp->msg=msg1;
	        temp->TTL=ttl1;
	        temp->next=NULL;
	        status[sender1][receiver1]=1;
	    }
	num_of_entries_in_queue+=1;
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
	 num_of_entries_in_queue-=1;
	 return y;
	  }
}
void *peer(void *arg)
{
	int myid=(int)arg,i,j,msg1,ttl1,sender1,count,came_from;
	node_ptr y;
	int myneighbours[N+1],loop_count=0;
	for(i=0;i<=r_num+1;i++)
	myneighbours[i]=nw[myid][i];
	printf("Neigbours of peer\n = %d are :\n ", myid);
	for(i=0;i<=r_num+1;i++)
	{
			if (myneighbours[i] == 1)
				printf("\n%d\n", i);
	}
	usleep(100);
	for (i=0;i<10;i++){
	    status[myid][i]=0;
		status1[came_from][myid][i]=0;
	}
	while (num_of_entries_in_queue >= 0 && loop_count<=N)
		{
			//printf(" i am %d\n",myid);
		y=grabMSG(myid);
		if(y!=NULL)
		{
			y->TTL=y->TTL-1;
			//globalttl-=1;
			//ttl1=y->TTL-1;
			msg1=y->msg;
			sender1=y->sender;
			if(y->TTL>0)
			{
				came_from=sender1;
			//	printf("message  came from%d\n",came_from);
				for(i=0;i<=r_num+1;i++)
				{
					if(myneighbours[i]==1 && flag[i]!=1 && i!=came_from && status1[came_from][myid][i]==0)
					{
						 fprintf(print,"%d %d %d %d %d \n",came_from,myid,i,msg1,y->TTL);
						sendMSG(myid,i,msg1,y->TTL);
						status[myid][i]=1;
						status1[came_from][myid][i]=1;
					}
				}
			}
		}
		 else if (y==NULL) {
		            loop_count++;
					thr_yield();
					usleep(100);
				}
		free(y);
}

		return NULL;
}
int sendinitMSG(int sender1,int receiver1,int msg1,int ttl1)
{
	node_ptr temp;
temp=(node*)malloc(sizeof(node));
if(ttl1>1)
{
	if( temp == NULL)
	     printf("");
	    else
	    {
	        temp->sender=sender1;
	        temp->msg=msg1;
	        temp->TTL=ttl1;
	        temp->next=NULL;
	    }
	flag[sender1]=1;
	num_of_entries_in_queue=1;
	        enterQ(temp,receiver1);
	        return 0;
}
else
return 0;
}
void *mainpeer(void *arg)
{
	int id=(int)arg,sender1,came_from,msg1,ttl,i;
	node_ptr y;
	int myneighbours[N+1],loop_count=0;
		for(i=0;i<=r_num+1;i++)
		myneighbours[i]=nw[id][i];
	y=grabMSG(id);
	{
	if(y!=NULL)
	{
		ttl=y->TTL;
					msg1=y->msg;
					sender1=y->sender;
					if(y->TTL>0)
					{
						came_from=sender1;
						for(i=0;i<=r_num+1;i++)
										{
											if(myneighbours[i]==1 && flag[i]!=1 && i!=came_from)
											{
												 fprintf(print,"%d %d %d %d %d \n",came_from,id,i,msg1,y->TTL);
												 sendMSG(id,i,msg1,y->TTL);

											}
										}
									}
	}
}
}



