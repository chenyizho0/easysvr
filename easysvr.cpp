/*************************************************************************"<<endl;
    > File Name: easysvr.cpp"<<endl;
    > Author: laixukai"<<endl;
    > Mail: laixukai@126.com "<<endl;
    > Created Time: 2015年01月15日 星期四 08时59分18秒"<<endl;
 ************************************************************************/
#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include"easysvr.h"
using namespace std;

vector<FUNCTION> functions;

int createSvrFile(const char * name)
{
	string svrfilename = string(name) + "svr.cpp";
	ofstream of;
	of.open(svrfilename.c_str());
	of.close();
	return 0;
}

int createCliFile(const char * name)
{
	string clifilename = string(name) + "cli.cpp";
	ofstream of;
	of.open(clifilename.c_str());
	of.close();
	return 0;
}

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

int createPart3(const char * name)
{
	string part3filename = string(name) + "svr.template.part3";
	ofstream of;
	of.open(part3filename.c_str());
	for (int i = 0;i < functions.size();i++)
	{
		of << "				case " << functions[i].get_id() << ":" << endl;
		of << "				{" << endl;
		of << "					iRet = callFunc" << functions[i].get_name() << "(sMsg,new_fd);" << endl;
		of << "					if (iRet != 0)" << endl;
		of << "					{" << endl;
		of << "						printf(\"callFunc" << functions[i].get_name() << " error : %d\\n\",iRet);" << endl;
		of << "					}" << endl;
		of << "					break;" << endl;
		of << "				}" << endl;
	}
	of.close();
	return 0;
}

int createPart4(const char * name)
{
	string part3filename = string(name) + "svr.template.part4";
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
	iRet = createPart3(argv[1]);
	if (iRet != 0)
	{
		printf("create part3 error : %d\n",iRet);
		return -2;
	}
	iRet = createPart4(argv[1]);
	if (iRet != 0)
	{
		printf("create part4 error : %d\n",iRet);
		return -2;
	}
	iRet = createSvrHeadFile(argv[1]);
	if (iRet != 0)
	{
		printf("create svr head file error : %d\n",iRet);
	}
	/*int iRet = createSvrFile(argv[1]);
	if (iRet != 0)
	{
		printf("create svr file error\n");
		return -1;
	}
	*/
}
