#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>  //for read,write and close functions
#include <netinet/in.h>
#include <string.h>
//#include <netdb.h>



int checkKey(char s[50])
{
        int i=0;
        for(i=0;i<strlen(s);i++)
        {
                if(s[i]!=' ')
                {
                        if(s[i]=='g'&&s[i+1]=='e'&&s[i+2]=='t'&&s[i+3]==' ')
                        {
                                return 1;
                        }
                        else
                        {
                                return 0;
                        }
                }
        }
}


void FileName(char s[50],char file[50])
{
        int i,j,n;
        n=strlen(s);
        for(i=0;i<n;i++)
        {
                if(s[i]==' '&&(s[i+1]==' '||s[i+1]=='\0'))
                {
                        for(j=i;j<n;j++)
                        {
                                s[j]=s[j+1];
                        }

                        i--;
                }
        }
        if(s[0]==' ')
        {
                for(i=0;s[i]!='\0';i++)
                {
                        s[i]=s[i+1];
                }
        }
        for(i=4;s[i]!='\0';i++)
        {
                file[i-4]=s[i];
        }
        file[i-4]='\0';
}



void main(int argc,char *argv[])
{
	if(argc<2)
	{
		perror("Enter Port number to check and connect to Listening Servers\n");
		exit(1);
	}
	int my_socket;
	struct sockaddr_in server_address;

	int port;
	port=atoi(argv[1]);
	my_socket=socket(AF_INET,SOCK_STREAM,0);

	if(my_socket<0)
	{
		perror("Socket Creation Failed\n");
		exit(1);
	}

	bzero((char *)&server_address,sizeof(server_address));

	server_address.sin_family=AF_INET;
	server_address.sin_port=htons(port);
	server_address.sin_addr.s_addr=INADDR_ANY;

	if(connect(my_socket,(struct sockaddr *)&server_address,sizeof(server_address))<0)
	{
		perror("Connection Failed\n");
		exit(1);
	}
	printf("Connection Successful...\n");
	char message[10000];
	int n,j=0;

	while(1)
	{
		bzero(message,sizeof(message));
ll:		printf("Enter Message : ");

		if(j==0)
		{
			scanf("%[^\n]s",message);
		}
		else
		{
			scanf("%*c%[^\n]s",message);
		}

		n=write(my_socket,message,strlen(message));

		if(n<0)
		{
			perror("Write Error\n");
		}

		if(checkKey(message)==0)
		{
			bzero(message,sizeof(message));

			n=read(my_socket,message,sizeof(message));
			if(n<0)
			{
				perror("Read Error\n");
			}
			printf("Server : %s\n",message);

			int i;
			i=strcmp(message,"bye");
			if(i==0)
			{
				printf("Server Disconnected\n");
				break;
			}
			j++;
		}
		else
		{
			char file[50];
			FileName(message,file);

			int n;
			n=read(my_socket,message,sizeof(message));

			int accept;
			if((strcmp(message,"No File Found!!")==0)||(strcmp(message,"File Request Denied")==0)||(strcmp(message,"Enter Argument <filename> along with keyword 'get'")==0))
			{
				printf("Server : %s\n",message);
			}
			else
			{
		        	FILE *p;
				p=fopen(file,"r");
				if(p!=NULL)
				{
					printf("FILE ALREADY EXISTS\n");
					printf("Enter 1 to overwrite, 0 to create another file\n");
					scanf("%d",&accept);
					if(accept!=1)
					{
						int N=strlen(file);
						file[N-2]='c';
						file[N-1]='o';
						file[N+0]='p';
						file[N+1]='y';
						file[N+2]='.';
						file[N+3]='c';
						file[N+4]='\0';

					}
				}
				printf("\n%s\n",message);
		        	printf("Writing received data into file ----%s----\n",file);
				fclose(p);

				p=fopen(file,"w");
				if(p==NULL)
				{
					printf("File Creation Unsuccessful!!\n");
				}

				fputs(message,p);

				printf("Data Written Successfully...\n");
		       		fclose(p);
				goto ll;
			}

		}

	}
}

