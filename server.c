/*******************************************
** server.c
** 2023/6/25
********************************************/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<pthread.h>
#include<sys/socket.h>
#include<ctype.h>
#include<arpa/inet.h>
#include<sys/time.h>

#define SERVER_PORT 8538
#define BUFFIZE 128

void getDateTime(char* ptime)
{
        struct tm* tm_t;
        struct timeval time;

        gettimeofday(&time, NULL);
        tm_t = localtime(&time.tv_sec);
        if(NULL != tm_t)
        {
                sprintf(ptime, "Local time:%04d-%02d-%02d %02d:%02d:%02d.%03ld\n",
                        tm_t->tm_year+1900,
                        tm_t->tm_mon+1,
                        tm_t->tm_mday,
                        tm_t->tm_hour,
                        tm_t->tm_min,
                        tm_t->tm_sec,
                        time.tv_usec/1000);
        }
        return;
}

int main(int argc, char* argv[])
{
        int lfd = 0, newfd = 0, res = 0;
        char buff[BUFFIZE];
        char client_IP[BUFFIZE];
        char timebuff[BUFFIZE];
        int port = atoi(argv[1]);


        struct sockaddr_in server_addr, client_addr;
        socklen_t client_addr_len;

        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(port);
        server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

        //1 make socket
        lfd = socket(AF_INET, SOCK_STREAM, 0);
        if(lfd == -1)
        {
                printf("make socket failed.\n");
                return -1;
        }

        //2 bind server struct
        res = bind(lfd, (struct sockaddr*)(&server_addr), sizeof(server_addr));
        if(res == -1)
        {
                printf("bind failed.\n");
                return -2;
        }

        //3 set listen limit
        res = listen(lfd, 128);
        if(res == -1)
        {
                printf("listen failed.\n");
                return -3;
        }

        //4 accept client
        client_addr_len = sizeof(client_addr);
        printf("acepting ... ... ...\n");
        newfd = accept(lfd, (struct sockaddr*)(&client_addr), &client_addr_len);
        if(newfd == -1)
        {
                printf("accept failed.\n");
                return -4;
        }

		printf("client ip = %s, port = %d\n", inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, client_IP, sizeof(client_IP)), ntohs(client_addr.sin_port));

        while(1)
        {
                memset(buff, 0, sizeof(buff));
                memset(timebuff, 0, sizeof(timebuff));
                //5 read client
                res = read(newfd, buff, sizeof(buff));
                if(res == -1)
                {
                        printf("read failed.\n");
                        return -5;
                }

                write(STDOUT_FILENO, buff, res);

                //6 send same data back
                getDateTime(timebuff);
                strcat(buff,timebuff);
                send(newfd, buff, strlen(buff),0);
                //write(newfd, buff, res);
		}
        close(lfd);
        close(newfd);

        return 0;
}

