#include "MemoRW.h"
#include <stdio.h>
#include <stdlib.h>

void MemoRWClass::_prepareWords(Addrss toDo, Addrss pageOr)
{

	//when writing
	if (toDo == Write) //if I want to read/Write I want this, but only when random or paging. Current only when weiring
	{
		_conversionBinary(Memm, pageOr);
	}
	//when reading
	else if (pageOr != Current) _conversionBinary(Memm, pageOr);

	//always do this, mark the device
	_conversionBinary(Dev, pageOr);
	if (toDo == Write)
	{
		//make conversion of data or page to binary
		//it is supposed that the user has already given
		//device, memory and, word or full-page data through the
		//results (may vary) structure and calls to _giveZipCode fucntion (might vary nam as well)
		_conversionBinary(Info, pageOr);
	}

}
void MemoRWClass::_showWords(Addrss toDo, Addrss pageOr)
{


	results.Device = ToolsClass::conversionInt(binaryData.Device, maxICBits);

	Serial.print("@ IC=");
	Serial.print(results.Device, DEC);
	Serial.print("\t");
	if (toDo != Write)
	{
		if (pageOr == Current)
		{
			_conversionBinary(Memm, pageOr);
		}
	}

	Serial.print("M=");
	results.Memory = ToolsClass::conversionInt(binaryData.Memory, maxBits + C16);
	Serial.print(results.Memory, DEC);
	Serial.print("\t");
	if (toDo != Write)
	{
		Serial.print("R\t");
		_conversionBinary(Info, pageOr);
	}
	else Serial.print("W\t");
	Serial.print("* E" + String(results.cE));
	Serial.print(" K" +  String(results.trials - results.cE)); //print the number of errors and OK's
	Serial.print(" *\t");
	if (pageOr != Page)
	{
		results.Data = ToolsClass::conversionInt(binaryData.Data, maxBits);
		Serial.print("C=\t");
		Serial.print(results.Data, DEC);
		Serial.print(" = '");
		unsigned char c = (unsigned char)results.Data;
		Serial.print(c);
		Serial.print("'");
		//Serial.print("+");
	}
	else
	{
		Serial.print("P=\t" + results.Page);
	
	
	}
	Serial.println();
}
void MemoRWClass::_giveZipCode(unsigned int ICnumber, unsigned int address)
{
	results.Device = ICnumber;
	results.Memory = address;
}
bool MemoRWClass::_conversionBinary(Addrss toWord, Addrss pageOr) //ADDRESSING LOOP
{
	if (toWord == Memm)
	{
		for (unsigned int i = 0; i < maxBits + C16; i++)
		{
			binaryData.Memory[i] = false;
			binaryData.Memory[i] = ToolsClass::doBinary(results.Memory, i);
		}
	}
	else if (toWord == Dev)
	{
		for (unsigned int i = 0; i < maxICBits; i++)
		{
			binaryData.Device[i] = false;
			binaryData.Device[i] = ToolsClass::doBinary(results.Device, i);
		}

		binaryData.DeviceAux[4] = binaryData.Device[0];
		binaryData.DeviceAux[5] = binaryData.Device[1];
		binaryData.DeviceAux[6] = binaryData.Device[2];

		if (MemoCom.IC != C02 && MemoCom.IC != C01)
		{
			binaryData.DeviceAux[6] = binaryData.Memory[8];
			if (MemoCom.IC == C08 || MemoCom.IC == C16)
			{
				binaryData.DeviceAux[5] = binaryData.Memory[9];
			}
			if (MemoCom.IC == C16)
			{
				binaryData.DeviceAux[4] = binaryData.Memory[10];
			}
		}
	}

	else if (toWord == Info)
	{
		if (pageOr == Page)
		{
			for (unsigned int j = 0; j < maxAllowedLenght(); j++)
			{
				for (unsigned int i = 0; i < maxBits; i++) //INITIALIZES THE PAGE ARRAY
				{
					binaryData.Page[j][i] = false;
				}
			}
			binaryData.pageIter = 0; //memmory cell index in the array
			unsigned int max = results.Page.length(); //take a lenght up to maxOfBits
			for (unsigned int u = 0; u < max; u++)
			{
				unsigned int aux = (unsigned int) results.Page[u];
				for (unsigned int j = 0; j < maxBits; j++)
				{
					binaryData.Page[binaryData.pageIter][j] = ToolsClass::doBinary(aux, j);
				}
				binaryData.pageIter++;
			}
		}
		else 
		{
			for (unsigned int i = 0; i < maxBits; i++)
			{
				binaryData.Data[i] = false;
				binaryData.Data[i] = ToolsClass::doBinary(results.Data, i);
			}
		}
	}

	return true;
}
void MemoRWClass::_readWord()
{
	results.Data = 0;
	MemoCom.RArray(binaryData.Data, maxBits);
	results.Data = ToolsClass::conversionInt(binaryData.Data, maxBits);
}
//Provides read or write access to a cell
void MemoRWClass::_readWritePageOrCell(Addrss toDo, Addrss pageOr, unsigned int sizePage = 0)
{
	_prepareWords(toDo, pageOr);

	bool notok = true;

	while (notok)
	{
		MemoCom.startStop(true);
		binaryData.DeviceAux[7] = (pageOr == Current);
		notok = _writeWord(Dev);
	}

	if (toDo != Write)
	{
		if (pageOr != Current) //random memory access
		{
			notok = _writeWord(Memm); //for random

			if (notok)
			{
				//goto restart;
				//break;
				return _readWritePageOrCell(toDo, pageOr, sizePage);
			}

			notok = true;
			while (notok)
			{
				MemoCom.startStop(true);
				binaryData.DeviceAux[7] = HIGH;
				notok = _writeWord(Dev); //high to read, low to write
			}

			if (pageOr == Page) _readAPage(sizePage);
			else _readWord(); //data contains the word read
							  /// RANDOM READ (GIVEN MEMMORY ADDRESS READ)
		}
		else
		{
			_readWord(); //data contains the word read
			results.Memory = results.Data; //current address
		}
	}
	else
	{
		notok = _writeWord(Memm); //for random

		if (notok)
		{
			return _readWritePageOrCell(toDo, pageOr, sizePage);
		}

		if (pageOr == Page)
		{
			_writeAPage();
		}
		else notok = _writeWord(Info); //CURRENT OR RANDOM
	}

	if (results.cE > 0 || notok)
	{
		return _readWritePageOrCell(toDo, pageOr, sizePage);

	}
	MemoCom.startStop(false);

	_showWords(toDo, pageOr);
}
bool MemoRWClass::_writeWord(Addrss addrs) //this is always the same except the 8th bit
{
	char toShow = '0';

	switch (addrs)
	{
	case Dev:
	{
		//toShow='D';
		Serial.print('D');
		MemoCom.WArray(binaryData.DeviceAux, maxBits);
		break;
	}
	case Memm:
	{
		//toShow='C';
		Serial.print('C');
		MemoCom.WArray(binaryData.Memory, maxBits);
		break;
	}
	case Info:
	{
	//	toShow='I';
		Serial.print('I');
		MemoCom.WArray(binaryData.Data, maxBits);
		break;
	}

	}
	bool notok = MemoCom.acknowledge(false);

	if (notok)
	{
		//toShow='E';
		Serial.print('E');
	}
	else
	{
	//	toShow = ('0');
		Serial.print('0');
	}
#ifdef DEBUG
	//Serial.print(toShow);
#endif // DEBUG




	return  notok;
}
void MemoRWClass::_readAPage(unsigned int nroOfMemCells)
{
	String msg = "";
	unsigned int j = 0;

	results.cE = 0;
	results.trials = nroOfMemCells;

	while (j < nroOfMemCells)
	{
		_readWord();
		bool notok = MemoCom.acknowledge(true);
		msg += (char)results.Data;
		if (notok) results.cE++;
		j++;
	}

	results.Page = msg;
}

void binaryData::init()
{


	//bool page[maxBits * 2][maxBits];


	Device = &device[0]; //3-bit array
	DeviceAux = &deviceAux[0]; //3-bit array // leave first 4 bits alone, necessary for IC 24C0x chips
	Memory = &memory[0]; //eleven-bit array
	Data = &data[0]; //8-bit array

	Page = (bool **)malloc(sizeof(bool *) * maxBits * 2);
	for (unsigned int i = 0; i < maxBits * 2; ++i)
	{
		Page[i] = (bool *)malloc(sizeof(bool) * maxBits);

	}
	for (size_t i = 0; i < maxBits * 2; i++)
	{
		for (size_t j = 0; j < maxBits; j++)
		{
			Page[j][i] = false;
		
		}
		
	}

#ifdef  DEBUG
	for (size_t i = 0; i < maxICBits; i++)
	{
		Serial.print(Device[i]);
	}
	Serial.println();
	Serial.println();
	for (size_t i = 0; i < maxBits; i++)
	{
		Serial.print(DeviceAux[i]);
	}
	Serial.println();
	Serial.println();
	for (size_t i = 0; i < maxBits + C16; i++)
	{
		Serial.print(Memory[i]);
	}
	Serial.println();
	Serial.println();
	for (size_t i = 0; i < maxBits * 2; i++)
	{
		for (size_t j = 0; j < maxBits; j++)
		{
			Page[j][i] = false;
			Serial.print(Page[j][i]);
		}
		Serial.println();
	}
	Serial.println();
	Serial.println();

#endif //  DEBUG



}
void MemoRWClass::_writeAPage()
{

	results.trials = maxAllowedLenght();
	results.cE = 0;
	results.cE  = MemoCom.WArray(binaryData.Page, binaryData.pageIter, maxBits);

	

}

unsigned int MemoRWClass::maxAllowedLenght()
{
	return MemoCom.maxAllowedLength;
}

void MemoRWClass::setup(Chip ic, uint8_t SDAPIN = 18U, uint8_t SCLPIN = 19U)
{
	binaryData.init();
	MemoCom.setup(IC,SDAPIN,SCLPIN);

}

///////////////////////////////
/////////////////////////////////
///////////////////////////////
////////////////////////////////

void MemoRWClass::readWriteMsg(unsigned int icNumber, unsigned int page, bool readMode = true, String msg = "", unsigned int msgSize = 0U)

{
	results.Page = "";

	unsigned int sizeOfPage = maxAllowedLenght();

	unsigned int size = sizeOfPage;
	if (!readMode) size = msg.length();
	//this value remains fixed
	unsigned int initialPage = page;

	unsigned	int u = 0; //for iterating the msg array
	unsigned int mB = 0; //keeps track of the iteration of msg array when write pages


	//so at every MaxBits =8 it resets the dataTxt for a new page-write

	while (u < size) //make packets of text
	{
		//add char to write if not reading...
		if (!readMode) results.Page += msg[u];

		//SO THIS CHANGES PAGE EVERY 16 PAGES BY CHANGING MEMO ADDRSS accordingly
		while (initialPage >= totalMaxBits)
		{
			page += fixedBias; //i.e. page - 240
			initialPage -= totalMaxBits; // initialPage - 16
		}

		//when a page Msg has been formed send it to the given page
		if (mB == sizeOfPage - 1)
		{
		
			_giveZipCode(icNumber, page);

			if (!readMode)
			{
				_readWritePageOrCell(Write, Page, sizeOfPage);
				results.Page = "";
			}
			else
			{
				_readWritePageOrCell(Read, Page, sizeOfPage);
			}

			///SUPERIMPORTANTE
			//delayMicroseconds(50); //deberia tratar de reducir esto pero lo necesita!

			mB = 0;
			page++;
			initialPage++;
			//modificar maxBits para otras paginas mayores a 32 o mayores a 8 bits!
		}
		else mB++;

		u++; //iterator
	}

	//make it 0 again or mark the exact excess of chars
	if (mB > 0) //if the are remaining characters to send less than a full page, send them
	{
		//send the excess character
		_giveZipCode(icNumber, page);
		if (!readMode)
		{
			_readWritePageOrCell(Write, Page, mB);
		}
		else
		{
			_readWritePageOrCell(Read, Page, mB);
		}
	}
}

void MemoRWClass::writeCell(unsigned int icNumber, unsigned int address, unsigned int data) {
	// Number of the 24C02 Device to which data to be written
	// Data to be written to specified memory of 24C02
	//int Data = data;       // In the range 0 - 255

	results.Data = data;
	_giveZipCode(icNumber, address);

	_readWritePageOrCell(Write, Random);
}

void MemoRWClass::readCell(unsigned int ICnumber, unsigned int address)
{
	_giveZipCode(ICnumber, address);
	_readWritePageOrCell(Read, Random);
}
void MemoRWClass::readEraseAll(unsigned int iCNumber, bool readMode = true, bool ramDo = true)
{
	unsigned int maxPages = (unsigned int)(maxLenght / maxBits);
	if (MemoCom.IC == C08 || MemoCom.IC == C16) maxPages *= 2;
	for (unsigned int i = 0; i < maxPages; i++)
	{
		readEraseAPage(iCNumber, i, maxAllowedLenght(), readMode, ramDo);
	}
}

void MemoRWClass::readErase(unsigned int icNumber, unsigned int startAddrss, unsigned int numberOfCells =1, bool readMode = true, bool ramDo = true)
{
	unsigned int i; 

	for (unsigned int u = 0; u < numberOfCells; u++)
	{
		if (ramDo) i = random(48, 57);
		else i = 48;
		if (!readMode) writeCell(icNumber, startAddrss + u, i);
		else readCell(icNumber, startAddrss + u);
	}
}

String MemoRWClass::readEraseAPage(unsigned int icNumber, unsigned int page, unsigned int numberOfCells =1, bool readMode = true, bool ramDo = true)
{
	unsigned int i = 48;
	String msg = "";
	unsigned int cellCount = 0;

	//given a page size of 16 binaryData
	for (unsigned int u = 0; u < numberOfCells; u++)
	{
		//if write, construct the string to overwrite, either from random or
		//"49s"
		if (!readMode)
		{
			if (ramDo) i = random(48, 57);
			msg += (char)i;
		}

		if (cellCount == maxAllowedLenght() - 1)
		{
			readWriteMsg(icNumber, page, readMode, msg.c_str());
			cellCount = 0;
			if (!readMode) msg = "";
			else msg += results.Page;
			page++;
		}
		else cellCount++;
	}

	return msg;
}



