#include <stdio.h>
#include <stdlib.h>
#include <thread.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include<process.h>
int main()
{int x = 0;
fork();
x++;
printf("%d %d",getpid(), x);
fork();
x++;
printf("%d %d",getpid(), x);
fork();
x++;
printf("%d %d",getpid(), x);
return 0;
}
