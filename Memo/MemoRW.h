#ifndef _MEMORW_h
#define _MEMORW_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "MemoCom.h"

#include "Tools.h"

class ZipCode
{
protected:

public:
	int Device = 0;   // In the range 0 - 7
					  // Memory Byte Address of the 24C04 from which data to be read
	int Memory = 0;    // In the range 0 - 255
	int Data = 0;
	String Page = "";

	int cE = 0;
	int trials = 0;
};
class binaryZipCode
{
protected:

public:
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

class MemoRWClass
{
protected:
	void prepareWords(Addrss toDo, Addrss pageOr);

	void showWords(Addrss toDo, Addrss pageOr);

	bool conversionBinary(Addrss toWord, Addrss pageOr);

	bool isWrite = false;
	bool current = false;
	bool isPage = false;

	binaryZipCode binZipCode;

public:

	ZipCode z;

	MemoComClass MemoCom;

	//Provides cell location
	void giveZipCode(int ICnumber, unsigned int address);

	//Reads a word
	void readWord();

	//Writes a word
	bool writeWord(Addrss addrs, bool hl);

	//Reads a page
	void readPage(int nroOfMemCells);
	//Writes a page
	void writePage();

	//Reads or writes a cell
	void readWriteCell(Addrss toDo, Addrss pageOr, int sizePage = 0);
};

#endif
