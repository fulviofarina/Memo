#include <stdio.h>
#include <math.h>
#include "MemoCom.h"

void MemoComClass::scl(bool hl)
{
	digitalWrite(SCLPin, hl);  //

	delayMicroseconds(tiempo);//
}
bool MemoComClass::sda(bool hl, bool write)
{
	bool read = false;
	if (write)
	{
		pinMode(SDAPin, OUTPUT);     // OUTPUT

		digitalWrite(SDAPin, hl);  //
	}
	else
	{
		pinMode(SDAPin, INPUT);     //

		read = digitalRead(SDAPin);
	}
	delayMicroseconds(tiempo);//
	return read;
}

void MemoComClass::startStop(bool start)
{
	if (start)
	{
		scl(HIGH);

		sda(HIGH, true);

		sda(LOW, true);

		//
		scl(LOW);
	}
	else
	{
		sda(LOW, true);

		scl(HIGH);
		// STOP
		sda(HIGH, true);

		delayMicroseconds(stoptime); //
	}
	//ambos low cuando sale de aqui (Start)
}
// the setup function runs once when you press reset or power the board
void MemoComClass::setupMemo(Chip ic) {
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

	return bitToWrite;//
}
bool MemoComClass::RBit()
{
	bool bitRW = false;
	pinMode(SDAPin, INPUT);
	scl(HIGH);

	for (uint8_t i = 0; i < maxBits; i++)   //
	{                           //
		bitRW = digitalRead(SDAPin);//   1   MSB
	}
	scl(LOW);

	return bitRW;//
}

bool MemoComClass::acknowledge(bool output)
{
	//sda(LOW);
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