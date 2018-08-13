#include "MemoClass.h"


void MemoClass::startt()
{
	timeRegister.started = micros();
}
void MemoClass::endt()
{
	timeRegister.ended = micros();
	timeRegister.elapsedMiliseconds = (timeRegister.ended - timeRegister.started)*1e-3;
}

void MemoClass::setup(Chip ic)
{

	MemoRW.setup(ic);
}

void MemoClass::writeMessage(unsigned int icNumber, unsigned int page, String msg)
{
	startt();
	MemoRW.readWriteMsg(icNumber, page, false, msg, 0);
	endt();
}
void MemoClass::write(unsigned int icNumber, unsigned int address, unsigned int data)
{
	startt();
	MemoRW.writeCell(icNumber, address, data);
	endt();
}


void MemoClass::read(unsigned int icNumber, unsigned int startAddress, unsigned int numberOfCells = 1)
{
	startt();
	MemoRW.readErase(icNumber, startAddress, numberOfCells, true, false);
	endt();
}
void MemoClass::erase(unsigned int icNumber, unsigned int startAddress, unsigned int numberOfCells = 1, bool randomValue = false)
{
	startt();
	//Serial.println("the number is");
	//Serial.println(numberOfCells);
	MemoRW.readErase(icNumber, startAddress, numberOfCells, false, randomValue);
	
	endt();
}

String MemoClass::readPage(unsigned int icNumber, unsigned int page, unsigned int specificLength = 0)
{
	startt();
	if (specificLength == 0) specificLength = MemoRW.maxAllowedLenght();
	String msg = MemoRW.readEraseAPage(icNumber, page, specificLength, true, false);
	endt();
	return msg;
}
void MemoClass::erasePage(unsigned int icNumber, unsigned int page, bool randomValue = false)
{
	startt();
	MemoRW.readEraseAPage(icNumber, page, MemoRW.maxAllowedLenght(), false, randomValue);
	endt();
}


void MemoClass::readAll(unsigned int iCNumber)
{
	startt();
	MemoRW.readEraseAll(iCNumber, true, false);
	endt();
}
void MemoClass::eraseAll(unsigned int iCNumber, bool randomValue = false)
{
	startt();
	MemoRW.readEraseAll(iCNumber, false, randomValue);
	endt();
}

//NOT FIXED
void MemoClass::readCurrent(unsigned int IC)
{
	MemoRW.readErase(IC, Current, 1, true, false);

	Serial.println(MemoRW.results.Memory);
}