#include "MemoCom.h"

void MemoComClass::scl(bool hl)
{
	if (hl)
	{
		pinMode(SCLPin, INPUT);
	}
	else
	{
		pinMode(SCLPin, OUTPUT);
		digitalWrite(SCLPin, hl);
	}
	delayMicroseconds(tiempo);
}
bool MemoComClass::sda(bool hl, bool write)
{
	bool read1 = false;
	if (write)
	{
		//	pinMode(SDAPin, OUTPUT);
		if (hl)
		{
			pinMode(SDAPin, INPUT);
		}
		else
		{
			pinMode(SDAPin, OUTPUT);
			digitalWrite(SDAPin, hl);
		}
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

void MemoComClass::setup(Chip ic, uint8_t SDAPIN = 18U, uint8_t SCLPIN = 19U)
{
	SDAPin = SDAPIN;
	SCLPin = SCLPIN;
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
void MemoComClass::RArray(bool * therrary, unsigned int length)
{
	for (unsigned int j = 0; j < length; j++)
	{
		therrary[j] = RBit();
	}
}
void MemoComClass::WArray(bool * thearray, unsigned int length)
{
	for (unsigned int i = 0; i < length; i++)
	{
		WBit(thearray[i]);
	}
}

unsigned int MemoComClass::WArray(bool ** thearray, unsigned int pageIters, unsigned int colength)
{
	unsigned int count = 0;
	for (unsigned int j = 0; j < pageIters; j++) //page iter keeps last memmory cell index within page
	{
		//send a data memmory (cell data; memmory data)
		for (unsigned int i = 0; i < colength; i++)
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