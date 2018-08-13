#ifndef _MEMOCOM_h
#define _MEMOCOM_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

//a4
#define SDAPin 18U
//a5
#define SCLPin 19U

#define maxLenght 256
#define maxBits 8
#define maxICBits 3

#define totalMaxBits  (maxBits * 2) // i.e. 16
#define fixedBias  (maxLenght - totalMaxBits) //i.e. 256-16 = 240

#define tiempo 3
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
	void scl(bool hl);
	bool sda(bool hl, bool write);

	bool RBit();
	void WBit(bool bitToWrite);

public:

	unsigned int maxAllowedLength;

	enum Chip IC;

	bool acknowledge(bool output);

	void startStop(bool started);

	void setup(Chip ic);
	unsigned int WArray(bool**  thearray, unsigned int rowLength);
	void WArray(bool * thearray);

	void RArray(bool *therrary);
	
};

#endif