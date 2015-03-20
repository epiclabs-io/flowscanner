// FlowScanner library
// Author: Javier Peletier <jm@friendev.com>
// Summary: Allows extracting values from a running stream, inspired in scanf
//
// Copyright (c) 2015 All Rights Reserved, http://friendev.com
//
// This source is subject to the GPLv2 license.
// Please see the License.txt file for more information.
// All other rights reserved.
//
// THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY 
// KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.


#ifndef __FlowScanner_H
#define __FlowScanner_H

#include <ACross.h>

#include <stdarg.h>

//defines a new FlowPattern object with its pattern string stored in PROGMEM
#define DEFINE_FLOWPATTERN(name, pattern) const char name##string[] PROGMEM = pattern;FlowPattern name ( name##string );

class FlowScanner;

class FlowPattern
{
	friend class FlowScanner;

	PGM_P pattern;
	
public:
	bool signaled;
	FlowPattern(PGM_P scanPattern);

};

class FlowScanner 
{
	static const uint8_t DEFAULT_DECIMAL_WIDTH = 80;
	static const uint8_t DEFAULT_WIDTH_MARKER = 0xFF;

	FlowPattern* pattern;
	PGM_P p;
	char t;
	char specifier;

	uint8_t width;
	uint8_t captureCount;
	bool enableCapture;
	int8_t sign;
	union
	{
		struct
		{
			int32_t value;
			uint8_t length;
		};
		struct
		{
			uint8_t* charPtr;
			PGM_P scanset;
			bool negatedScanset;
		};
	};


public:



	void setPattern(FlowPattern& flowPattern);


	bool scan(uint8_t c, ...);
	bool scan(uint8_t** buffer, uint16_t* length, ...);

	void reset();
	bool isSignaled();


private :
	void setPos(va_list &args, va_list init, uint8_t pos);
	bool isInScanset(uint8_t c);
	bool input(uint8_t c, va_list args);
	
	bool init_d(uint8_t c);
	void init_c(va_list &args, va_list init,uint8_t c);
	void init_scanset(uint8_t c);

	void captureInteger(va_list &args);
	char nextChar();
};


#endif