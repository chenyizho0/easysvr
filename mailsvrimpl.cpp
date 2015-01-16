#include<iostream>
#include"mail.pb.h"
#include"mailsvrimpl.h"
using namespace std;

int funcSendMail( const MailMsg & MailMsg_obj,SendMailReturnMsg & SendMailReturnMsg_obj)
{
	/*add the impl code here*/
	return 0;
}

int funcCheckId( const SenderMsg & SenderMsg_obj,CheckIdReturnMsg & CheckIdReturnMsg_obj)
{
	CheckIdReturnMsg_obj.set_error_code(-100);
	return 0;
}

