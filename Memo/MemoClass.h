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

	void startt();
	void endt();
	MemoRWClass MemoRW;

public:

	Times timeRegister;

	unsigned int cellValue();
	const char* MemoClass::pageValue();

	//reads the current cell
	void readCurrent(unsigned int IC);
	//writes a (string) message starting from the given page
	void writeMessage(unsigned int deviceNumber, unsigned int page, String msg);
	
	//writes to a cell
	void write(unsigned int deviceNumber, unsigned int address, unsigned int data);
	//erases a Page with a) 0's or b) random values
	void erasePage(unsigned int deviceNumber, unsigned int page, bool randomValue = false);

	//reads a page for a specific or default (max page) length
	const char * readPage(unsigned int deviceNumber, unsigned int page, unsigned int specificLength = 0);
	
	//reads cells starting from a given one
	void read(unsigned int deviceNumber, unsigned int startAddrss, unsigned int numberOfCells = 1);

	//erases cells starting from a given one
	void erase(unsigned int deviceNumber, unsigned int startAddrss, unsigned int numberOfCells = 1, bool randomValue = false);
	//reads all the content (Memory dump)
	void readAll(unsigned int deviceNumber);
	//erases all the content with a) 0's or b) random values
	void eraseAll(unsigned int deviceNumber, bool randomValue = false);

	//setups the library
	void setup(Chip ic);
};

#endif
