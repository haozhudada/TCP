/**************************************************
** client.c
** 2023/6/25
***************************************************/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<pthread.h>
#include<sys/socket.h>
#include<arpa/inet.h>

#define SERVER_PORT 8538
#define BUFFIZE 128

int main(int argc, char* argv[])
{
        int fd = 0;
        int res = 0;
        int count = 10;
        char buff[BUFFIZE];

        struct sockaddr_in server_addr;
        server_addr.sin_family = AF_INET;
	if(argc == 2)
	        server_addr.sin_port = htons(atoi(argv[1]));
        else
		server_addr.sin_port = htons(SERVER_PORT);
	inet_pton(AF_INET,"127.0.0.1",&server_addr.sin_addr);
        // 1 make socket
        fd = socket(AF_INET, SOCK_STREAM, 0);
        if(fd == 1)
        {
                printf("make socket failed.");
                return -1;
        }

        // 2 connect
        res = connect(fd,(struct sockaddr*)(&server_addr), sizeof(server_addr));
        if(res == -1)
        {
                printf("make connect failed.\n");
                return -2;
        }

        while(count--)
        {
                //3 write data to server.
                res = write(fd, "hello.\n",sizeof("hello.\n"));
                if(res == -1)
                {
                        printf("write failed.\n");
                        return -3;
                }
                sleep(1);

                //4 read data from server.
                res = read(fd, buff, sizeof(buff));
                if(res == -1)
                {
                        printf("read failed.\n");
                        return -4;
                }

                //5 show data on desktop
                int ret = write(STDOUT_FILENO, buff, res);
                if(res == -1)
                {
                        printf("write desktop failed.\n");
                        return -5;
                }
        }
        close(fd);
        return 0;
}

