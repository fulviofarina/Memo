#include "MemoCom.h"
#include "Util.h"

struct ZipCode
{
	int Device = 0;   // In the range 0 - 7
					  // Memory Byte Address of the 24C04 from which data to be read
	int Memory = 0;    // In the range 0 - 255
	int Data = 0;
	String Page = "";

	int cE = 0;
	int trials = 0;
};
struct binaryZipCode
{
	bool Device[maxICBits] = { 0, 0, 0 }; //3-bit array
	bool DeviceAux[maxBits] = { 1,0,1,0,0,0, 0, 0 }; //3-bit array // leave first 4 bits alone, necessary for IC 24C0x chips
	bool Memory[maxBits + C16] = { 0,0,0,0, 0, 0, 0, 0, 0 ,0,0 }; //eleven-bit array
	bool Data[maxBits] = { 0, 0, 0, 0, 0, 0, 0, 0 }; //8-bit array

	bool Page[maxBits * 2][maxBits];

	int pageIter = 0;
};
enum Addrss
{
	Read = 0,
	Write,
	Dev,
	Memm,
	Info,
	Page,
	Current,
	Random
};

ZipCode z;

binaryZipCode b;

bool isWrite = false;
bool current = false;
bool isPage = false;

void giveZipCode(int ICnumber, unsigned int address)
{
	/*
		if (IC != Chip::C02 && IC != Chip::C01)
		{
			while (address >= maxLenght)
			{
				address -= maxLenght;
				if (IC == Chip::C04)	ICnumber += 4;
				else ICnumber += 2;
			}
			//z.factor = 2 * maxBits;
		}
		*/
	z.Device = ICnumber;
	z.Memory = address;
	//else  z.factor = maxBits;
}

bool conversionBinary(Addrss toWord, Addrss pageOr) //ADDRESSING LOOP
{
	if (toWord == Memm)
	{
		for (int i = 0; i < maxBits + C16; i++)
		{
			b.Memory[i] = false;
			b.Memory[i] = doBinary(z.Memory, i);
		}
	}
	else if (toWord == Dev)
	{
		for (int i = 0; i < maxICBits; i++)
		{
			b.Device[i] = false;
			b.Device[i] = doBinary(z.Device, i);
		}

		b.DeviceAux[4] = b.Device[0];
		b.DeviceAux[5] = b.Device[1];
		b.DeviceAux[6] = b.Device[2];

		if (IC != C02 && IC != C01)
		{
			b.DeviceAux[6] = b.Memory[8];
			if (IC == C08 || IC == C16)
			{
				b.DeviceAux[5] = b.Memory[9];
			}
			if (IC == C16)
			{
				b.DeviceAux[4] = b.Memory[10];
			}
			//z.factor = 2 * maxBits;
		}

		//5  //A2 is the first binary digit
		//	WBit(b.Device[1]);  //6 // A1
		//WBit(b.Device[2]);  //7 //A0
	}

	else if (toWord == Info)
	{
		if (pageOr == Page)
		{
			for (int j = 0; j < factor; j++)
			{
				for (int i = 0; i < maxBits; i++) //INITIALIZES THE PAGE ARRAY
				{
					b.Page[j][i] = false;
					//	//notok = writeWord(Info, LOW);
				}
			}
			b.pageIter = 0; //memmory cell index in the array
			int max = z.Page.length(); //take a lenght up to maxOfBits
			for (int u = 0; u < max; u++)
			{
				int aux = z.Page.charAt(u);
				for (int j = 0; j < maxBits; j++)
				{
					b.Page[b.pageIter][j] = doBinary(aux, j);
				}
				b.pageIter++;
			}
		}
		else {
			for (int i = 0; i < maxBits; i++)
			{
				b.Data[i] = false;
				b.Data[i] = doBinary(z.Data, i);
			}
		}
	}

	return true;
}

void readWord()
{
	z.Data = 0;
	for (uint8_t j = 0; j < maxBits; j++)
	{
		b.Data[j] = RBit();
	}
	z.Data = conversionInt(b.Data, maxBits);
}

bool writeWord(Addrss addrs, bool hl) //this is always the same except the 8th bit
{
	//writeWordToAddress(addrs, hl);
	switch (addrs)
	{
	case Dev:
	{
		Serial.print('D');
		b.DeviceAux[7] = hl;
		//therefore for instance, if I send to device 1, it will be hardwired A2=HIGH
		// but I dont have to invert the array for other icNumbers
		//on th eother hand IF I USE IC NUMBERS I CAN GIVE PAGE NUMBERS DIRECTLY!
		//LEAVE THEREFORE IN THIS ORDER
		for (int i = 0; i < maxBits; i++)
		{
			WBit(b.DeviceAux[i]);
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
			WBit(b.Memory[i]);
		}
		break;
	}
	case Info:
	{
		Serial.print('I');
		for (int i = 0; i < maxBits; i++)
		{
			WBit(b.Data[i]);
		}
		break;
	}
	}
	bool notok = acknowledge(false);

	if (notok) {
		Serial.print('E');
		//return writeWord(addrs, hl);
	}
	else Serial.print('K');

	return  notok;
}

void readPage(int nroOfMemCells)
{
	String msg = "";
	int j = 0;

	z.cE = 0;
	z.trials = nroOfMemCells;

	//Serial.println(nroOfMemCells);
	while (j < nroOfMemCells)
	{
		readWord();
		bool notok = acknowledge(true);
		char c = z.Data;
		//Serial.print(c);
		msg += c;
		if (notok) z.cE++;
		j++;
	}

	z.Page = msg;
}
void writePage()
{
	int j = 0;
	//int cK = 0;
	z.cE = 0;
	z.trials = factor;
	//b.pageIter CHANGE THIS BACK
	for (j = 0; j < b.pageIter; j++) //page iter keeps last memmory cell index within page
	{
		//send a data memmory (cell data; memmory data)
		for (int i = 0; i < maxBits; i++)
		{
			WBit(b.Page[j][i]);
		}

		bool notok = acknowledge(false);

		if (notok) z.cE++;
	}
}

void prepareWords(Addrss toDo, Addrss pageOr)
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
void showWords(Addrss toDo, Addrss pageOr)
{
	int dH = DEC; //decimal or hex?

	z.Device = conversionInt(b.Device, maxICBits);

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
	z.Memory = conversionInt(b.Memory, maxBits + C16);
	Serial.print(z.Memory, dH);

	if (!isWrite)
	{
		Serial.print(" R- ");
		conversionBinary(Info, pageOr);
	}
	else Serial.print(" W- ");

	if (!isPage)
	{
		z.Data = conversionInt(b.Data, maxBits);
		Serial.print(z.Data, dH);
		Serial.print("+");
		//String c = "";
		//c+=(char)z.Data;
		//Serial.print(c);//why does it CRASH with this??
		//why the hell cannot this work when it encounters a "space"?
	}
	else
	{
		Serial.print("P: " + z.Page);
		Serial.print(" * E" + String(z.cE) + "K" + String(z.trials - z.cE)); //print the number of errors and OK's
	}
	Serial.println();
}
void readWriteCell(Addrss toDo, Addrss pageOr, int sizePage)
{
	prepareWords(toDo, pageOr);

	//restart:

	bool notok = true;

	while (notok)
	{
		startStop(true);
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
				startStop(true);
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
		//notok = writeWord(Dev, LOW);
		notok = writeWord(Memm, LOW); //for random

		if (notok)
		{
			//break;
		//	goto restart;
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
	startStop(false);

	/*
	if (write)
	{
		notok = true;
		while (notok)
		{
			Serial.println("is busy");
			startStop(true);
			notok = writeWord(Dev, LOW);
		}
	}
	*/

	showWords(toDo, pageOr);
}







