#include <SPI.h>              //Library for using SPI Communication 
#include <mcp2515.h>          //Library for using CAN Communication (https://github.com/autowp/arduino-mcp2515/)
#define BRAKELIGHT_PIN 4
struct can_frame canMsg;
MCP2515 mcp2515(10);                 // SPI CS Pin 10

const int decimalpoint = 3;

void setup()
{
  Serial.begin(9600);                //Begins Serial Communication at 9600 baudrate
  SPI.begin();                       //Begins SPI communication
  pinMode(BRAKELIGHT_PIN,OUTPUT);
  mcp2515.reset();
  mcp2515.setBitrate(CAN_250KBPS, MCP_8MHZ); //Sets CAN at speed 500KBPS and Clock 8MHz
  mcp2515.setNormalMode();                  //Sets CAN at normal mode
} 
//breakdown floating points to a specific decimal point
void decimal_breakdown(float value, int decimalpoint, char category) {
  int digit_array [decimalpoint + 1];
  digit_array[0] = int(value);
  for (int i = 0; i < decimalpoint; i++) {
    value = value - int(value);
    value = value * 10;
    digit_array[i + 1] = int(value);
  }
  //if (category == 'w') {
    //for (int i = 0; i < decimalpoint + 1; i++) {
      //windshieldwipercurrent[i] = digit_array[i];
    //}
  //}
}
//MCP2515 functions --------------------------------------------------------------------------------            

void transmitter(char category) {
  canMsg.can_id  = 0x032;           //CAN id as 0x032 for Arduino 1
  canMsg.can_dlc = decimalpoint + 2;  //CAN data length as decimal point + 2

  canMsg.data[0] = category;
  //if (category == 'w') {
   // for (int i = 1; i < decimalpoint + 1; i++) {
      //canMsg.data[i] = windshieldwipercurrent[i-1];
    //}
   // Serial.println("Send out windsheild wiper motor current(A)");
  //}

  mcp2515.sendMessage(&canMsg);     //Sends the CAN message

}
             
void receiver() {
  if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) // To receive data (Poll Read)
  {
    
    if (canMsg.can_id == 0x0030) { //if arduino 2 sending data
    }
    if (canMsg.can_id == 0x0031) { //if esp32 sending data
      //int voltValue = canMsg1.data[0];
      //Serial.println(voltValue);
      //delay(200);
    }
  }
}
//MCP2515 functions --------------------------------------------------------------------------------            

void loop()
{
  receiver();
  digitalWrite(4,HIGH);
  delay(200);
  digitalWrite(4,LOW);
  delay(200);
}
