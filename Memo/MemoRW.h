#ifndef _MEMORW_h
#define _MEMORW_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "MemoCom.h"

#include "Tools.h"


//#define DEBUG 1

class Results
{
protected:


public:
	unsigned int Device = 0;   // In the range 0 - 7
					  // Memory Byte Address of the 24C04 from which data to be read
	unsigned int Memory = 0;    // In the range 0 - 255
	unsigned int Data = 0;
	String Page = "";

	unsigned	int cE = 0;
	unsigned int trials = 0;
};
class binaryData
{
protected:
	bool device[maxICBits] = { 0, 0, 0 };

	bool deviceAux[maxBits] = { 1,0,1,0,0,0, 0, 0 }; //3-bit array // leave first 4 bits alone, necessary for IC 24C0x chips
	bool memory[maxBits + C16] = { 0,0,0,0, 0, 0, 0, 0, 0 ,0,0 }; //eleven-bit array
	bool data[maxBits] = { 0, 0, 0, 0, 0, 0, 0, 0 }; //8-bit array

public:
	
	void init();
	bool* Device;  //3-bit array
	bool* DeviceAux;//  
	bool* Memory;//  //eleven-bit array
	bool* Data;//  //8-bit array
	bool** Page;// [maxBits * 2][maxBits];

	unsigned int pageIter = 0;
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

	//prepare words
	void _prepareWords(Addrss toDo, Addrss pageOr);

	//show serial results
	void _showWords(Addrss toDo, Addrss pageOr);

	//converts
	bool _conversionBinary(Addrss toWord, Addrss pageOr);
	
    //Reads a page
	void _readAPage(unsigned int nroOfMemCells);
	//Writes a page
	void _writeAPage();
	//Reads a word
	void _readWord();
	//Writes a word
	bool _writeWord(Addrss addrs);

	//binary data
	binaryData binaryData;
	//Provides cell location
	void _giveZipCode(unsigned int ICnumber, unsigned int address);
	//Reads or writes a cell
	void _readWritePageOrCell(Addrss toDo, Addrss pageOr, unsigned int sizePage = 0);

	MemoComClass MemoCom;

public:

	void writeCell(unsigned int icNumber, unsigned int address, unsigned int data);
	void readCell(unsigned int ICnumber, unsigned int address);

	void readEraseAll(unsigned int iCNumber, bool readMode = true, bool ramDo = true);
	void readErase(unsigned int icNumber, unsigned int startAddrss, unsigned int numberOfCells =1, bool readMode = true, bool ramDo = true);

	unsigned int maxAllowedLenght();
	String readEraseAPage(unsigned int icNumber, unsigned int page, unsigned int numberOfCells =1, bool readMode = true, bool ramDo = true);
	Results results;
	void setup(Chip ic, uint8_t SDAPIN = 18U, uint8_t SCLPIN = 19U);
	

	//another group
	void readWriteMsg(unsigned int icNumber, unsigned int page, bool readMode = true, String msg = "", unsigned int msgSize = 0);


};

#endif
