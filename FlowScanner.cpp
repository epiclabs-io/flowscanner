

#include "FlowScanner.h"

FlowPattern::FlowPattern(PGM_P scanPattern) : pattern(scanPattern)
{}



void FlowScanner::setPattern(FlowPattern& flowPattern)
{
	pattern = &flowPattern;
	reset();
}

void FlowScanner::setPos(va_list &args, va_list init, uint8_t pos)
{
	va_copy(args, init);
	for (uint8_t i = 1; i < pos; i++)
		va_arg(args, uint8_t*);

}

bool FlowScanner::isInScanset(uint8_t c)
{
	char x=pgm_read_byte(scanset);
	for (PGM_P s = scanset ; x != ']'; x=pgm_read_byte(++s))
	{
		if (c == x)
			return !negatedScanset;
	}

	return negatedScanset;

}


int8_t isSign(uint8_t c)
{
	return c == '-' ? -1 : (c == '+' ? 1 : 0);
}

bool FlowScanner::init_d(uint8_t c)
{
	sign = isSign(c);
	if (isdigit(c) || sign != 0)
	{


		if (width == DEFAULT_WIDTH_MARKER)
			width = DEFAULT_DECIMAL_WIDTH;
		specifier = t;
		value = 0;

		if (sign != 0)
			return true; //consume character
	}
	
	return false;
	
}

void FlowScanner::init_c(va_list &args, va_list init,uint8_t c)
{
	if (width == DEFAULT_WIDTH_MARKER)
		width = 1;

	if (specifier == 0)
		specifier = t;

	if (captureCount)
	{
		setPos(args, init, captureCount);
		charPtr = va_arg(args, uint8_t*);
		captureCount++;
	}
}
void FlowScanner::init_scanset(uint8_t c)
{
	specifier = t;
	nextChar();
	if (t == '^')
	{
		negatedScanset = true;
		nextChar();
	}
	else
		negatedScanset = false;

	scanset = p;

	while (t != ']')
		nextChar();

	
}

void FlowScanner::captureInteger(va_list& args)
{
	switch (length)
	{
		case 1:*va_arg(args, int8_t*) = (sign == -1) ? -((int8_t)value) : (int8_t)value; break;
		case 2:*va_arg(args, int16_t*) = (sign == -1) ? -((int16_t)value) : (int32_t)value; break;
		case 4:*va_arg(args, int32_t*) = (sign == -1) ? -((int32_t)value) : (int32_t)value; break;

		default:
			break;
	}
}


bool FlowScanner::input(uint8_t c, va_list& args)
{
	if (isSignaled())
		return true;

	va_list init;

	va_copy(init, args);
	bool ret = false;
	bool consumed = false;
	bool r = false;


	do
	{
		if (t == '%' && specifier == 0)
		{
			nextChar();


			if (t != '%')
			{
				if (t == '*')
				{
					captureCount = 0;
					nextChar();
				}
				else
					captureCount = 1;


				width = DEFAULT_WIDTH_MARKER;
				if (isdigit((int)t))
				{
					width = 0;
					while (isdigit((int)t))
					{
						width = width * 10 + (t - '0');
						nextChar();
					}
				}

				if (width == 0) // %0 detected
				{
					if (c == '\0')
					{
						consumed = true;
						ret = (t == 0);
						continue;
					}
					else
						goto input_fail;
				}
				else
				{
					length = 2;
					specifier = 0;


					do
					{
						switch (t)
						{
							case 'd': consumed = init_d(c); break;

							case '[': init_scanset(c); //fall back to 'c'
							case 'c': init_c(args, init, c); break;
							case ' ':specifier = t; break;

							case 'h':
							{
								length = 1;
							}; break;

							case 'w':
							{
								length = 4;
							}; break;

							default:
								break;
						}
						nextChar();
					} while (t != 0 && specifier == 0);
				}


			}

		}

		if (consumed)
			break;

		if (specifier != 0)
		{
			switch (specifier)
			{
				case 'd':
				{
					if (isdigit(c) && width > 0)
					{
						width--;
						value = value * 10 + (c - '0');
						consumed = true;
						if (width > 0)
							continue;
					}

					if (captureCount)
					{
						setPos(args, init, captureCount);
						captureCount++;
						
						captureInteger(args);

					}
					
					specifier = 0;

				}break;


				case '[':
				{
					if (!isInScanset(c) || width==0)
					{
						if (captureCount)
							*charPtr = 0;

						specifier = 0;
						break;
					}
				} // fall down to 'c'

				case 'c':
				{
					if (width > 0)
					{
						width--;
						consumed = true;
						if (captureCount)
						{
							*charPtr = c;
							charPtr++;
						}
						if (width > 0)
							continue;
					}
					else
						specifier = 0;
				}break;
				
				case ' ':
				{
					if (c == ' ' || c == '\t')
						consumed = true;
					else
						specifier = 0;
				}break;


				default:
					break;
			}

			if (t == 0)
			{
				ret = true;
				break;
			}
			else
				continue;

		}

		if (t == c && (t != 0))
		{
			nextChar();
			if (t == 0)
				ret = true;

			break;

		}

	input_fail:
		if (r)
			break;
			
		reset();
		r = true;
	
	}while (!consumed);

	va_end(init);

	if (ret)
		pattern->signaled = true;

	return ret;
}

void FlowScanner::reset()
{
	p = pattern->pattern-1;
	nextChar();
	specifier = 0;
	captureCount = 0;
	pattern->signaled = 0;

}


bool FlowScanner::scan(uint8_t c, ...)
{
	va_list args;
	va_start(args, c);
	
	bool ret = input(c, args);

	va_end(args);
	
	return ret;
}

bool FlowScanner::scan(uint8_t** buffer, uint16_t* length, ...)
{
	va_list args;
	va_start(args, length);

	bool ret;
	while ((*length)-- && !(ret = input(*((*buffer)++), args)));

	va_end(args);

	return ret;
}

bool FlowScanner::isSignaled()
{
	return pattern->signaled;
}

char FlowScanner::nextChar()
{
	p++;
	return t = pgm_read_byte(p);
}