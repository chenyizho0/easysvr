/*************************************************************************
    > File Name: easysvr.h
    > Author: laixukai
    > Mail: laixukai@126.com 
    > Created Time: 2015年01月16日 星期五 23时49分07秒
 ************************************************************************/

#include<string>
using namespace std;

class FUNCTION
{
public:
	FUNCTION()
	{
		FUNCTION_ID = 0;
	}
	FUNCTION(string function_name,string input,string output,int function_id):FUNCTION_NAME(function_name),INPUT(input),OUTPUT(output),FUNCTION_ID(function_id)
	{

	}
	string get_name() const
	{
		return FUNCTION_NAME;
	}
	string get_input() const
	{
		return INPUT;
	}
	string get_output() const
	{
		return OUTPUT;
	}
	int get_id() const
	{
		return FUNCTION_ID;
	}
private:
	string FUNCTION_NAME;
	string INPUT;
	string OUTPUT;
	int FUNCTION_ID;
};
