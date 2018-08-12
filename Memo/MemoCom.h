#ifndef _MEMOCOM_h
#define _MEMOCOM_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

////
//// WIRING = EVERYTHING TO GROUND EXCEPT
// VCC OF COURSE,
//SDA y SCL which are connected to VCC through a 10 k resistor
//lecture is done to these pins from Arduino to chip.
//A0,A1,A2 in the chip are address configurable hardWire
//

#define SDAPin 18U //18U A4
#define SCLPin 19U //19U A5

#define maxLenght 256
#define maxBits 8
#define maxICBits 3

#define tiempo 2 //funciona con 1 y 3 mil abajo, 1us
#define stoptime  3000 //funciona con 3 mil,    o 3ms

enum Chip
{
	C02 = 0,
	C04,
	C08,
	C16,
	C01
};

///MOST BASIC INNER CLASS
class MemoComClass
{
protected:

public:

	int factor = maxBits;

	enum Chip IC = C02; //;minimum value

	void scl(bool hl);
	bool sda(bool hl, bool write);

	void startStop(bool start);
	// the setup function runs once when you press reset or power the board
	void setupMemo(Chip ic);

	bool WBit(bool bitToWrite);
	bool RBit();

	bool acknowledge(bool output);
};

extern MemoComClass MemoCom;

#endif
