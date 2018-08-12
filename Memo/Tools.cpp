#include "Tools.h"

void ToolsClass::init()
{
}

bool ToolsClass::doBinary(int data, int iter)
{
	int iter1 = (1 << iter);
	int result = data & iter1;
	bool r = result ? 1 : 0;

	return r;
}

int ToolsClass::power(int x, int y)
{
	if (y == 0)
	{
		return 1;
	}
	else if (y % 2 == 0)
	{
		return power(x, y / 2)*power(x, y / 2);
	}
	else
	{
		return x*power(x, y / 2)*power(x, y / 2);
	}
}
//AUTOMATIC FOR BINARY
int ToolsClass::conversionInt(bool arrBits[], int arrSize, int base = 2)
{
	int data = 0;

	for (uint8_t j = 0; j < arrSize; j++)
	{
		int bit = arrBits[j];

		data += bit * power(base, j); //conversion to unsigned int representing a char (or word)
	}
	return data;
}

//conversion from another base
unsigned int ToolsClass::conversionInt(unsigned int arrBits[], int arrSize, int base = 2)
{
	unsigned int data = 0;
	for (uint8_t j = 0; j < arrSize; j++)
	{
		unsigned int bit = arrBits[j];

		data += bit * power(base, j); //conversion to unsigned int representing a char (or word)
	}
	return data;
}