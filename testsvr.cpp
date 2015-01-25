#include<iostream>
#include<fstream>
#include<stdio.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<errno.h>
#include<sys/wait.h>
#include"test.pb.h"
#include"testsvrimpl.h"
#include"testsvr.h"
using namespace std;


void handler(int sig)
{
	printf("sig=%d\n",sig);
}

int init(const char * confFile,string &sIp,int &myport,int &lisnum)
{
	ifstream readfile;
	readfile.open(confFile);
	if (!readfile.is_open())
	{
		printf("read conf file error : %s\n",confFile);
		return -1;
	}
	char tmp[100];
	if (!readfile.eof())
	{
		readfile.getline(tmp,100);
		sIp = string(tmp);
	}
	else
	{
		printf("no ip in conf : %s\n",confFile);
		return -2;
	}
	if (!readfile.eof())
	{
		readfile.getline(tmp,100);
		myport = atoi(tmp);
		if (myport < 0)
		{
			printf("port error : %s\n",confFile);
			return -3;
		}
	}
	else
	{
		printf("no port in conf : %s\n",confFile);
		return -4;
	}
	if (!readfile.eof())
	{
		readfile.getline(tmp,100);
		lisnum = atoi(tmp);
		if (lisnum < 0)
		{
			printf("lisnum error :%s\n",confFile);
			return -5;
		}
	}
	else
	{
		printf("no lisnum in conf :%s\n",confFile);
		return -6;
	}
	readfile.close();
	return 0;
}

int process(int new_fd)
{
	int iErrorCode = 0;
	while(1)
	{
		int iRet = 0;
		char buffer[1000];
		int numbytes = recv(new_fd,buffer,1000,0);
		if (numbytes < 0)
		{
			printf("recv error\n");
			iErrorCode = -1;
			break;
		}
		string sFuncType = string(buffer,numbytes);
		FuncType functype_obj;
		if (!functype_obj.ParseFromString(sFuncType))
		{
			printf("parse functype error\n");
			iErrorCode = -2;
			break;
		}
		string sMsg;
		int iType = 0;
		sMsg = functype_obj.msg();
		iType = functype_obj.type();
		switch(iType)
		{
			case 1:
			{
				iRet = callFuncFuncA(sMsg,new_fd);
				if (iRet != 0)
				{
					printf("callFuncFuncA error : %d\n",iRet);
				}
				break;
			}
			default:
				break;
		}
	}
	close(new_fd);
	return iErrorCode;
}

int main()
{
	int myport = 0;
	int lisnum = 0;
	string sIp;
	int iRet = 0;
	iRet = init("testsvr.conf",sIp,myport,lisnum);
	if (iRet != 0)
	{
		printf("init error %d\n",iRet);
		return 0;
	}
	int sockfd,new_fd;
	socklen_t socklen;
	struct sockaddr_in my_addr,their_addr;
	socklen = sizeof(their_addr);

	signal(SIGPIPE,handler);
	signal(SIGCHLD,SIG_IGN);

	if ((sockfd = socket(AF_INET,SOCK_STREAM,0)) == -1)
	{
		perror("socket");
		exit(errno);
	}
	memset(&my_addr,0,sizeof(my_addr));
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(myport);
	my_addr.sin_addr.s_addr = inet_addr(sIp.c_str()); 
	if (bind(sockfd,(struct sockaddr *) &my_addr,sizeof(struct sockaddr)) == -1)
	{
		perror("bind");
		exit(errno);
	}
	if (listen(sockfd,lisnum) == -1)
	{
		perror("listen");
		exit(errno);
  	}
	while(true)
	{
		if ((new_fd = accept(sockfd,(struct sockaddr *)&their_addr,&socklen)) == -1)
	  	{
	  		perror("accept");
	  		exit(errno);
	  		return -1;
	  	}
		char buf[128];

		int pid;
		if((pid = fork()) == -1)
		{
			perror("fork");
			close(new_fd);
			continue;
		}
		if (pid > 0)
		{
			close(new_fd);
			continue;
		}
		else if(pid == 0)
		{
			close(sockfd);
			iRet = process(new_fd);
			printf("process return : %d\n",iRet);
			return iRet;
		}
	}
}
int callFuncFuncA(const string &sMsg,const int & new_fd)
{
	A A_obj;
	if (!A_obj.ParseFromString(sMsg))
	{
		printf("parse error\n");
		return -2;
	}
	ReturnA ReturnA_obj;
	int iRet = funcFuncA(A_obj,ReturnA_obj);
	if (iRet != 0)
	{
		printf("logic error\n");
		return -3;
	}
	string sReturn;
	ReturnA_obj.SerializeToString(&sReturn);
	iRet = send(new_fd,sReturn.c_str(),sReturn.size(),0);
	if (iRet < 0)
	{
		printf("send error\n");
		return -4;
	}
	return 0;
}

