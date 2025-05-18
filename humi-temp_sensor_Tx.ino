#include "DHT.h"
#include <RF22.h>
#include <RF22Router.h>

#define DHTPIN 5       // Set DHT pin
#define DHTTYPE DHT11  // Set DHT type
#define SOURCE_ADDRESS1 26
#define DESTINATION_ADDRESS 8
RF22Router rf22(SOURCE_ADDRESS1); //i am a transmitter

boolean successful_packet = false;
long randNumber;
int max_delay=1000; //random wait time
float tempEgg;
float humiEgg;

DHT dht(DHTPIN, DHTTYPE);  // Initialize DHT sensor 

void setup() {
   Serial.begin(9600);
  if (!rf22.init())
    Serial.println("RF22 init failed");
 
      if (!rf22.setFrequency(431.0))
       Serial.println("setFrequency Fail");
        rf22.setTxPower(RF22_TXPOW_20DBM);
          rf22.addRouteTo(DESTINATION_ADDRESS, DESTINATION_ADDRESS);
               rf22.setModemConfig(RF22::GFSK_Rb125Fd125);
              dht.begin();   // Setup sensor
              randomSeed(2000); //start randNumber
}

void loop() {

float humiEgg  = dht.readHumidity();          // Read the humidity in %
float tempEgg  = dht.readTemperature();       // Read the temperature as Celsius
     if (isnan(humiEgg) && isnan(tempEgg))  {
        Serial.println("Failed to read from DHT sensor!");
}

// Get Measurement
int humiEgg2=(int)(humiEgg); 
int tempEgg2=(int)(tempEgg*100);
   
 char data_read[RF22_ROUTER_MAX_MESSAGE_LEN];
 uint8_t data_send[RF22_ROUTER_MAX_MESSAGE_LEN];
 memset(data_read, '\0', RF22_ROUTER_MAX_MESSAGE_LEN);
 memset(data_send, '\0', RF22_ROUTER_MAX_MESSAGE_LEN);
 
 sprintf(data_read, "%d %d",tempEgg2,humiEgg2);
 
 
 data_read[RF22_ROUTER_MAX_MESSAGE_LEN - 1] = '\0';
 memcpy(data_send, data_read, RF22_ROUTER_MAX_MESSAGE_LEN);


successful_packet = false;
  while (!successful_packet)
  {
    if (rf22.sendtoWait(data_send,sizeof(data_send),DESTINATION_ADDRESS)!= RF22_ROUTER_ERROR_NONE)
       {
        Serial.println("sendtoWait failed");
        randNumber=random(200,max_delay);
        //Serial.println(randNumber);
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
