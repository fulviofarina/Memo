#include "MemoClass.h"

// Number of the 24C02 Device to which data to be written
// the loop function runs over and over again forever

void MemoClass::writeCell(int icNumber, unsigned int address, int data) {
	// Number of the 24C02 Device to which data to be written
	// Data to be written to specified memory of 24C02
	//int Data = data;       // In the range 0 - 255

	MemoRW.z.Data = data;
	MemoRW.giveZipCode(icNumber, address);

	MemoRW.readWriteCell(Write, Random);
}

void MemoClass::readCell(int ICnumber, unsigned int address)
{
	MemoRW.giveZipCode(ICnumber, address);
	MemoRW.readWriteCell(Read, Random);
}

void MemoClass::setup(Chip ic)
{
	MemoRW.MemoCom.setup(ic);
}
//IT USES PAGES TO WRITE MESSAGES! NOT CELL BY CELL
//if READ then you dont give the Msg obviously but should provide its lenght
//if write you dont give the message size, it is taken from the message lenght.
void MemoClass::readWriteMsg(int icNumber, int page, bool readMode, String msg, int msgSize)

{
	MemoRW.z.Page = "";

	int size = msgSize;
	if (!readMode) size = msg.length();
	//this value remains fixed
	int initialPage = page;

	int u = 0; //for iterating the msg array
	int mB = 0; //keeps track of the iteration of msg array when write pages

	int sizeOfPage = MemoRW.MemoCom.factor;
	//so at every MaxBits =8 it resets the dataTxt for a new page-write

	while (u < size) //make packets of text
	{
		//add char to write if not reading...
		if (!readMode) MemoRW.z.Page += msg[u];

		//SO THIS CHANGES PAGE EVERY 16 PAGES BY CHANGING MEMO ADDRSS accordingly
		while (initialPage >= totalMaxBits)
		{
			page += fixedBias; //i.e. page - 240
			initialPage -= totalMaxBits; // initialPage - 16
		}

		//when a page Msg has been formed send it to the given page
		if (mB == sizeOfPage - 1)
		{
			//Serial.println(z.Page);
			MemoRW.giveZipCode(icNumber, page);

			if (!readMode)
			{
				MemoRW.readWriteCell(Write, Page, sizeOfPage);
				MemoRW.z.Page = "";
			}
			else
			{
				MemoRW.readWriteCell(Read, Page, sizeOfPage);
			}

			///SUPERIMPORTANTE
			//delayMicroseconds(50); //deberia tratar de reducir esto pero lo necesita!

			mB = -1;
			page++;
			initialPage++;
			//modificar maxBits para otras paginas mayores a 32 o mayores a 8 bits!
		}

		mB++; //after sending this is 1
		u++; //iterator
	}

	//make it 0 again or mark the exact excess of chars
	if (mB > 0) //if the are remaining characters to send less than a full page, send them
	{
		//send the excess character
		MemoRW.giveZipCode(icNumber, page);
		if (!readMode)
		{
			MemoRW.readWriteCell(Write, Page, mB);
		}
		else
		{
			MemoRW.readWriteCell(Read, Page, mB);
		}
	}
}

void MemoClass::readErase(int icNumber, int startAddrss, int lenght, bool readMode = true, bool ramDo = true)
{
	int dev = icNumber;
	uint8_t i = 49; //48 is space
	for (int u = 0; u < lenght; u++)
	{
		if (ramDo) i = random(49, 57);
		else i = 49;

		if (!readMode) writeCell(dev, startAddrss + u, i);
		else readCell(dev, startAddrss + u);
	}
}

String MemoClass::readEraseAPage(int icNumber, int Page, int lenght, bool readMode = true, bool ramDo = true)
{
	startt();

	uint8_t i = 49;
	String msg = "";
	int mB = 0;

	//given a page size of 16 binZipCode
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

		if (mB == MemoRW.MemoCom.factor - 1)
		{
			readWriteMsg(icNumber, Page, readMode, msg.c_str(), MemoRW.MemoCom.factor);
			mB = -1;
			if (!readMode) msg = "";
			else msg += MemoRW.z.Page;
			Page++;
		}
		mB++;
	}

	endt();

	return msg;
}

void MemoClass::startt()
{
	timeRegister.started = micros();
}
void MemoClass::endt()
{
	timeRegister.ended = micros();
	timeRegister.elapsedMiliseconds = (timeRegister.ended - timeRegister.started)*1e-3;
}
void MemoClass::readEraseAll(int iCNumber, bool readMode = true, bool ramDo = true)
{
	startt();
	int max = (maxLenght / maxBits);
	if (MemoRW.MemoCom.IC == C08 || MemoRW.MemoCom.IC == C16) max *= 2;
	for (int i = 0; i < max; i++)
	{
		readEraseAPage(iCNumber, i, MemoRW.MemoCom.factor, readMode, ramDo);
	}
	endt();
}

//NOT FIXED
void MemoClass::readCurrent(int IC)
{
	readErase(IC, Current, 1, true, false);

	Serial.println(MemoRW.z.Memory);
}

void MemoClass::testErase()
{
	readEraseAll(0, false);
	readEraseAll(1, false);
	delay(5000);

	readEraseAll(0, true);
	readEraseAll(1, true);
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