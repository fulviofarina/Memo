////
//// WIRING = EVERYTHING TO GROUND EXCEPT
// VCC OF COURSE,
//SDA y SCL which are connected to VCC through a 10 k resistor
//lecture is done to these pins from Arduino to chip.
//A0,A1,A2 in the chip are address configurable hardWire
//

#define SDAPin  A4 //18U A4
#define SCLPin  A5 //19U A5

#define maxLenght 256
#define maxBits 8
//#define maxMemBits 11
#define maxICBits 3

#define tiempo 2 //funciona con 1 y 3 mil abajo, 1us
#define stoptime  3000 //funciona con 3 mil,    o 3ms

int factor = maxBits;

enum Chip
{
	C02 = 0,
	C04,
	C08,
	C16,
	C01
};

enum Chip IC = C02; //minimum value

void scl(bool hl)
{
	//if (hl) 	pinMode(SCLPin, INPUT);
//	else {
	//	pinMode(SCLPin, OUTPUT);
	digitalWrite(SCLPin, hl);  //
//}
	delayMicroseconds(tiempo);//
}
bool sda(bool hl, bool write)
{
	bool read = false;
	if (write)
	{
		pinMode(SDAPin, OUTPUT);     // OUTPUT

		digitalWrite(SDAPin, hl);  //
	}
	else
	{
		pinMode(SDAPin, INPUT);     //

		read = digitalRead(SDAPin);
	}
	delayMicroseconds(tiempo);//
	return read;
}

void startStop(bool start)
{
	//	pinMode(SDAPin, OUTPUT);

	if (start)
	{
		scl(HIGH);

		sda(HIGH, true);

		sda(LOW, true);

		//
		scl(LOW);
	}
	else
	{
		sda(LOW, true);

		scl(HIGH);
		// STOP
		sda(HIGH, true);

		delayMicroseconds(stoptime); //
	}
	//ambos low cuando sale de aqui (Start)
}
// the setup function runs once when you press reset or power the board
void setupMemo(Chip ic) {
	pinMode(SCLPin, OUTPUT);
	IC = ic;
	if (IC != C02 && IC != C01)
	{
		factor = 2 * maxBits;
	}
	else  factor = maxBits;
}

bool WBit(bool bitToWrite)
{
	sda(bitToWrite, true);
	//Serial.print(bitToWrite);
	scl(HIGH);

	scl(LOW);

	return bitToWrite;//
}
bool RBit()
{
	//it seems I cannot use the sda function here because INPUT needs to be active before callin scl?
	bool bitRW = false;
	pinMode(SDAPin, INPUT);
	scl(HIGH);

	for (uint8_t i = 0; i < maxBits; i++)   //
	{                           //
		bitRW = digitalRead(SDAPin);//   1   MSB
	}
	scl(LOW);

	return bitRW;//
}

bool acknowledge(bool output)
{
	//sda(LOW);
	bool bit = true;
	if (output)
	{
		bit = LOW;
		sda(bit, output);
	}
	else
	{
		bit = sda(false, false);
	}
	scl(HIGH);

	scl(LOW);

	return bit;
}







