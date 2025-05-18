
#include <RF22.h>
#include <RF22Router.h>

#define SOURCE_ADDRESS1 26 //  (Humidity-Temperature sensor)
#define SOURCE_ADDRESS2 22 //  (Temperature sensor)
#define SOURCE_ADDRESS3 24 //  (Weight sensor)
#define DESTINATION_ADDRESS 8 // Receiver
RF22Router rf22(DESTINATION_ADDRESS); // I am Receiver 

int led5 = 5; //
int led6 = 6;
int led7 = 7;
int led8 = 8;


void setup() {
  Serial.begin(9600);
  pinMode(led5, OUTPUT); // LED connected to weight sensor
  pinMode(led6, OUTPUT); // LED connected to Humidity-Temperature sensor(Humidity part)
  pinMode(led7, OUTPUT); //LED connected to Humidity-Temperature sensor(Temperature part)
  pinMode(led8, OUTPUT); //LED connected to Temperature sensor

  
  if (!rf22.init())
    Serial.println("RF22 init failed");
  
  if (!rf22.setFrequency(431.0))
    Serial.println("setFrequency Fail");
  rf22.setTxPower(RF22_TXPOW_20DBM);
  
  rf22.setModemConfig(RF22::GFSK_Rb125Fd125);
  

  // Manually define the routes for this network
  rf22.addRouteTo(SOURCE_ADDRESS1, SOURCE_ADDRESS1);
  rf22.addRouteTo(SOURCE_ADDRESS2, SOURCE_ADDRESS2);
  rf22.addRouteTo(SOURCE_ADDRESS3, SOURCE_ADDRESS3);
}

void loop() {
  
  int roomtemp1=0; //Value we get from Source2 (room temperature multiplied by 100)
  float roomtemp2=0; //Variable that we will use to divide roomtemp1 with 100 and get the actual temperature
  int weight1 = 0; //Value we get from Source1 (food weight multiplied by 1000)
  float weight2 = 0; //Variable that we will use to divide weight1 with 1000 and get the actual weight of food
  float  egghumidity1 = 0; //Value we get from Source3 (egg humidity)
  float eggtemp1 = 0; //Value we get from Source3 (egg temperature multiplied by 100)
  float eggtemp2 = 0; //Variable that we will use to divide eggtemp1 with 100 and get the actual temperature of eggs
  
  uint8_t buf[RF22_ROUTER_MAX_MESSAGE_LEN];
  char incoming[RF22_ROUTER_MAX_MESSAGE_LEN];
  memset(buf, '\0', RF22_ROUTER_MAX_MESSAGE_LEN);
  memset(incoming, '\0', RF22_ROUTER_MAX_MESSAGE_LEN);
  uint8_t len = sizeof(buf);
  uint8_t from;
      
  
  
    
   
    

  if (rf22.recvfromAck(buf, &len, &from))
  {
    Serial.print("Got request from : ");
    Serial.println(from, DEC);
    
    
   if (from==22)   //with this if statement, depending on the source that sent us values, receiver will take respective actions,22 is Temperature sensor
    {
       buf[RF22_ROUTER_MAX_MESSAGE_LEN - 1] = '\0';
       memcpy(incoming, buf, RF22_ROUTER_MAX_MESSAGE_LEN);
       int roomtemp1=atoi((char*)incoming);
       float roomtemp2=roomtemp1/100.0; //Here we convert our incoming to the actual room temperature
   
       Serial.print("Rooms temp is: ");
       Serial.print(roomtemp2);
       Serial.println("C");
    
   if (roomtemp2<15|| roomtemp2>25) //critical room temperature
     { 
      Serial.println("Room temperature is critical");
      digitalWrite(8,HIGH); //turn on warning LED
      delay(7000);
      digitalWrite(8,LOW); //turn off warning LED
      delay(500);

   }else{
     
      Serial.println("Room temperature is fine");
     }
    }
 
   
   else   if  (from==24) //24 is weight sensor
   {
  

      buf[RF22_ROUTER_MAX_MESSAGE_LEN - 1] = '\0';
      memcpy(incoming, buf, RF22_ROUTER_MAX_MESSAGE_LEN);
      int  weight1=atoi((char*)incoming);
      float weight2=weight1/1000.0; //Here we convert our incoming to the actual food weight
      Serial.print("Region's weight is: ");
      Serial.print(weight2);
     Serial.println("kg");
      if (weight2<0.5) //Too little food
     {
        Serial.println("Need food");
        digitalWrite(5,HIGH);
        delay(7000);
        digitalWrite(5,LOW);
        delay(500);

     }else{
     
       Serial.println("Food is fine");
     }
    }
   
   else if (from==26)      //26 is Humidity-Temperature sensor
   {
  
      buf[RF22_ROUTER_MAX_MESSAGE_LEN - 1] = '\0';
      memcpy(incoming, buf, RF22_ROUTER_MAX_MESSAGE_LEN); 
      const char d[2]=" "; //We create a string with two values   
      char *token;
      token=strtok(incoming,d); //We separate the string values
      float eggtemp1=atof(token);
      token=strtok(NULL,d);
      float egghumidity1=atof(token);
      float  eggtemp2=eggtemp1/100.0; //Here we convert our incoming to the actual egg temperature
          
      Serial.print("Region's temperature is: ");
      Serial.print(eggtemp2);
      Serial.println(" C");
      
      Serial.print("Region's humidity is: ");
      Serial.print(egghumidity1);
      Serial.println("%");
     
     
     if (eggtemp2<37 || eggtemp2>38) //Critical temperature for the eggs
     {
      Serial.println("Egg temperature critical");
     
       digitalWrite(7,HIGH);
       delay(7000);
       digitalWrite(7,LOW);
       delay(500);
       
     }else{
      Serial.println("Egg temperature is fine");
     }
      if (egghumidity1<50 || egghumidity1>60) // critical humidity for the eggs
      {
      Serial.println("Humidity critical");
      digitalWrite(6,HIGH);
      delay(7000);
      digitalWrite(6,LOW);
      delay(500);
     }else{
      Serial.println("Humidity is fine");
     }
     
    }
      
  } 

       Serial.println(" "); //one line prints for easier serial reading
       

       delay(1000);


}
