
#include "stdafx.h"

#include "../../FlowScanner.h"

char* target1 = "Content-Length:150";
FlowPattern pattern1(PSTR("%10[^:]%d"));

char* target2 = "a-250lala";
FlowPattern pattern2(PSTR("%3hd"));

char* target3 = "HTTP/1.0 200 OK\r\n"
"Date: Fri, 23 Jan 2015 09:14:43 GMT\r\n"
"Server: TinyWeb/1.93\r\n"
"Content-Length: 121\r\n"
"Content-Type: text/html\r\n"
"Last-Modified: Fri, 17 May 2013 00:55:37 GMT\r\n\r\n"
"<!DOCTYPE html>\r\n"
"<html>\r\n"
"<head>\r\n"
" <title>Hello world</title>\r\n"
""
"</head>\r\n"
"<body>\r\n"
"<h1> Hello world!! </h1>"
"</body>\r\n"
"</html>\r\n";

//FlowPattern pattern3(PSTR("HTTP/%*1d.%*1d %d%*100[^\r\n]\r\n"));

DEFINE_FLOWPATTERN(pattern3, "HTTP/%*1d.%*1d %d%*100[^\r\n]\r\n");

char text[3];
char text2[10];

int _tmain(int argc, _TCHAR* argv[])
{

	FlowScanner flow;

	uint8_t d;



	//for (uint16_t i = 0; i < strlen(target1); i++)
	//{
	//	if (flow.input(target1[i], text2,&b))
	//	{
	//		printf("%s ", text2);
	//		printf("%d\n", b);
	//	}
	//}

	flow.setPattern(pattern2);

	for (uint16_t i = 0; i < strlen(target2); i++)
	{
		if (flow.scan(target2[i], &d))
		{
			printf("%d\n", d);
			break;
		}
	}
	getchar();
	flow.reset();

	uint16_t len = strlen(target2);
	uint8_t* t = (uint8_t*)target2;

	if (flow.scan(&t, &len, &d))
	{
		printf("%d\n", d);
	}


	uint16_t status;
	len = strlen(target3);
	t = (uint8_t*)target3;

	flow.setPattern(pattern3);
	if (flow.scan(&t, &len, &status))
	{
		printf("status = %d\n", status);
	}




	getchar();

	return 0;
}
