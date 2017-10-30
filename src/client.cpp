//client code to request server
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n,intrst=0;
    float wait_time;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];
    if (argc < 5) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
    char const* const filename = argv[3];
    FILE* file = fopen(filename,"r");
    FILE* file1 = fopen(filename,"r");
    char ch;
    int sz=0;
    //calculating file size of transactions.txt
    while(!feof(file1))
    {
        ch = fgetc(file1);
        if(ch == '\n')
        {
            sz++;
        }
    }

    wait_time = atof(argv[4]);
    //printf("Number of lines in transactions.txt is %d",sz);

    //send intrst = 0 to indicate it does not calculate interest
    n = write(sockfd,&intrst,sizeof(intrst));
        if (n < 0) 
        {
            error("ERROR writing to socket");
        }
    //send transaction file size to server
    n = write(sockfd,&sz,sizeof(sz));
        if (n < 0) 
        {
            error("ERROR writing to socket");
        }
    char * pch;
    int ts;
    char buffer3[256];
    int cntr = 10;
    while(fgets(buffer,sizeof(buffer),file))
    {
        printf("Client timestamp:%d\n",cntr);
        strcpy(buffer3,buffer);
        pch = strtok (buffer3," ");
        ts = atoi(pch);
        printf("Timestamp for this instruction is:%d\n",ts);
        printf("%s\n",buffer);
        if(ts <= cntr)
        {
            n = write(sockfd,buffer,strlen(buffer));
            if (n < 0) 
            {
                error("ERROR writing to socket");
            }
        }
        else if(ts > cntr)
        {
            while(ts != cntr)
            {
                cntr++;
                usleep(1000000*wait_time);
                printf("Client clock:%d\n",cntr);
            }
            n = write(sockfd,buffer,strlen(buffer));
            if (n < 0) 
            {
                error("ERROR writing to socket");
            }
        }

        
        bzero(buffer,256);
        n = read(sockfd,buffer,255);
        if (n < 0) 
        {
            error("ERROR reading from socket");
        }
         printf("%s\n",buffer);
         printf("\nTransaction completed\n");
         //sleep(2);
    }
    fclose(file);
    
    close(sockfd);
    return 0;
}
