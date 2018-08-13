#ifndef _MEMOCOM_h
#define _MEMOCOM_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#define maxLenght 256
#define maxBits 8
#define maxICBits 3

#define totalMaxBits  (maxBits * 2) // i.e. 16
#define fixedBias  (maxLenght - totalMaxBits) //i.e. 256-16 = 240

#define tiempo 2
#define stoptime  3000

enum Chip
{
	C02 = 0,
	C04,
	C08,
	C16,
	C01
};

class MemoComClass
{
protected:
	//a4
	uint8_t SDAPin = 18U;
	//a5
	uint8_t SCLPin = 19U;
	void scl(bool hl);
	bool sda(bool hl, bool write);

	bool RBit();
	void WBit(bool bitToWrite);

public:

	unsigned int maxAllowedLength;

	enum Chip IC;

	bool acknowledge(bool output);

	void startStop(bool started);
	void setup(Chip ic, uint8_t SDAPIN = 18U, uint8_t SCLPIN = 19U);
	unsigned int WArray(bool**  thearray, unsigned int rowLength, unsigned int colength);
	void WArray(bool * thearray, unsigned int length);

	void RArray(bool *therrary, unsigned int length);
};

#endif