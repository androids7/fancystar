#include <iostream>
#define WYC_LOG
#include "wyc/util/util.h"

#pragma comment(lib,"fslog_d.lib")

using namespace wyc;

int main(int, char **)
{
	// if ansi encoding is used
	// must set locale first
	setlocale(LC_ALL,"chs");
	wyc_log_init("test_log","../../../tmp",LOG_NORMAL,false);
	
	wyc_print("user info");
	wyc_sys("system info");
	wyc_warn("warning!");
	wyc_error("this is an error");
	wyc_fatal("fatal error");
	wyc_logw(LOG_NORMAL,L"��ͨ��Ϣ");
	wyc_logw(LOG_SYS,L"ϵͳ��Ϣ");
	wyc_logw(LOG_WARN,L"������Ϣ");
	wyc_logw(LOG_ERROR,L"������Ϣ");
	wyc_logw(LOG_FATAL,L"���ش���");

	wyc_log_close();

	printf("Press [Enter] to continue\n");
	getchar();
	return 0;
}

