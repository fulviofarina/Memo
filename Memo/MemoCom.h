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

public:

	int factor;


	enum Chip IC;

	void scl(bool hl);
	bool sda(bool hl, bool write);

	void startStop(bool start);

	void setupMemo(Chip ic);

	bool WBit(bool bitToWrite);
	bool RBit();

	bool acknowledge(bool output);
};

extern MemoComClass MemoCom;

#endif