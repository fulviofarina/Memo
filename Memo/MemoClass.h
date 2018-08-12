#ifndef _MEMOCLASS_h
#define _MEMOCLASS_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class Times
{
protected:

public:
	unsigned long start;
	unsigned long end;
	double diff;
};

class MemoClass
{
protected:

	//NOT FIXED
	void readCurrent(int IC);

	void testErase();

	//NOT FIXED
	void readCurrentloop(int IC);

public:

	Times t;

	void writeCell(int icNumber, unsigned int address, int data);

	void readCell(int ICnumber, unsigned int address);

	//IT USES PAGES TO WRITE MESSAGES! NOT CELL BY CELL
	//if READ then you dont give the Msg obviously but should provide its lenght
	//if write you dont give the message size, it is taken from the message lenght.
	void readWriteMsg(int icNumber, int page, bool read, String msg, int msgSize);
	//per cell erase, starting at startadress and given by lenght...

	void readErase(int icNumber, int startAddrss, int lenght, bool ramDo, bool readMode);

	String readEraseAPage(int icNumber, int Page, int lenght, bool ramDo, bool readMode);

	void startt();
	void endt();
	void readEraseAll(int iCNumber, bool ramDo, bool readMode);
};

extern MemoClass Memo;

#endif
