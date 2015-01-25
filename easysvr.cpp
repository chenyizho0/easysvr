#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include"easysvr.h"
using namespace std;

vector<FUNCTION> functions;


int readFunctions(const char * name)
{
	functions.clear();
	string funcfilename = string(name) + ".func";
	ifstream readfile;
	readfile.open(funcfilename.c_str());
	if (!readfile.is_open())
	{
		printf("funcfile error\n");
		return -1;
	}
	string function_name;
	string function_input;
	string function_output;
	int function_id = 0;
	while(!readfile.eof())
	{
		readfile >> function_name;
		if (!readfile.eof())
			readfile >> function_input;
		else
			break;
		if (!readfile.eof())
			readfile >> function_output;
		else
			break;
		++function_id;
		functions.push_back(FUNCTION(function_name,function_input,function_output,function_id));
	}
	readfile.close();
	for (int i = 0;i < functions.size();i++)
	{
		cout << functions[i].get_name() << " " << functions[i].get_input() << " " << functions[i].get_output() << " " << functions[i].get_id() << endl;
	}
	return 0;
}

int createProcessPart2(const char * name)
{
	string part3filename = string(name) + "svr.process.template.part2";
	ofstream of;
	of.open(part3filename.c_str());
	for (int i = 0;i < functions.size();i++)
	{
		of << "			case " << functions[i].get_id() << ":" << endl;
		of << "			{" << endl;
		of << "				iRet = callFunc" << functions[i].get_name() << "(sMsg,new_fd);" << endl;
		of << "				if (iRet != 0)" << endl;
		of << "				{" << endl;
		of << "					printf(\"callFunc" << functions[i].get_name() << " error : %d\\n\",iRet);" << endl;
		of << "				}" << endl;
		of << "				break;" << endl;
		of << "			}" << endl;
	}
	
	of << "			default:"  << endl;
	of << "				break;"  << endl;
	of << "		}"  << endl;
	of << "	}" << endl;
	of << "	close(new_fd);" << endl;
	of << "	return iErrorCode;"  << endl;
	of << "}"  << endl;
	of << endl;
	of.close();
	return 0;
}

int createSvrFunction(const char * name)
{
	string part3filename = string(name) + "svr.function.template";
	ofstream of;
	of.open(part3filename.c_str());
	for (int i  = 0;i < functions.size();i++)
	{
		of << "int callFunc" << functions[i].get_name() << "(const string &sMsg,const int & new_fd)" << endl;
		of << "{" << endl;
		of << "	" << functions[i].get_input() << " " << functions[i].get_input() << "_obj;" << endl;
		of << "	if (!" << functions[i].get_input() << "_obj.ParseFromString(sMsg))" << endl;
		of << "	{" << endl;
		of << "		printf(\"parse error\\n\");" << endl;
		of << "		return -2;" << endl;
		of << "	}" << endl;
		of << "	" << functions[i].get_output() << " " << functions[i].get_output() << "_obj;" << endl;
		of << "	int iRet = func" << functions[i].get_name() << "(" << functions[i].get_input() << "_obj" << "," << functions[i].get_output() << "_obj" << ");" << endl;
		of << "	if (iRet != 0)" << endl;
		of << "	{" << endl;
		of << "		printf(\"logic error\\n\");" << endl;
		of << "		return -3;" << endl;
		of << "	}" << endl;
		of << "	string sReturn;" << endl;
		of << "	" << functions[i].get_output() << "_obj"  << ".SerializeToString(&sReturn);" << endl;
		of << "	iRet = send(new_fd,sReturn.c_str(),sReturn.size(),0);" << endl;
		of << "	if (iRet < 0)" << endl;
		of << "	{" << endl;
		of << "		printf(\"send error\\n\");" << endl;
		of << "		return -4;" << endl;
		of << "	}" << endl;
		of << "	return 0;" << endl;
		of << "}" << endl;
		of << endl;
	}

	of.close();
	return 0;
}

int createSvrHeadFile(const char * name)
{
	string svrheadfilename = string(name) + "svr.h";
	ofstream of;
	of.open(svrheadfilename.c_str());

	of << "#include<iostream>"  << endl;
	of << "#include<string>" << endl;
	of << "#include\"" << string(name) << ".pb.h\"" << endl;
	of << "using namespace std;" << endl;
	of << endl;
	for (int i = 0;i < functions.size();i++)
	{
		of << "int callFunc" <<  functions[i].get_name() <<"(const string &sMsg,const int & new_fd);" << endl;
		of << endl;
	}
	of << "int process(int sockfd);" << endl; 
	of << endl;
	of.close();
	return 0;
}

int createSvrImplHeadFile(const char * name)
{
	string svrimplheadfilename = string(name) + "svrimpl.h";
	ofstream of;
	of.open(svrimplheadfilename.c_str());
	of << "#include<iostream>" << endl;
	of << "#include\"" << string(name) << ".pb.h\"" << endl;
	of << "using namespace std;" << endl;
	of << endl;
	for (int i = 0;i < functions.size();i++)
	{
		of << "int func" << functions[i].get_name() << "( const " << functions[i].get_input() << " & " << functions[i].get_input() << "_obj," << functions[i].get_output() << " & "<< functions[i].get_output() << "_obj);" << endl;
		of << endl;
	}
	of.close();
	return 0;
}

int createSvrImplFile(const char * name)
{
	string svrimplfilename = string(name) + "svrimpl.cpp";
	ofstream of;
	of.open(svrimplfilename.c_str());
	of << "#include<iostream>" << endl;
	of << "#include\"" << string(name) << ".pb.h\"" << endl;
	of << "#include\"" << string(name) << "svrimpl.h\""  << endl;
	of << "using namespace std;" << endl;
	of << endl;
	for (int i = 0;i < functions.size();i++)
	{
		of << "int func" << functions[i].get_name() << "( const " << functions[i].get_input() << " & " << functions[i].get_input() << "_obj," << functions[i].get_output() << " & "<< functions[i].get_output() << "_obj)" << endl;
		of << "{" << endl;
		of << "	/*add the impl code here*/" << endl;
		of << "	return 0;" << endl; 
		of << "}" << endl;
		of << endl;
	}
	of.close();
	return 0;
}

int createCliHeadFile(const char * name)
{
	string cliheadfilename = string(name) + "cli.h";
	ofstream of;
	of.open(cliheadfilename.c_str());

	of << "#include<iostream>" << endl;
	//of << "#include\"" << string(name) << ".pb.h\"" << endl;
	of << "using namespace std;" << endl;
	of <<  endl;
	of << "class " << string(name) << "Client" << endl;
	of << "{" << endl;
	of << "public:" << endl;
	of << "	" << string(name) << "Client(string configFile,int sockfd_tmp = 0,int iPort_tmp = 0);" << endl;
	of << "	~" << string(name) << "Client();" << endl;
	of << "	int Connect();" << endl;

	for (int i = 0;i < functions.size();i++)
	{
		of << "	int Func" << functions[i].get_name() << "(const " << functions[i].get_input() << " " << functions[i].get_input() << "_obj," << functions[i].get_output() << " &" << functions[i].get_output() << "_obj);" << endl;
	}

	of << "private:" << endl;
	of << "	string sIp;" << endl;
	of << "	int iPort;" << endl;
	of << "	int sockfd;" << endl;
	of << "	struct sockaddr_in sockaddr_in_obj;" << endl;
	of << "};" << endl;

	of.close();
	return 0;
}

int createCliPart2File(const char * name)
{
	string clipart2filename = string(name) + "cli.template.part2";
	ofstream of;
	of.open(clipart2filename.c_str());

	for (int i = 0;i < functions.size();i++)
	{
		of << "int " << string(name) << "Client::Func" << functions[i].get_name() << "(const " << functions[i].get_input() << " " << functions[i].get_input() << "_obj," << functions[i].get_output() << " &" << functions[i].get_output() << "_obj)" << endl;
		of << "{" << endl;
		of << "	FuncType functype_obj;" << endl;
		of << "	string sInput;" << endl;
		of << "	" << functions[i].get_input() << "_obj.SerializeToString(&sInput);" << endl;
		of << "	functype_obj.set_msg(sInput);" << endl;
		of << "	functype_obj.set_type(" << functions[i].get_id() <<");" << endl;
		of << "	string sTar;" << endl;
		of << "	functype_obj.SerializeToString(&sTar);" << endl;
		of << "	int iRet = send_socket(sTar,sockfd);" << endl;
		of << "	if (iRet < 0)" << endl;
		of << "	{" << endl;
		of << "		printf(\"send error\\n\");" << endl;
		of << "		return -1;" << endl;
		of << "	}" << endl;
		of << "	string sOutput;" << endl;
		of << "	iRet = recv_socket(sOutput,sockfd);" << endl;
		of << "	if (iRet > 0)" << endl;
		of << "	{" << endl;
		of << "		if (!" << functions[i].get_output() << "_obj.ParseFromString(" << "sOutput))" << endl;
		of << "		{" << endl;
		of << "			printf(\"parse error\\n\");" << endl;
		of << "			return -4;" << endl;
		of << "		}" << endl;
		of << "	}" << endl;
		of << "	else if (iRet < 0)" << endl;
		of << "	{" << endl;
		of << "		return -2;" << endl;
		of << "	}" << endl;
		of << "	else " << endl;
		of << "	{" << endl;
		of << "		printf(\"the other close\\n\");" << endl;
		of << "		return -3;" << endl;
		of << "	}" << endl;
		of << "	return 0;" << endl;
		of << "}" << endl;
		of << endl;
	}

	of.close();
	return 0;
}

int createTestCli(const char * name)
{
	string clitestfilename = "test" + string(name) + "cli.cpp";
	ofstream of;
	of.open(clitestfilename.c_str());

	of << "#include<iostream>" << endl;
	of << "#include<stdio.h>" << endl;
	of << "#include<string.h>" << endl;
	of << "#include<sys/socket.h>" << endl;
	of << "#include<netinet/in.h>" << endl;
	of << "#include<arpa/inet.h>" << endl;
	of << "#include<stdlib.h>" << endl;
	of << "#include<errno.h>" << endl;
	of << "#include<netdb.h>" << endl;
	of << "#include<unistd.h>" << endl;
	of << "#include<fstream>" << endl;
	of << "#include<string>" << endl;
	of << "#include<sstream>" << endl;
	of << "#include\"" << string(name) << ".pb.h\"" << endl;
	of << "#include\"" << string(name) << "cli.h\"" << endl;
	of << "using namespace std;" << endl;
	of << endl;
	of << "int main()" << endl;
	of << "{" << endl;
	of << "	" << string(name) << "Client " << string(name)  << "client_obj(\"" << string(name) << "client.xml\");" << endl;
	of << "	int iRet = " << string(name) << "client_obj.Connect();" << endl;
	of << "	if (iRet == -1)" << endl;
	of << "	{" << endl;
	of << "		printf(\"client connect error %d\\n\",iRet);" << endl;
	of << "		return 0;" << endl;
	of << "	}" << endl;
	of << "" << endl;
	for (int i = 0;i < functions.size();i++)
	{
		of << "	/* test Func" << functions[i].get_name() << "*/" << endl;
		of << "	/*" << endl;
		of << "	" << functions[i].get_input() << " " << functions[i].get_input() << "_obj;" << endl;
		of << "	" << functions[i].get_output() << " " <<  functions[i].get_output() << "_obj;" << endl;
		of << "	iRet = " << string(name) << "client_obj.Func" << functions[i].get_name()  << "(" << functions[i].get_input() << "_obj," << functions[i].get_output() << "_obj);" << endl;
		of << "	if (iRet != 0)" << endl;
		of << "	{" << endl;
		of << "		printf(\"func" << functions[i].get_name() << " error %d\\n\",iRet);" << endl;
		of << "		return 0;" << endl;
		of << "	}" << endl;
		of << "	*/" << endl;
		of << endl;
	}
	of << "	return 0;" << endl;
	of << "}" << endl;
	of.close();
	return 0;
}

int main(int argc,char ** argv)
{
	if (argc != 2)
	{
		printf("Usage : %s svrname\n",argv[0]);
		return 0;
	}
	int iRet = readFunctions(argv[1]);
	if (iRet != 0)
	{
		printf("read functions error : %d\n",iRet);
		return -1;
	}
	iRet = createProcessPart2(argv[1]);
	if (iRet != 0)
	{
		printf("create part3 error : %d\n",iRet);
		return -2;
	}
	iRet = createSvrFunction(argv[1]);
	if (iRet != 0)
	{
		printf("create part4 error : %d\n",iRet);
		return -2;
	}
	iRet = createSvrHeadFile(argv[1]);
	if (iRet != 0)
	{
		printf("create svr head file error : %d\n",iRet);
		return -2;
	}
	iRet = createSvrImplHeadFile(argv[1]);
	if (iRet != 0)
	{
		printf("create svr impl head file error : %d\n",iRet);
		return -2;
	}
	iRet = createSvrImplFile(argv[1]);
	if (iRet != 0)
	{
		printf("create svr impl file error : %d\n",iRet);
		return -2;
	}
	iRet = createCliHeadFile(argv[1]);
	if (iRet != 0)
	{
		printf("create cli head file error : %d\n",iRet);
		return -2;
	}
	iRet = createCliPart2File(argv[1]);
	if (iRet != 0)
	{
		printf("create cli part2 file error : %d\n",iRet);
		return -2;
	}
	iRet = createTestCli(argv[1]);
	if (iRet != 0)
	{
		printf("create cli test file error : %d\n",iRet);
		return -2;
	}

	return 0;
}
