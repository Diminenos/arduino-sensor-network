#include <RF22.h>
#include <RF22Router.h>
#define SOURCE_ADDRESS3 24
#define DESTINATION_ADDRESS 8
#define fsrpin A0
RF22Router rf22(SOURCE_ADDRESS3); //i am a transmitter


boolean successful_packet = false;
long randNumber;
int max_delay=1000;  //random wait time



int fsrPin = 0;     // the FSR and 10K pulldown are connected to a1
int fsrReading;     // the analog reading from the FSR resistor divider
float fsrVoltage;     // the analog reading converted to voltage
long double fsrResistance;  // The voltage converted to resistance, 
long double fsrConductance; 
double fsrForce;       // Finally, the resistance converted to force
float weight;



void setup() {
 Serial.begin(9600);
  if (!rf22.init())
    Serial.println("RF22 init failed");
  
  if (!rf22.setFrequency(431.0))
    Serial.println("setFrequency Fail");
  rf22.setTxPower(RF22_TXPOW_20DBM);
  rf22.setModemConfig(RF22::GFSK_Rb125Fd125);
  rf22.addRouteTo(DESTINATION_ADDRESS, DESTINATION_ADDRESS);
   
  randomSeed(2000); //start randNumber

}

void loop() 
{
    fsrReading = analogRead(fsrPin);   // analog voltage reading ranges from about 0 to 1023 which maps to 0V to 5V (= 5000mV)
    fsrVoltage = map(fsrReading, 0, 1023, 0, 5000);
     if (fsrVoltage == 0) {
      Serial.println("No food");  
     
    }  else {
      // The voltage = Vcc * R / (R + FSR) where R = 10K and Vcc = 5V
       //so FSR = ((Vcc - V) * R) / V        
        fsrResistance = 5000 - fsrVoltage;     // fsrVoltage is in millivolts so 5V = 5000mV
        fsrResistance *= 10000;                // 10K resistor
        fsrResistance /= fsrVoltage;
        fsrConductance = 1000000;           // we measure in micromhos so 
        fsrConductance /= fsrResistance;
        fsrForce = fsrConductance / 80;       // we use the FSR guide graph to approximate the force
        weight=fsrForce/9.8;                  // and from the force we calculate the weight
      }
  delay(7000);

  // Get Measurement
 
  int weight2 =(int)(weight*1000);
  char data_read[RF22_ROUTER_MAX_MESSAGE_LEN];
  uint8_t data_send[RF22_ROUTER_MAX_MESSAGE_LEN];
  memset(data_read, '\0', RF22_ROUTER_MAX_MESSAGE_LEN);
  memset(data_send, '\0', RF22_ROUTER_MAX_MESSAGE_LEN);
  sprintf(data_read, "%d", weight2);
 
  data_read[RF22_ROUTER_MAX_MESSAGE_LEN - 1] = '\0';
  memcpy(data_send, data_read, RF22_ROUTER_MAX_MESSAGE_LEN);
  successful_packet = false;
  while (!successful_packet)
  {
    if (rf22.sendtoWait(data_send, sizeof(data_send), DESTINATION_ADDRESS) != RF22_ROUTER_ERROR_NONE)
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
