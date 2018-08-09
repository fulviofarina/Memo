#include "Memo.h"

void loopMemo(String msg)
{
	//String msg = "Bacco es mi perro y yo lo quiero mucho";

	//Serial.println(msg.length());
	//	IC = Chip::C04;

	/*
	startt();
	writeCell(0, 255, 26);
	endt();

	delay(1000);

	startt();
	readCell(0, 255);
	endt();

	delay(1000);
	*/

	/*
	writeCell(1, 255, 16);
	delay(1000);
	writeCell(0, 230, 14);
	delay(1000);
	writeCell(1, 230, 11);
	delay(1000);
	readCell(0, 255);
	delay(1000);
	readCell(1, 255);
	delay(1000);
	readCell(0, 230);
	delay(1000);
	readCell(1, 230);
	*/

	//delay(1000);
	//readCell(0, 255);
	//readCell(0, 511);
	//readCell(0, 3);

	//readEraseAPage(0, 31, 16, false, false);
	//writeCell(0, 511, 14);
	//delay(1000);

	//erase(0, 0, 512, true);
	//delay(1000);
	//erase(1, 0, 512, true);

	//	readEraseAPage(0, 31, 16, false, false);
	//delay(2000);
	//	readEraseAPage(0, 31, 16, false, true);

	readWriteMsg(0, 0, false, msg, 0);
	delay(1000);
	readWriteMsg(0, 0, true, msg, msg.length());

	startt();
	readEraseAll(0, true, false);
	endt();
	delay(2000);

#if defined (writeAll)
	readEraseAll(0, false, false);
	delay(2000);
	readEraseAll(1, false, false);
	delay(2000);
	readEraseAll(2, false, false);
	delay(2000);
#endif

	startt();
	readEraseAll(0, false, true);

	endt();
	delay(2000);

#if defined (readAll)
	readEraseAll(0, false, true);
	delay(2000);

	readEraseAll(1, false, true);
	delay(2000);
	readEraseAll(2, false, true);
	delay(2000);
#endif

	Serial.println("*************");
	Serial.println();

	//	Serial.println(z.Data);
	//Serial.println(z.Page);
}

void loopMemo2()
{
	String msg = "Voy a probar de nuevo porque esto no sirve";
	//	readWriteMsg(1, 0, false, msg,0);
	//readWriteMsg(1, 0, true, msg, 100);

	//	msg = "";
	//msg += 49;
	//	msg += 8;
	//msg = readEraseAPage(0, 0, 1024, true,true);
	//readErase(0, 0,msg.length(), true, true);

	//readEraseAll(1, false, false);
	readEraseAll(0, true, false);

	//Serial.println(msg);

	//	readEraseAPage(1, 0, 1024, false, true);
	//readEraseAll(0, true, false);

	//msg = "1234567891234567";
	//readWriteMsg(0, 42, false, msg, 0);

	//	readWriteMsg(0,0 , false, msg, 0);
	//readWriteMsg(1, 0, false, msg, 0);
	// readEraseAPage(0, 24, 16, false, true);
	delay(1000);
	//readEraseAPage(0, 19, 16, false, true);
	delay(1000);
	//delay(1000);
	//readEraseAPage(0, 14, 16, false, true);
	delay(1000);

	//readEraseAPage(0, 9, 16, false, true);
	delay(1000);

	//	readEraseAll(0, false, true);

	//readErase(0, 0, 1024, true, false);
	Serial.println(msg);

	//readEraseAll(1, true, false);

	//readEraseAll(1, false, true);
	readEraseAll(0, false, true);
}







