#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>

char * FileName(char s[50])
{
        int i,j;
        char file[50];
        for(i=4;i<strlen(s);i++)
        {
                file[i-4]=s[i];
        }
        file[i-4]='\0';
        strcpy(s,file);
        return s;
}


char * sendStr(char s[10000])
{
        char c[10000],file[50];
        int i,j;

        for(i=0;i<strlen(s);i++)
        {
                if(s[i]==' '&&s[i+1]==' ')
                {
                        for(j=i;j<strlen(s);j++)
                        {
                                s[j]=s[j+1];
                        }
                i--;
                }
        }

        if(s[i]=='\0'&&s[i-1]==' ')
        {
                s[i-1]='\0';
        }


                int n;
                n=strlen(s);

                if(n==3||(n==4&&s[n]==' '))
                {
                        sprintf(c,"Enter Argument <filename> along with keyword 'get'");
                }
                else
                {
                        FILE *p;

                                strcpy(file,FileName(s));

                        int acc;
                        printf("FILE REQUESTED : %s?\n",file);
                        printf("Enter 1 to accept request, 0 to deny request\n");
                        scanf("%d",&acc);
                        if(acc==1)
                        {
                        printf("FILENAME : %s/\n",file);
                                p=fopen(file,"r");

                                if(p==NULL)
                                {
                                        sprintf(c,"No File Found!!");
                                }
                                else
                                {
                                        i=0;
                                        while(1)
                                        {
                                                if(feof(p))
                                                {
                                                        break;
                                                }
                                                char t;
                                                t=fgetc(p);
                                                c[i]=t;
                                                i++;
                                        }
                                        c[i-1]='\0';
                                }
                        }
                        else
                        {
                                sprintf(c,"File Request Denied");
                        }
                }
        strcpy(s,c);
        printf("\n---Sent File : %s---\n",file);
        return s;
}

int checkKey(char s[50])
{
        if(s[0]=='g'&&s[1]=='e'&&s[2]=='t')
                return 1;
        else
                return 0;
}

void main(int argc,char *argv[]) {

        if(argc<2)
        {
                perror("Port No. Not provided\n");
                exit(1);
        }
        int server_socket;int opt=1;
        struct sockaddr_in server_address;
        server_socket=socket(AF_INET,SOCK_STREAM,0);
        if(server_socket<0)
        {
                perror("Socket Creation Failed\n");
                exit(1);
        }
        bzero((char *)&server_address,sizeof(server_address));
        int port;
        port=atoi(argv[1]);
        server_address.sin_family=AF_INET;
        server_address.sin_addr.s_addr=INADDR_ANY;
        server_address.sin_port=htons(port);
        if(( bind(server_socket,(struct sockaddr *)&server_address, sizeof(server_address)) )<0)
        {
                perror("Binding Failed\n");
                exit(1);
        }
        listen(server_socket,5);
        printf("Listening...\n");
        int client_socket;
        struct sockaddr_in client_address;
        socklen_t client_len;
        client_len=sizeof(client_address);
        client_socket=accept(server_socket,(struct sockaddr *)&client_address,&client_len);
        if(client_socket<0)
        {
                perror("Accept Error\n");
                exit(1);
        }
        char message[20000];
        int n; int j=0;
        while(1)
        {
                bzero(message,sizeof(message));
                n=read(client_socket,message,sizeof(message));


                if(n<0)
                {
                        perror("Read Failure\n");
                }
                if(checkKey(message)==0)
                {
                        printf("Client : %s\n",message);

                        printf("Enter Message : ");
                        if(j==0)
                        {
                                scanf("%[^\n]s",message);
                        }
                        else
                        {
                                scanf("%*c%[^\n]s",message);
                        }

                        n=write(client_socket, message , strlen(message));
                        if(n<0)
                        {
                                perror("Write Failure\n");
                                exit(1);
                        }
                        int i;
                        i=strcmp(message,"bye");
                        if(i==0)
                        {
                                break;
                        }
                        j++;
                }
                else
                {
                        sendStr(message);
                        write(client_socket,message,strlen(message));

                }
        }
        close(server_socket);
        close(client_socket);
}


