#include "MemoRW.h"
//#define writeall 1
//#define readAll 1

// Number of the 24C02 Device to which data to be written
// the loop function runs over and over again forever

struct Times
{
	unsigned long start;
	unsigned long end;
	double diff;
};

Times t;

void writeCell(int icNumber, unsigned int address, int data) {
	// Number of the 24C02 Device to which data to be written
	// Data to be written to specified memory of 24C02
	//int Data = data;       // In the range 0 - 255

	z.Data = data;
	giveZipCode(icNumber, address);
	readWriteCell(Write, Random, 0);
}

void readCell(int ICnumber, unsigned int address)
{
	giveZipCode(ICnumber, address);
	readWriteCell(Read, Random, 0);
}

//IT USES PAGES TO WRITE MESSAGES! :D NOT CELL BY CELL
//if READ then you dont give the Msg obviously but should provide its lenght
//if write you dont give the message size, it is taken from the message lenght.
void readWriteMsg(int icNumber, int page, bool read, String msg, int msgSize)

{
	z.Page = "";

	int size = msgSize;
	if (!read) size = msg.length();
	int pageCnt = page;

	//z.Memory = pageOrMem;
//	int factor = z.IC*maxBits;
	int u = 0; //for iterating the msg array
	int mB = 0; //keeps track of the iteration of msg array when write pages
				//so at every MaxBits =8 it resets the dataTxt for a new page-write
	while (u < size) //make packets of text
	{
		if (!read) z.Page += msg[u];

		//SO THIS CHANGES PAGE EVERY 16 PAGES BY CHANGING MEMO ADDRSS accordingly
		while (pageCnt >= (maxBits * 2))
		{
			page += (maxLenght - (maxBits * 2)); //i.e. +-240
			pageCnt -= (maxBits * 2); // -=16
		}

		//when a page Msg has been formed send it to the given page
		if (mB == (factor)-1)
		{
			//Serial.println(z.Page);
			giveZipCode(icNumber, page);

			if (!read)
			{
				readWriteCell(Write, Page, factor);
				z.Page = "";
			}
			else
			{
				readWriteCell(Read, Page, factor);
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
		giveZipCode(icNumber, page);
		if (!read)
		{
			readWriteCell(Write, Page, mB);
			//z.Page = "";
		}
		else
		{
			readWriteCell(Read, Page, mB);
		}
		//diff += t.diff;
	}
}

//per cell erase, starting at startadress and given by lenght...

void readErase(int icNumber, int startAddrss, int lenght, bool ramDo, bool read)
{
	int dev = icNumber;
	uint8_t i = 49; //48 is space
	for (int u = 0; u < lenght; u++)
	{
		if (ramDo) i = random(48, 57);
		else i = 49;
		//int memo =  ;

		if (!read) writeCell(dev, startAddrss + u, i);
		else readCell(dev, startAddrss + u);
	}
}

String readEraseAPage(int icNumber, int Page, int lenght, bool ramDo, bool read)
{
	uint8_t i = 49;
	String msg = "";
	int mB = 0;
	//z.Memory = Page;

	//given a page size of 16 b
	for (int u = 0; u < lenght; u++)
	{
		//if write, construct the string to overwrite, either from random or
		//"9s"
		if (!read)
		{
			if (ramDo) i = random(48, 57);
			else i = 49;
			msg += (char)i;
		}

		if (mB == factor - 1)
		{
			readWriteMsg(icNumber, Page, read, msg.c_str(), factor);
			mB = -1;
			if (!read) msg = "";
			else msg += z.Page;
			Page++;

			//delay(1000);
			//	z.Memory++;
		}
		mB++;
	}

	return msg;
}

void startt()
{
	t.start = micros();
}
void endt()
{
	t.end = micros();
	t.diff = (t.end - t.start);

	Serial.println();
	Serial.println(t.diff);
}
void readEraseAll(int iCNumber, bool ramDo, bool read)
{
	int max = (maxLenght / maxBits);
	if (IC == C08 || IC == C16) max *= 2;
	for (int i = 0; i < max; i++)
	{
		//delay(50); //with 50 works good!!!
		readEraseAPage(iCNumber, i, factor, ramDo, read);
		//	readEraseAPage(iCNumber, i, factor, ramDo, true);
			//delay(500);
	}
}

//NOT FIXED
void readCurrent(int IC)
{
	readWriteCell(Read, Current, 0);

	//readWriteCell(false, false, maxBits, true);
	Serial.println(z.Memory);
}

void testErase()
{
	readEraseAll(0, true, false);
	readEraseAll(1, true, false);
	delay(5000);

	readEraseAll(0, true, true);
	readEraseAll(1, true, true);
}

//NOT FIXED
void readCurrentloop()
{
	int i = 255;
	while (i >= 0)
	{
		readCurrent(0);
		i--;
	}
}







