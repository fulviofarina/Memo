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

void MemoClass::writeMessage(unsigned int deviceNumber, unsigned int page, String msg)
{
	startt();
	MemoRW.readWriteMsg(deviceNumber, page, false, msg, 0);
	endt();
}
void MemoClass::write(unsigned int deviceNumber, unsigned int address, unsigned int data)
{
	startt();
	MemoRW.writeCell(deviceNumber, address, data);
	endt();
}

void MemoClass::read(unsigned int deviceNumber, unsigned int startAddress, unsigned int numberOfCells = 1)
{
	startt();
	MemoRW.readErase(deviceNumber, startAddress, numberOfCells, true, false);
	endt();
}
void MemoClass::erase(unsigned int deviceNumber, unsigned int startAddress, unsigned int numberOfCells = 1, bool randomValue = false)
{
	startt();
	//Serial.println("the number is");
	//Serial.println(numberOfCells);
	MemoRW.readErase(deviceNumber, startAddress, numberOfCells, false, randomValue);

	endt();
}

const char * MemoClass::readPage(unsigned int deviceNumber, unsigned int page, unsigned int specificLength = 0)
{
	startt();
	if (specificLength == 0) specificLength = MemoRW.maxAllowedLenght();
	const char * msg = MemoRW.readEraseAPage(deviceNumber, page, specificLength, true, false);
	endt();
	return msg;
}
void MemoClass::erasePage(unsigned int deviceNumber, unsigned int page, bool randomValue = false)
{
	startt();
	MemoRW.readEraseAPage(deviceNumber, page, MemoRW.maxAllowedLenght(), false, randomValue);
	endt();
}
unsigned int MemoClass::cellValue()
{
	return MemoRW.results.Data;
}
const char* MemoClass::pageValue()
{
	return MemoRW.results.Page.c_str();
}
void MemoClass::readAll(unsigned int deviceNumber)
{
	startt();
	MemoRW.readEraseAll(deviceNumber, true, false);
	endt();
}
void MemoClass::eraseAll(unsigned int deviceNumber, bool randomValue = false)
{
	startt();
	MemoRW.readEraseAll(deviceNumber, false, randomValue);
	endt();
}

//NOT FIXED
void MemoClass::readCurrent(unsigned int IC)
{
	MemoRW.readErase(IC, Current, 1, true, false);

	Serial.println(MemoRW.results.Memory);
}