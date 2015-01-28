
#include <FlowScanner.h>


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



DEFINE_FLOWPATTERN(pattern3, "HTTP/%*1d.%*1d %d%*100[^\r\n]\r\n");


void test1()
{


	FlowScanner flow;

	uint16_t len;
	uint8_t* t;

	uint16_t status;

	len = strlen(target3);
	t = (uint8_t*)target3;

	flow.setPattern(pattern3);
	if (flow.scan(&t, &len, &status))
	{
		printf("status = %d\n", status);
	}
	
	


}
