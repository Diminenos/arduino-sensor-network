#include <RF22.h>
#include <RF22Router.h>
#include <Wire.h> 
#include <OneWire.h> 
#include <DallasTemperature.h>
#define SOURCE_ADDRESS2 22
#define DESTINATION_ADDRESS 8
#define ONE_WIRE_BUS 2
RF22Router rf22(SOURCE_ADDRESS2); //i am a transmitter
 


OneWire oneWire(ONE_WIRE_BUS);  // Setup a oneWire instance to communicate with any OneWire devices
DallasTemperature sensors(&oneWire); // Pass our oneWire reference to Dallas Temperature. 
boolean successful_packet = false;
long randNumber;
int max_delay=1000;           //random wait time

void setup() {
   Serial.begin(9600);
  if (!rf22.init())
    Serial.println("RF22 init failed");
  
  if (!rf22.setFrequency(431.0))
    Serial.println("setFrequency Fail");
  rf22.setTxPower(RF22_TXPOW_20DBM);
  rf22.setModemConfig(RF22::GFSK_Rb125Fd125);
  rf22.addRouteTo(DESTINATION_ADDRESS, DESTINATION_ADDRESS);
    sensors.begin(); // initialize the sensor
      randomSeed(2000);//start randNumber

}

void loop() {
 
   sensors.requestTemperatures();          // call sensors.requestTemperatures() to issue a global temperature 
   float temp=sensors.getTempCByIndex(0);  // We can have more than one DS18B20   
                                           // 0 refers to the first IC on the wire


    Serial.print("Rooms temp is: ");
       Serial.print(temp);
       Serial.println("C");
     // Get Measurement

   int temp2=(int)(temp*100);
  
  char data_read[RF22_ROUTER_MAX_MESSAGE_LEN];
  uint8_t data_send[RF22_ROUTER_MAX_MESSAGE_LEN];
  memset(data_read, '\0', RF22_ROUTER_MAX_MESSAGE_LEN);
  memset(data_send, '\0', RF22_ROUTER_MAX_MESSAGE_LEN);
  sprintf(data_read, "%d", temp2);
  

  data_read[RF22_ROUTER_MAX_MESSAGE_LEN - 1] = '\0';
  memcpy(data_send, data_read, RF22_ROUTER_MAX_MESSAGE_LEN);
 

  successful_packet = false;
  while (!successful_packet)
  {
    if (rf22.sendtoWait(data_send, sizeof(data_send), DESTINATION_ADDRESS) != RF22_ROUTER_ERROR_NONE)
    {
      Serial.println("sendtoWait failed");
      randNumber=random(200,max_delay);
     // Serial.println(randNumber);
      delay(randNumber);
    }
    else
    {
      successful_packet = true;
      Serial.println("sendtoWait Succesful");
    }
  }
  
  delay(20000);
  
}
