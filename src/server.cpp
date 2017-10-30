//Server code to accept client requests
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <semaphore.h>
#include <pthread.h>

//struct to store the records.txt file
struct records{
    int accnt_num;
    char name[50];
    float accnt_bal;
};

//mutex to lock the crtical resource for concurrency control
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

//array of structures to store records
struct records rec[1000];
void * connection_handler(void *); /*connection handler for each client request*/
void startprocess(int,int);
void withdraw(int,float,int); /* to handle withdraw request*/
void deposit(int,float,int);  /* to handle deposit request*/
void intrst_calc(float,int); /* to calculate interest */

//Default error meassage
void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{   
int *new_sock;
int t=0;
int t1,t2,t3,t4=0;  
char const* const filename = "records.txt";
FILE* file = fopen(filename,"r");
char * pch;
char buffer[256];
//load the records.txt file into struct rec
while(fgets(buffer,sizeof(buffer),file))
{
    t1=0,t2=0,t3=0;
    //printf("\nReading File\n");
    //printf("%s\n",buffer);
    pch = strtok (buffer," ");
    while(pch != NULL && t3!=1)
    {   
        if(t1 == 0)
        {
            rec[t].accnt_num = atoi(pch);
            t1 = 1;
        }
        else if(t1 == 1 && t2 == 0)
        {
            strcpy(rec[t].name,pch);
            t2 = 1;
        }
        else if(t1 ==1 && t2 ==1)
        {
            //printf("pch:%s",pch);
            rec[t].accnt_bal = atof(pch);
            t3 =1;
        }
        pch = strtok (NULL, " ");
    }
    t++;
t4++;
}
printf("Number of records in records.txt is %d\n",t4);
printf("************************************************************************\n");
for(int a=0;a<t4;a++)
 {
     printf("record:%d\n",a);
     printf("%d\n",rec[a].accnt_num);
     printf("%s\n",rec[a].name);
     printf("%.02f\n",rec[a].accnt_bal);
     printf("************************************************************************\n");
}

     int sockfd, newsockfd, portno, pid;
     socklen_t clilen;
     struct sockaddr_in serv_addr, cli_addr;
     int fd;
     struct stat mystat;

     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");
     listen(sockfd,10);
     clilen = sizeof(cli_addr);
     

     while (newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr, &clilen)) {
         printf("Connection accepted \n");
         new_sock = (int *)malloc(1);   
        *new_sock = newsockfd;
         pthread_t pid;
         if(pthread_create(&pid,NULL,connection_handler,(void*)new_sock) < 0)    
         {
            perror("No thread created");
            return 1;
         }

        printf("handler assigned\n");
     } // end of while 
     close(sockfd);
     return 0; 
}

void * connection_handler(void *socknewfd)
{
    //printf("\nInside connection handler\n");
    int sock1 = *(int *)socknewfd;
    printf("\nConnected to client id:%d\n",sock1);
    int t10 = 0;
    //calculate size of records.txt file
    char const* const filename3 = "records.txt";
    FILE* file3 = fopen(filename3,"r");
    char buffer3[256];
    //calculate the size of the records.txt file
    while(fgets(buffer3,sizeof(buffer3),file3))
    {
        t10++;
    }
    startprocess(sock1,t10);
}

void startprocess (int sock,int t5)
{
   int n,k,b_size,sz,ac_num,amt,intrst;
   //n = read(sock,buffer1,255);
   //printf("Inside startproces\n");
    n = read(sock,&intrst,sizeof(intrst));
    if (n < 0) error("ERROR reading from socket");

    //printf("Interest flag is:%d\n",intrst);
    //checking if it is interest request
    if(intrst == 1)
    {
        sleep(5);
        //printf("In interest block");
        float intrst_amt;
        n = read(sock,&intrst_amt,sizeof(intrst_amt));
        if (n < 0) error("ERROR reading from socket");
        intrst_calc(intrst_amt,t5);
        n = write(sock,"Data received from client",25);
        if (n < 0) error("ERROR writing to socket");
    printf("\n************************************************************************\n");
for(int a=0;a<t5;a++)
 {
     printf("record:%d\n",a);
     printf("%d\n",rec[a].accnt_num);
     printf("%s\n",rec[a].name);
     printf("%.02f\n",rec[a].accnt_bal);
     printf("************************************************************************\n");
}
    }
    //else indicates its a client request to withdraw or deposit
   else
   {
   n = read(sock,&sz,sizeof(sz));
   if (n < 0) error("ERROR reading from socket");
   do{
   char buffer[256];   
   bzero(buffer,256);
   n = read(sock,buffer,255);
   if (n < 0) error("ERROR reading from socket");
   b_size = strlen(buffer);
    //printf("size of string:%d\n",b_size-1);

    int p1=0,p2=0,p3=0,p4=0;
    char st[8];
    char * pch;
    pch = strtok (buffer," ");
    while(pch != NULL && p4!=1)
    {   
        if(p1 == 0)
        {
            p1 = 1;
        }
        else if(p1 == 1 && p2 == 0)
        {
            ac_num = atoi(pch);
            p2 = 1;
        }
        else if(p1 ==1 && p2 ==1 && p3 == 0)
        {
            p3 =1;
        }
        else if(p1 ==1 && p2 ==1 && p3 == 1)
        {
            amt = atof(pch);
            p4 =1;
        }
        pch = strtok (NULL, " ");
    }


    for(k = 0; k<b_size-1;k++)
    {

        if(buffer[k] == 'w')
        {
            //printf("withdraw\n");
            withdraw(ac_num,amt,t5);
            break;
        }
        if(buffer[k] == 'd')
        {
            //printf("deposit\n");
            deposit(ac_num,amt,t5);
            break;
        }
    }
   //printf("Data received from client: %s\n",buffer);
   n = write(sock,"Data received at server end",27);
   if (n < 0) error("ERROR writing to socket");
   sz--;
}while(sz > 0);
printf("\nNumber of records in records.txt is %d\n",t5);
printf("\n************************************************************************\n");
for(int a=0;a<t5;a++)
 {
    printf("record:%d\n",a);
     printf("%d\n",rec[a].accnt_num);
     printf("%s\n",rec[a].name);
     printf("%.02f\n",rec[a].accnt_bal);
     printf("************************************************************************\n");
 }
}
}

void intrst_calc(float intr,int loop)
{
    for(int b=0;b<loop;b++)
    {
            if(rec[b].accnt_bal>0)
            {
            pthread_mutex_lock(&mutex);
            rec[b].accnt_bal = rec[b].accnt_bal + (rec[b].accnt_bal * intr);
            pthread_mutex_unlock(&mutex);
            printf("\nInterest added to account number:%d\n",rec[b].accnt_num);
        }
        else
        {
            printf("\nAccount balance for %d is zero, no interest added",rec[b].accnt_num);
        }
        }
}

void withdraw(int ac_num,float amt,int t6)
{
    int flag = 0;
    printf("\nData received from client\n");
    printf("Withdraw %.02f from account %d\n",amt,ac_num);
    for(int b=0;b<t6;b++)
    {
        if(rec[b].accnt_num == ac_num)
        {
            if(rec[b].accnt_bal < amt)
            {
                printf("Requested amount exceeds account balance\n");
            }
            else
            {
                pthread_mutex_lock(&mutex);
                //sleep(1);
                rec[b].accnt_bal = rec[b].accnt_bal - amt;
                printf("\n%.02f withdrawn from account number %d\n",amt,rec[b].accnt_num);
                printf("\nCurrent balance for account %d is %.02f\n",rec[b].accnt_num,rec[b].accnt_bal);
                pthread_mutex_unlock(&mutex);
            }
            flag = 1;
        }
    }
    if(flag == 0)
    {
        printf("\nPlease enter a valid account number. NO such record exists");
    }
}

void deposit(int ac_num,float amt,int t6)
{
int flag = 0;
printf("\nData received from client\n");
printf("\nDeposit %f into account %d",amt,ac_num);
    for(int b=0;b<t6;b++)
    {
        if(rec[b].accnt_num == ac_num)
        {
            pthread_mutex_lock(&mutex);
            rec[b].accnt_bal = rec[b].accnt_bal + amt;
            printf("\n%.02f deposited to account number %d\n",amt,rec[b].accnt_num);
            printf("\nCurrent balance for account %d is %.02f\n",rec[b].accnt_num,rec[b].accnt_bal);
            pthread_mutex_unlock(&mutex);
            flag = 1;

        }
    }
    if(flag == 0)
    {
        printf("Please enter a valid account number. NO such record exists");
    }
}