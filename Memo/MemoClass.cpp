
#include "MemoClass.h"
#include "MemoRW.h"

// Number of the 24C02 Device to which data to be written
// the loop function runs over and over again forever

void MemoClass::writeCell(int icNumber, unsigned int address, int data) {
	// Number of the 24C02 Device to which data to be written
	// Data to be written to specified memory of 24C02
	//int Data = data;       // In the range 0 - 255

	MemoRW.z.Data = data;
	MemoRW.giveZipCode(icNumber, address);
	MemoRW.readWriteCell(Write, Random, 0);
}

void MemoClass::readCell(int ICnumber, unsigned int address)
{
	MemoRW.giveZipCode(ICnumber, address);
	MemoRW.readWriteCell(Read, Random, 0);
}

//IT USES PAGES TO WRITE MESSAGES! NOT CELL BY CELL
//if READ then you dont give the Msg obviously but should provide its lenght
//if write you dont give the message size, it is taken from the message lenght.
void MemoClass::readWriteMsg(int icNumber, int page, bool readMode, String msg, int msgSize)

{
	MemoRW.z.Page = "";

	int size = msgSize;
	if (!readMode) size = msg.length();
	int pageCnt = page;

	//z.Memory = pageOrMem;
	//	int factor = z.IC*maxBits;
	int u = 0; //for iterating the msg array
	int mB = 0; //keeps track of the iteration of msg array when write pages
				//so at every MaxBits =8 it resets the dataTxt for a new page-write
	while (u < size) //make packets of text
	{
		if (!readMode) MemoRW.z.Page += msg[u];

		//SO THIS CHANGES PAGE EVERY 16 PAGES BY CHANGING MEMO ADDRSS accordingly
		while (pageCnt >= (maxBits * 2))
		{
			page += (maxLenght - (maxBits * 2)); //i.e. +-240
			pageCnt -= (maxBits * 2); // -=16
		}

		//when a page Msg has been formed send it to the given page
		if (mB == (MemoCom.factor) - 1)
		{
			//Serial.println(z.Page);
			MemoRW.giveZipCode(icNumber, page);

			if (!readMode)
			{
				MemoRW.readWriteCell(Write, Page, MemoCom.factor);
				MemoRW.z.Page = "";
			}
			else
			{
				MemoRW.readWriteCell(Read, Page, MemoCom.factor);
			}

			///SUPERIMPORTANTE
			//delayMicroseconds(50); //deberia tratar de reducir esto pero lo necesita!

			mB = -1;
			page++;
			pageCnt++;
			//modificar maxBits para otras paginas mayores a 32 o mayores a 8 bits!
		}

		mB++; //after sending this is 1
		u++; //iterator
	}
	//mB--; //make it 0 again or mark the exact excess of chars
	if (mB > 0) //if the are remaining characters to send less than a full page, send them
	{
		//mB--; //send the excess character
		MemoRW.giveZipCode(icNumber, page);
		if (!readMode)
		{
			MemoRW.readWriteCell(Write, Page, mB);
			//z.Page = "";
		}
		else
		{
			MemoRW.readWriteCell(Read, Page, mB);
		}
		//diff += t.diff;
	}
}

//per cell erase, starting at startadress and given by lenght...

void MemoClass::readErase(int icNumber, int startAddrss, int lenght, bool ramDo, bool readMode)
{
	int dev = icNumber;
	uint8_t i = 49; //48 is space
	for (int u = 0; u < lenght; u++)
	{
		if (ramDo) i = random(48, 57);
		else i = 49;
		//int memo =  ;

		if (!readMode) writeCell(dev, startAddrss + u, i);
		else readCell(dev, startAddrss + u);
	}
}

String MemoClass::readEraseAPage(int icNumber, int Page, int lenght, bool ramDo, bool readMode)
{
	uint8_t i = 49;
	String msg = "";
	int mB = 0;
	//z.Memory = Page;

	//given a page size of 16 b
	for (int u = 0; u < lenght; u++)
	{
		//if write, construct the string to overwrite, either from random or
		//"49s"
		if (!readMode)
		{
			if (ramDo) i = random(48, 57);
			else i = 49;
			msg += (char)i;
		}

		if (mB == MemoCom.factor - 1)
		{
			readWriteMsg(icNumber, Page, readMode, msg.c_str(), MemoCom.factor);
			mB = -1;
			if (!readMode) msg = "";
			else msg += MemoRW.z.Page;
			Page++;
		}
		mB++;
	}

	return msg;
}

void MemoClass::startt()
{
	t.start = micros();
}
void MemoClass::endt()
{
	t.end = micros();
	t.diff = (t.end - t.start);

	Serial.println();
	Serial.println(t.diff);
}
void MemoClass::readEraseAll(int iCNumber, bool ramDo, bool readMode)
{
	int max = (maxLenght / maxBits);
	if (MemoCom.IC == C08 || MemoCom.IC == C16) max *= 2;
	for (int i = 0; i < max; i++)
	{
		readEraseAPage(iCNumber, i, MemoCom.factor, ramDo, readMode);
	}
}

//NOT FIXED
void MemoClass::readCurrent(int IC)
{
	readErase(IC, Current, 1, false, true);

	Serial.println(MemoRW.z.Memory);
}

void MemoClass::testErase()
{
	readEraseAll(0, true, false);
	readEraseAll(1, true, false);
	delay(5000);

	readEraseAll(0, true, true);
	readEraseAll(1, true, true);
}

//NOT FIXED
void MemoClass::readCurrentloop(int IC)
{
	int i = 255;
	while (i >= 0)
	{
		readCurrent(IC);
		i--;
	}
}