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

class MemoClass
{
protected:

	//NOT FIXED
	void readCurrent(int IC);

	void testErase();
	void startt();
	void endt();
	//NOT FIXED
	void readCurrentloop(int IC);

	MemoRWClass MemoRW;

	Times timeRegister;

public:

	void writeCell(int icNumber, unsigned int address, int data);

	void readCell(int ICnumber, unsigned int address);

	void readWriteMsg(int icNumber, int page, bool read, String msg, int msgSize);
	//per cell erase, starting at startadress and given by lenght...

	void readErase(int icNumber, int startAddrss, int lenght, bool readMode = true, bool ramDo = true);

	String readEraseAPage(int icNumber, int Page, int lenght, bool readMode = true, bool ramDo = true);

	void readEraseAll(int iCNumber, bool readMode = true, bool ramDo = true);
	void setup(Chip ic);
};

#endif
