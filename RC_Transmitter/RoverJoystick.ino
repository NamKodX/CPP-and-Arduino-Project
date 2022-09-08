/*
  NamKodX 01/02/03
  Arduino NRF24L01 Transmitter - Joystick Controller Test
*/
// include these libraries
// #include <stduint8_t const.h>
// #include <avr/uint8_t consterrupt.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>
#include <Wire.h>
//#include <MPU6050.h>
//MPU6050 mpu;
// declaration of RF24 controller pins
#define joy1_X 			A1 	// joystick_1 x axis (VRx)
#define joy1_Y 			A0 	// joystick_1 y axis (VRy)
#define joy1_button A4 	// joystick_1 button
#define joy2_X 			A3 	// joystick_2 x axis (VRx)
#define joy2_Y 			A2 	// joystick_2 y axis( VRy)
#define joy2_button A5 	// joystick-2 button
#define pot_1 			A6 	// potansiyometer_1
#define pot_2 			A7 	// potansiyometer_2
#define switch_1 		2 	// switch_1
#define switch_2 		3 	// switch_2
#define button_1 		6 	// button_1
#define button_2 		4		// button_2
#define button_3 		5 	// button_3
#define button_4 		7 	// button_4
//-------------------------------------------------------
#define CE 					9 	// NRF24L01 CE pin connection
#define CSN 				10 	// NRF24L01 CSN pin connection
#define SCK					13	// Serial Clock (SCLK) pin
#define MOSI				11	// Master Out Slave In (MOSI) pin
#define	MISO				12	// Master In Slave Out (MISO) pin
//#define IRQ 			NC	//
//-------------------------------------------------------
//using pin X for the CE pin, and pin Y for the CSN pin
RF24 radio(CE, CSN);   // nRF24L01 (CE, CSN)
//const byte address[6] = "00001"; // Address
// Max size of this struct is 32 bytes - NRF24L01 buffer limit
struct Data_Package {
  byte djoy1_button;
  byte djoy2_button;
  byte dswitch_1;
  byte dswitch_2;
  byte dbutton_1;
  byte dbutton_2;
  byte dbutton_3;
  byte dbutton_4;
  byte dpot_1;
  byte dpot_2;
  byte djoy1_X;
  byte djoy1_Y;
  byte djoy2_X;
  byte djoy2_Y;
  //byte pitch;
  //byte roll;
};
//Create a variable with the above structure
Data_Package data;
//  Reset first values
void resetData() {
  // initial default values
  data.djoy1_button = 1;
  data.djoy2_button = 1;
  data.dswitch_1 = 1;
  data.dswitch_2 = 1;
  data.dbutton_1 = 1;
  data.dbutton_2 = 1;
  data.dbutton_3 = 1;
  data.dbutton_4 = 1;
  data.dpot_1 = 1;
  data.dpot_2 = 1;
  data.djoy1_X = 127;
  data.djoy1_Y = 127;
  data.djoy2_X = 127;
  data.djoy2_Y = 127;
  //data.dpitch = 0;
  //data.droll = 0;
}
//
void setup() {
  // start serial communication
  Serial.begin(115200);
  // Set all the control pins as input
  pinMode(joy1_button, INPUT_PULLUP);   // INPUT_PULLUP
  pinMode(joy2_button, INPUT_PULLUP);   // INPUT_PULLUP
  pinMode(switch_1, INPUT_PULLUP);
  pinMode(switch_2, INPUT_PULLUP);
  pinMode(button_1, INPUT_PULLUP);
  pinMode(button_2, INPUT_PULLUP);
  pinMode(button_3, INPUT_PULLUP);
  pinMode(button_4, INPUT_PULLUP);
  pinMode(pot_1, 	INPUT);
  pinMode(pot_2, 	INPUT);
  pinMode(joy1_X, INPUT);
  pinMode(joy1_Y, INPUT);
  pinMode(joy2_X, INPUT);
  pinMode(joy2_Y, INPUT);
  //
  resetData();
  // define the radio communication
  if (!radio.begin()) {
    Serial.println(F("radio hardware is not responding!!"));
    while (1) {}  // hold in infinite loop
  }
  radio.openWritingPipe(0x0000000001);
  //radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  //radio.setPALevel(RF24_PA_LOW);
  // set the module as transmitter
  radio.stopListening();
}
//
void loop() {
  // Read all analog inputs and map them to one Byte value
  // Read all digital inputs
  data.djoy1_button = !digitalRead(joy1_button);
  data.djoy2_button = !digitalRead(joy2_button);
  data.dswitch_1 = !digitalRead(switch_1);
  data.dswitch_2 = !digitalRead(switch_2);
  data.dbutton_1 = !digitalRead(button_1);
  data.dbutton_2 = !digitalRead(button_2);
  data.dbutton_3 = !digitalRead(button_3);
  data.dbutton_4 = !digitalRead(button_4);
  // Convert the analog read value from 0 to 1023 into
  // a BYTE value from 0 to 255
  data.dpot_1  = map(analogRead(pot_1), 0, 1023, 255, 0);
  data.dpot_2  = map(analogRead(pot_2), 0, 1023, 255, 0);
  data.djoy1_X = map(analogRead(joy1_X), 0, 1023, 255, 0);
  data.djoy1_Y = map(analogRead(joy1_Y), 0, 1023, 0, 255);
  data.djoy2_X = map(analogRead(joy2_X), 0, 1023, 255, 0);
  data.djoy2_Y = map(analogRead(joy2_Y), 0, 1023, 0, 255);
  //data.dpitch = (pitch);
  //data.droll = (roll);
  // Send the whole data from the structure to the receiver
  radio.write(&data, sizeof(Data_Package));
}
