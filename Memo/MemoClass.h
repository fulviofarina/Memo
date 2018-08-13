#ifndef _MEMOCLASS_h
#define _MEMOCLASS_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "MemoCom.h"
#include "MemoRW.h"

class Times
{
protected:

public:
	unsigned long started;
	unsigned long ended;
	double elapsedMiliseconds;
};


//Wrapper Class for end-user EEPROM manipulation
class MemoClass
{
protected:

	//NOT FIXED
	void readCurrent(unsigned int IC);

	
	void startt();
	void endt();
	
	MemoRWClass MemoRW;

	Times timeRegister;

	

public:

	void writeMessage(unsigned int icNumber, unsigned int page, String msg);
	void write(unsigned int icNumber, unsigned int address, unsigned int data);
	void erasePage(unsigned int icNumber, unsigned int page, bool randomValue = false);

//	unsigned int * ConArray;

	String readPage(unsigned int icNumber, unsigned int page, unsigned int specificLength = 0);

	void read(unsigned int icNumber, unsigned int startAddrss, unsigned int numberOfCells =1);

	void erase(unsigned int icNumber, unsigned int startAddrss, unsigned int numberOfCells =1, bool randomValue = false);
	void readAll(unsigned int icNumber);
	void eraseAll(unsigned int icNumber, bool randomValue = false);

	
	void setup(Chip ic);
};

#endif
