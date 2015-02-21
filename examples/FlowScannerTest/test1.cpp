
#include <FlowScanner.h>
#include <ACross.h>

#ifdef ACROSS_ARDUINO
#include "printf.h"
#endif

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


//char * targetDNS = "\x06\x05\x81\x80\x00\x01\x00\x03\x00\x00\x00\x00\x03\x77\x77\x77\x08\x66\x72\x69\x65\x6e\x64\x65\x76\x03\x63\x6f\x6d\x00\x00\x01\x00\x01\xc0\x0c\x00\x05\x00\x01\x00\x00\x03\xc5\x00\x0a\x07\x70\x72\x61\x65\x74\x6f\x72\xc0\x10\xc0\x2e\x00\x05\x00\x01\x00\x00\x03\xc5\x00\x06\x03\x69\x70\x31\xc0\x10\xc0\x44\x00\x01\x00\x01\x00\x00\x03\xc5\x00\x04\x55\xd6\x81\x43";

char * targetDNS = "\x79\x79\x00\x01\x00\x01\x00\x00\x03\xc5\x00\x04\x55\xd6\x81\x43";


char* targetZero = "\0abc\0test\0";
char* targetWhitespaceTest = "hello   \tworld";
char* targetWhitespaceTest2 = "hello+world";
char* targetWhitespaceNumbers = "hello     79 world";



DEFINE_FLOWPATTERN(pattern3, "HTTP/%*1d.%*1d %d%*100[^\r\n]\r\n");
DEFINE_FLOWPATTERN(testzero, "%0abc%0test%0"); 

DEFINE_FLOWPATTERN(catchDNSResponse, "%0\x01" "%0\x01" "%*4c" "%0\x04" "%4c"); // "%*1[\0]\x01" "%*1[\0]\x01" "%*4c" "%*1[\0]\x04" "%4c"

DEFINE_FLOWPATTERN(testWhitespace, "hello% world");
DEFINE_FLOWPATTERN(testWhitespaceNumbers, "hello% %d% world");



void test1()
{
#ifdef ACROSS_ARDUINO
	printf_begin();
#endif

	printf("test");
	FlowScanner flow;
	uint16_t len;


	len = 10;
	uint8_t* t = (uint8_t*)targetZero;

	flow.setPattern(testzero);
	if (flow.scan(&t, &len))
	{
		printf("match zeros\n");
	}
	


	flow.setPattern(catchDNSResponse);
	uint8_t ip[4];
	int n = 16; //180; //length of DNS capture above.
	for (char* c = targetDNS; n>0;c++,n--)
	{
		if (flow.scan(*c, ip))
		{
			printf("%d.%d.%d.%d\n", ip[0], ip[1], ip[2], ip[3]);
			break;
		}
	}
	Serial.println("test2");
	flow.setPattern(testWhitespace);

	char* target = targetWhitespaceTest;
	len = strlen(target);
	if (flow.scan(((uint8_t**)&target), &len))
	{
		printf("match whitespace\n");
	}
	Serial.println("test3");





	Serial.println("test4");

	
	uint16_t x;

	len = strlen(target3);
	t = (uint8_t*)target3;

	flow.setPattern(pattern3);
	if (flow.scan(&t, &len, &x))
	{
		printf("status = %d\n", x);
	}

	Serial.println("test5");

	t = (uint8_t*)targetWhitespaceNumbers;
	len = strlen((char*)t);

	flow.setPattern(testWhitespaceNumbers);
	if (flow.scan(&t, &len, &x))
	{
		printf("number = %d\n", x);
	}






	getchar();	
	return;


}
