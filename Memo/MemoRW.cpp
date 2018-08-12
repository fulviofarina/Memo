#include "MemoRW.h"

void MemoRWClass::giveZipCode(int ICnumber, unsigned int address)
{
	z.Device = ICnumber;
	z.Memory = address;
}

bool MemoRWClass::conversionBinary(Addrss toWord, Addrss pageOr) //ADDRESSING LOOP
{
	if (toWord == Memm)
	{
		for (int i = 0; i < maxBits + C16; i++)
		{
			binZipCode.Memory[i] = false;
			binZipCode.Memory[i] = ToolsClass::doBinary(z.Memory, i);
		}
	}
	else if (toWord == Dev)
	{
		for (int i = 0; i < maxICBits; i++)
		{
			binZipCode.Device[i] = false;
			binZipCode.Device[i] = ToolsClass::doBinary(z.Device, i);
		}

		binZipCode.DeviceAux[4] = binZipCode.Device[0];
		binZipCode.DeviceAux[5] = binZipCode.Device[1];
		binZipCode.DeviceAux[6] = binZipCode.Device[2];

		if (MemoCom.IC != C02 && MemoCom.IC != C01)
		{
			binZipCode.DeviceAux[6] = binZipCode.Memory[8];
			if (MemoCom.IC == C08 || MemoCom.IC == C16)
			{
				binZipCode.DeviceAux[5] = binZipCode.Memory[9];
			}
			if (MemoCom.IC == C16)
			{
				binZipCode.DeviceAux[4] = binZipCode.Memory[10];
			}
		}
	}

	else if (toWord == Info)
	{
		if (pageOr == Page)
		{
			for (int j = 0; j < MemoCom.factor; j++)
			{
				for (int i = 0; i < maxBits; i++) //INITIALIZES THE PAGE ARRAY
				{
					binZipCode.Page[j][i] = false;
				}
			}
			binZipCode.pageIter = 0; //memmory cell index in the array
			int max = z.Page.length(); //take a lenght up to maxOfBits
			for (int u = 0; u < max; u++)
			{
				int aux = z.Page.charAt(u);
				for (int j = 0; j < maxBits; j++)
				{
					binZipCode.Page[binZipCode.pageIter][j] = ToolsClass::doBinary(aux, j);
				}
				binZipCode.pageIter++;
			}
		}
		else {
			for (int i = 0; i < maxBits; i++)
			{
				binZipCode.Data[i] = false;
				binZipCode.Data[i] = ToolsClass::doBinary(z.Data, i);
			}
		}
	}

	return true;
}

void MemoRWClass::readWord()
{
	z.Data = 0;
	for (uint8_t j = 0; j < maxBits; j++)
	{
		binZipCode.Data[j] = MemoCom.RBit();
	}
	z.Data = ToolsClass::conversionInt(binZipCode.Data, maxBits);
}

bool MemoRWClass::writeWord(Addrss addrs, bool hl) //this is always the same except the 8th bit
{
	switch (addrs)
	{
	case Dev:
	{
		Serial.print('D');
		binZipCode.DeviceAux[7] = hl;
		//therefore for instance, if I send to device 1, it will be hardwired A2=HIGH
		// but I dont have to invert the array for other icNumbers
		//on th eother hand IF I USE IC NUMBERS I CAN GIVE PAGE NUMBERS DIRECTLY!
		//LEAVE THEREFORE IN THIS ORDER
		for (int i = 0; i < maxBits; i++)
		{
			MemoCom.WBit(binZipCode.DeviceAux[i]);
		}
		//8

		break;
	}
	case Memm:
	{
		Serial.print('M');
		for (int i = 0; i < maxBits; i++)
		{
			//int pos = (maxBits - 1 ) - i;
			MemoCom.WBit(binZipCode.Memory[i]);
		}
		break;
	}
	case Info:
	{
		Serial.print('I');
		for (int i = 0; i < maxBits; i++)
		{
			MemoCom.WBit(binZipCode.Data[i]);
		}
		break;
	}
	}
	bool notok = MemoCom.acknowledge(false);

	if (notok) {
		Serial.print('E');
		//return writeWord(addrs, hl);
	}
	else Serial.print('K');

	return  notok;
}

void MemoRWClass::readPage(int nroOfMemCells)
{
	String msg = "";
	int j = 0;

	z.cE = 0;
	z.trials = nroOfMemCells;

	while (j < nroOfMemCells)
	{
		readWord();
		bool notok = MemoCom.acknowledge(true);
		char c = z.Data;
		//Serial.print(c);
		msg += c;
		if (notok) z.cE++;
		j++;
	}

	z.Page = msg;
}
void MemoRWClass::writePage()
{
	int j = 0;
	//int cK = 0;
	z.cE = 0;
	z.trials = MemoCom.factor;
	//binZipCode.pageIter CHANGE THIS BACK
	for (j = 0; j < binZipCode.pageIter; j++) //page iter keeps last memmory cell index within page
	{
		//send a data memmory (cell data; memmory data)
		for (int i = 0; i < maxBits; i++)
		{
			MemoCom.WBit(binZipCode.Page[j][i]);
		}

		bool notok = MemoCom.acknowledge(false);

		if (notok) z.cE++;
	}
}

void MemoRWClass::prepareWords(Addrss toDo, Addrss pageOr)
{
	//util booleans to avoid usign the enum
	// the ennum is kept for clarity of code
	isPage = (pageOr == Page);
	isWrite = (toDo == Write);
	current = (pageOr == Current);
	//when reading current, the last bit is true on device address

	//when writing
	if (isWrite) //if I want to read/Write I want this, but only when random or paging. Current only when weiring
	{
		conversionBinary(Memm, pageOr);
	}
	//when reading
	else if (!current) conversionBinary(Memm, pageOr);

	//always do this, mark the device
	conversionBinary(Dev, pageOr);
	if (isWrite)
	{
		//make conversion of data or page to binary
		//it is supposed that the user has already given
		//device, memory and, word or full-page data through the
		//ZipCode (may vary) structure and calls to giveZipCode fucntion (might vary nam as well)
		conversionBinary(Info, pageOr);
	}
	//if (Write == toDo) conversionBinary(true,page);
}
void MemoRWClass::showWords(Addrss toDo, Addrss pageOr)
{
	int dH = DEC; //decimal or hex?

	z.Device = ToolsClass::conversionInt(binZipCode.Device, maxICBits);

	Serial.print(" @");
	Serial.print(z.Device, dH);

	if (!isWrite)
	{
		//conversionBinary(Memory, pageOr);
		if (current)
		{
			conversionBinary(Memm, pageOr);
		}
	}

	Serial.print(" M-");
	z.Memory = ToolsClass::conversionInt(binZipCode.Memory, maxBits + C16);
	Serial.print(z.Memory, dH);

	if (!isWrite)
	{
		Serial.print(" R- ");
		conversionBinary(Info, pageOr);
	}
	else Serial.print(" W- ");

	if (!isPage)
	{
		z.Data = ToolsClass::conversionInt(binZipCode.Data, maxBits);
		Serial.print(z.Data, dH);
		Serial.print("+");
	}
	else
	{
		Serial.print("P: " + z.Page);
		Serial.print(" * E" + String(z.cE) + "K" + String(z.trials - z.cE)); //print the number of errors and OK's
	}
	Serial.println();
}

//Provides read or write access to a cell
void MemoRWClass::readWriteCell(Addrss toDo, Addrss pageOr, int sizePage = 0)
{
	prepareWords(toDo, pageOr);

	bool notok = true;

	while (notok)
	{
		MemoCom.startStop(true);
		notok = writeWord(Dev, current);
	}

	if (!isWrite)
	{
		if (!current) //random memory access
		{
			notok = writeWord(Memm, LOW); //for random

			if (notok)
			{
				//goto restart;
				//break;
				return readWriteCell(toDo, pageOr, sizePage);
			}

			notok = true;
			while (notok)
			{
				MemoCom.startStop(true);
				notok = writeWord(Dev, HIGH); //high to read, low to write
			}

			if (isPage) readPage(sizePage);
			else readWord(); //data contains the word read
							 /// RANDOM READ (GIVEN MEMMORY ADDRESS READ)
		}
		else
		{
			readWord(); //data contains the word read
			z.Memory = z.Data; //current address
		}
	}
	else
	{
		notok = writeWord(Memm, LOW); //for random

		if (notok)
		{
			return readWriteCell(toDo, pageOr, sizePage);
		}

		if (isPage)
		{
			writePage();
		}
		else notok = writeWord(Info, LOW); //CURRENT OR RANDOM
	}

	if (z.cE > 0 || notok)
	{
		return readWriteCell(toDo, pageOr, sizePage);
		//goto restart;
	}
	MemoCom.startStop(false);

	showWords(toDo, pageOr);
}