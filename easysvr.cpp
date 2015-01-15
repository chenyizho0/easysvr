/*************************************************************************"<<endl;
    > File Name: easysvr.cpp"<<endl;
    > Author: laixukai"<<endl;
    > Mail: laixukai@126.com "<<endl;
    > Created Time: 2015年01月15日 星期四 08时59分18秒"<<endl;
 ************************************************************************/
#include<iostream>
#include<fstream>
#include<string>
using namespace std;
string svrfilename;
string clifilename;
int createSvrFile(char * name)
{
	svrfilename = string(name) + "svr.cpp";
	ofstream of;
	of.open(svrfilename.c_str());
	of << "#include<iostream>" << endl;
	of << "#include<stdio.h>"<<endl;
	of << "#include<sys/socket.h>"<<endl;
	of << "#include<stdlib.h>"<<endl;
	of << "#include<netinet/in.h>"<<endl;
	of << "#include<arpa/inet.h>"<<endl;
	of << "#include<unistd.h>"<<endl;
	of << "#include<errno.h>"<<endl;
	of << "#include<sys/wait.h>"<<endl;
	of << "#include\"mail.pb.h\""<<endl;
	of << "#include\"mailsvrimpl.h\""<<endl;
	of << "using namespace std;"<<endl;
	of << ""<<endl;
	of << "int myport = 22222;"<<endl;
	of << "int lisnum = 5;"<<endl;
	of << ""<<endl;
	of << ""<<endl;
	of << "void handler(int sig)"<<endl;
	of << "{"<<endl;
	of << "	printf(\"sig=%d\\n\",sig);"<<endl;
	of << "}"<<endl;
	of << ""<<endl;
	of << "int main()"<<endl;
	of << "{"<<endl;
	of << "	int sockfd,new_fd;"<<endl;
	of << "	socklen_t socklen;"<<endl;
	of << "	struct sockaddr_in my_addr,their_addr;"<<endl;
	of << "	socklen = sizeof(their_addr);"<<endl;
	of << ""<<endl;
	of << "	signal(SIGPIPE,handler);"<<endl;
	of << "	signal(SIGCHLD,SIG_IGN);"<<endl;
	of << ""<<endl;
	of << "	if ((sockfd = socket(AF_INET,SOCK_STREAM,0)) == -1)"<<endl;
	of << "	{"<<endl;
	of << "		perror(\"socket\");"<<endl;
	of << "		exit(errno);"<<endl;
	of << "	}"<<endl;
	of << "	memset(&my_addr,0,sizeof(my_addr));"<<endl;
	of << "	my_addr.sin_family = AF_INET;"<<endl;
	of << "	my_addr.sin_port = htons(myport);"<<endl;
	of << "	my_addr.sin_addr.s_addr = inet_addr(\"127.0.0.1\"); "<<endl;
	of << "	if (bind(sockfd,(struct sockaddr *) &my_addr,sizeof(struct sockaddr)) == -1)"<<endl;
	of << "	{"<<endl;
	of << "		perror(\"bind\");"<<endl;
	of << "		exit(errno);"<<endl;
	of << "	}"<<endl;
	of << "	if (listen(sockfd,lisnum) == -1)"<<endl;
	of << "	{"<<endl;
	of << "		perror(\"listen\");"<<endl;
	of << "		exit(errno);"<<endl;
	of << "  	}"<<endl;
	of << "	while(true)"<<endl;
	of << "	{"<<endl;
	of << "		if ((new_fd = accept(sockfd,(struct sockaddr *)&their_addr,&socklen)) == -1)"<<endl;
	of << "	  	{"<<endl;
	of << "	  		perror(\"accept\");"<<endl;
	of << "	  		exit(errno);"<<endl;
	of << "	  		return -1;"<<endl;
	of << "	  	}"<<endl;
	of << "		int pid;"<<endl;
	of << "		if((pid = fork()) == -1)"<<endl;
	of << "		{"<<endl;
	of << "			perror(\"fork\");"<<endl;
	of << "			close(new_fd);"<<endl;
	of << "			continue;"<<endl;
	of << "		}"<<endl;
	of << "		if (pid > 0)"<<endl;
	of << "		{"<<endl;
	of << "			close(new_fd);"<<endl;
	of << "			continue;"<<endl;
	of << "		}"<<endl;
	of << "		else if(pid == 0)"<<endl;
	of << "		{"<<endl;
	of << "			close(sockfd);"<<endl;
	of << "			char buffer[1000];"<<endl;
	of << "	  		recv(new_fd,buffer,1000,0);"<<endl;
	of << "	  		SenderMsg sendermsg;"<<endl;
	of << "	  		if (sendermsg.ParseFromString(string(buffer)) != 0)"<<endl;
	of << "	  		{"<<endl;
	of << "	  			printf(\"parse error\\n\");"<<endl;
	of << "	  			return -2;"<<endl;
	of << "	  		}"<<endl;
	of << "	  		else"<<endl;
	of << "	  		{"<<endl;
	of << "	  			cout << sendermsg.smtpsvrport() << endl;"<<endl;
	of << "	  			cout << sendermsg.sendername() << endl;"<<endl;
	of << "	  			cout << sendermsg.sendermailname() << endl;"<<endl;
	of << "	  			cout << sendermsg.smtpsvrname() << endl;"<<endl;
	of << "	  			cout << sendermsg.base64name() << endl;"<<endl;
	of << "	  			cout << sendermsg.base64passwd() << endl;"<<endl;
	of << "	  		}"<<endl;
	of << "	  		CheckIdReturnMsg checkidreturnmsg;"<<endl;
	of << "	  "<<endl;
	of << "	/*lo	gic code*/  "<<endl;
	of << "	  		int iRet = func(sendermsg,checkidreturnmsg);"<<endl;
	of << "	  		if (iRet != 0)"<<endl;
	of << "	  		{"<<endl;
	of << "	  			printf(\"logic error\\n\");"<<endl;
	of << "	  			return -3;"<<endl;
	of << "	  		}"<<endl;
	of << "	  "<<endl;
	of << "	  "<<endl;
	of << "	  		string sCheckidreturnmsg;"<<endl;
	of << "	  		checkidreturnmsg.SerializeToString(&sCheckidreturnmsg);"<<endl;
	of << "	  		send(new_fd,sCheckidreturnmsg.c_str(),sCheckidreturnmsg.size(),0);"<<endl;
	of << "			close(new_fd);"<<endl;
	of << "			exit(0);"<<endl;
	of << "		}"<<endl;
	of << "	}"<<endl;
	of << "}"<<endl;
	of << "	return 0;"<<endl;
	of << "}"<<endl;
	of.close();
	return 0;
}

int createCliFile(char * name)
{
	clifilename = string(name) + "svr.cpp";
	ofstream of;
	of.open(clifilename.c_str());

}
int main(int argc,char ** argv)
{
	if (argc != 2)
	{
		printf("Usage : %s svrname",argv[0]);
		return 0;
	}
	int iRet = createSvrFile(argv[1]);
	if (iRet != 0)
	{
		printf("create svr file error\n");
		return -1;
	}
}
