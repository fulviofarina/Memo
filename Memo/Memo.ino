
#include "MemoCom.h"

#include "MemoRW.h"
#include "Memo.h"

/*
 Name:		Memo.ino
 Created:	8/8/2018 21:04:32
 Author:	Fulvio
*/



//#include "MemoCom.h"
//#include "MemoRW.h"

//#define writeall 1
//#define readAll 1

// the setup function runs once when you press reset or power the board
void setup(void)
{
	Serial.begin(9600);
}

// Number of the 24C02 Device to which data to be written
// the loop function runs over and over again forever

// the loop function runs over and over again until power down or reset



void loop(void) {
	String msg = "probando";

	Memo.readWriteMsg(0, 0, false, msg, 0);
	delay(1000);
	Memo.readWriteMsg(0, 0, true, msg, msg.length());

	Memo.startt();
	Memo.readEraseAll(0, true, false);
	Memo.endt();
	delay(2000);

#if defined (writeAll)
	Memo.readEraseAll(0, false, false);
	delay(2000);
	Memo.readEraseAll(1, false, false);
	delay(2000);
	Memo.readEraseAll(2, false, false);
	delay(2000);
#endif

	Memo.startt();
	Memo.readEraseAll(0, false, true);

	Memo.endt();
	delay(2000);

#if defined (readAll)
	Memo.readEraseAll(0, false, true);
	delay(2000);

	Memo.readEraseAll(1, false, true);
	delay(2000);
	Memo.readEraseAll(2, false, true);
	delay(2000);
#endif

	Serial.println("*************");
	Serial.println();

	//	Serial.println(z.Data);
	//Serial.println(z.Page);
}

//#include "MemoRW.h"
//#include "MemoCom.h"

/*
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
	Memo.readEraseAll(0, true, false);

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
	Memo.readEraseAll(0, false, true);
}

*/









