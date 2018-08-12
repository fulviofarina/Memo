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
		factor = 2 * maxBits;
	}
	else  factor = maxBits;
}

bool MemoComClass::WBit(bool bitToWrite)
{
	sda(bitToWrite, true);

	scl(HIGH);

	scl(LOW);

	return bitToWrite;
}
bool MemoComClass::RBit()
{
	bool bitRW = false;
	pinMode(SDAPin, INPUT);
	scl(HIGH);

	for (uint8_t i = 0; i < maxBits; i++)
	{
		bitRW = digitalRead(SDAPin);
	}
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