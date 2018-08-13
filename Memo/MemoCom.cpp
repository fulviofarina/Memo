#include "MemoCom.h"

void MemoComClass::scl(bool hl)
{
	digitalWrite(SCLPin, hl);

	delayMicroseconds(tiempo);
}
bool MemoComClass::sda(bool hl, bool write)
{
	bool read1 = false;
	if (write)
	{
		pinMode(SDAPin, OUTPUT);

		digitalWrite(SDAPin, hl);
	}
	else
	{
		pinMode(SDAPin, INPUT);

		read1 = digitalRead(SDAPin);
	}
	delayMicroseconds(tiempo);
	return read1;
}

void MemoComClass::startStop(bool start1)
{
	if (start1)
	{
		scl(HIGH);

		sda(HIGH, true);

		sda(LOW, true);

		scl(LOW);
	}
	else
	{
		sda(LOW, true);

		scl(HIGH);

		sda(HIGH, true);

		delayMicroseconds(stoptime);
	}
}

void MemoComClass::setup(Chip ic)
{
	pinMode(SCLPin, OUTPUT);
	IC = ic;
	if (IC != C02 && IC != C01)
	{
		maxAllowedLength = 2 * maxBits;
	}
	else  maxAllowedLength = maxBits;
}

void MemoComClass::WBit(bool bitToWrite)
{
	sda(bitToWrite, true);

	scl(HIGH);

	scl(LOW);
}
void MemoComClass::RArray(bool * therrary)
{
	for (unsigned int j = 0; j < maxBits; j++)
	{
		therrary[j] = RBit();
	}
}
void MemoComClass::WArray(bool * thearray)
{
	for (unsigned int i = 0; i < maxBits; i++)
	{
		WBit(thearray[i]);
	}
}

unsigned int MemoComClass::WArray(bool ** thearray, unsigned int pageIters)
{
	unsigned int count = 0;
	for (unsigned int j = 0; j < pageIters; j++) //page iter keeps last memmory cell index within page
	{
		//send a data memmory (cell data; memmory data)
		for (unsigned int i = 0; i < maxBits; i++)
		{
			WBit(thearray[j][i]);
		}

		bool notok = acknowledge(false);

		if (notok) count++;
	}
	return count;
}


bool MemoComClass::RBit()
{
	bool bitRW = false;
	pinMode(SDAPin, INPUT);
	scl(HIGH);

	bitRW = digitalRead(SDAPin);

	scl(LOW);

	return bitRW;
}

bool MemoComClass::acknowledge(bool output)
{
	bool bit = true;
	if (output)
	{
		bit = LOW;
		sda(bit, output);
	}
	else
	{
		bit = sda(false, false);
	}
	scl(HIGH);

	scl(LOW);

	return bit;
}