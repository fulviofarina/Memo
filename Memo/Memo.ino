
#include "MemoClass.h"

/*
Name:		Memo.ino
Created:	8/8/2018 21:04:32
Author:	Fulvio
*/

 MemoClass Memo;


// the setup function runs once when you press reset or power the board
void setup(void)
{
	Serial.begin(9600);


	//This block can be invoked in setup() or loop()

	//Select the 24Cxx series chip to employ
	Chip chipy = C04;
	//Setup for the given chip
	Memo.setup(chipy);


}


void loop(void) {

	/*THE DATA IS DISPLAYED IN THE SERIAL CONSOLE*/
	/*BUT CAN BE ACCESSED PROGRAMATICALLY*/

	//1)
	// IC Number to write data. A maximum of
	// 8 devices for 24C02
	// 4 devices for 24C04
	// 2 devices for 24C08
	// 1 device for 24C16
	int deviceNumber = 0; //  IC number = 0 for my example Chip 24C04
	 //erases all EEPROM data with: a) 0s or b) random values
	bool randomValues = false; //will use 0s instead
	Memo.eraseAll(deviceNumber, randomValues);

	Serial.println();
	Serial.println();

	delay(1000);
	//2)	
	//write a message to the EEPROM
	String msg = "Testing a message";
	//page to start writing the message
	unsigned int initialPage = 0;
	//writes the message 
	//Memo.writeMessage(deviceNumber, initialPage, msg);

	Serial.println("ok");

	delay(1000);
	Serial.println();
	Serial.println();


	Serial.println("ok");

	//3)
	//reads all the EEPROM content
	//Memo.readAll(deviceNumber);
	
	delay(1000);

	Serial.println();
	Serial.println();

	//4)	
	//choose an address
	unsigned int address = 132;
	//erases the address content
	Memo.erase(deviceNumber, address);
	
	Serial.println();
	Serial.println();
	
	unsigned int address2 = 168;
	randomValues = true; //will use a random value instead of 0's
	 //erases the address content by inserting a random value on it
	Memo.erase(deviceNumber, address2, 1, randomValues);

	Serial.println();
	Serial.println();
	
	//5)	
	unsigned int address3 = 100;
	//erase the address content plus 3 more (contiguos) cells
	//with 0's
	Memo.erase(deviceNumber, address3, 3, randomValues);

	Serial.println();
	Serial.println();
	//6)
	//unsigned int address3 = 200;
	//reads the cell adddress plus 3 contiguos cells
	Memo.read(deviceNumber, address3, 3);

	Serial.println();
	Serial.println();
	//7)
	//assign a value
	unsigned int data = 164;
	unsigned int address4 = 6;
	//writes the cell adddress with the value
	Memo.write(deviceNumber, address4, data);


	Serial.println();
	Serial.println();
	//8)
	//reads a page with a specific cellcount lenght
	unsigned int pageNunmber = 10;
	unsigned int specificLength = 5;

	String result = Memo.readPage(deviceNumber, pageNunmber, specificLength);

	Serial.println();
	Serial.println();

	//9)
	//reads a page with a specific cellcount lenght
	unsigned int pageNunmber2 = 13;

	result = Memo.readPage(deviceNumber, pageNunmber2);

	Serial.println();
	Serial.println();


	Serial.println("**** END *****");
	Serial.println();


}
