
#include "MemoClass.h"



//#include "MemoRW.h"
//#include "Memo.h"

/*
Name:		Memo.ino
Created:	8/8/2018 21:04:32
Author:	Fulvio
*/

 MemoClass Memo;

//#define writeall 1
//#define readAll 1

// the setup function runs once when you press reset or power the board
void setup(void)
{
	Serial.begin(9600);
	Chip chipy = C04;
	Memo.setup(chipy);
}

// Number of the 24C02 Device to which data to be written


void loop(void) {

	Memo.readEraseAll(0, false, false);


	String msg = "probando a ver que tan largo puede ser esto y si funciona pues funciona y deberian entrar todo ñame y toda papa que yo quiera que entre en este eeprom de bajo consumo y de tan precaria forma de pensar cuando pensé que tenía que hacer ésto y todavía creo que queda espacio para por lo menos 3 simbolos mas y un abecedario de oportunidades si o no? SI pues insisto que tengo que meterle mas MIERDA HORRIBLE A ESTO y no se termina de LLENAR NUEO";

	Memo.readWriteMsg(0, 0, false, msg, 0);
	//delay(1000);
	//Memo.readWriteMsg(0, 0, true, msg, msg.length());

	
	Memo.readEraseAll(0, false, true);

	delay(2000);

#if defined (writeAll)
	Memo.readEraseAll(0, false, false);
	delay(2000);
	Memo.readEraseAll(1, false, false);
	delay(2000);
	Memo.readEraseAll(2, false, false);
	delay(2000);
#endif

	

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


}

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
