#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>

RF24 radio(7, 8);               
RF24Network network(radio);     
const uint16_t this_node = 01;   
const uint16_t master00 = 00;   
const unsigned long interval = 50;  
unsigned long last_sent; 

byte isisensoru = 0;
byte motor1 = 0;

void setup() {
  Serial.begin(9600);
  SPI.begin();
  radio.begin();
  network.begin(90, this_node); //(kanal, alıcı adresi)
  radio.setDataRate(RF24_2MBPS);
  pinMode(2,1);

}
void loop() 
{ 
  network.update();
//=================== Yayını alma ================//
  
  while ( network.available() ) 
  {    
    RF24NetworkHeader header;
    network.read(header, &motor1, sizeof(motor1)); 
  }
  

  //===== Veri İşlemleri =====//

  isisensoru =  map(analogRead(A0),0,255,0,20);
  if( isisensoru > 35 )
  {
    motor1 = 0;
    digitalWrite(2,1); 
  }
  else digitalWrite(2,0);
  analogWrite(3,map(motor1,0,100,0,255));
  
  Serial.print("ISI=");
  Serial.print( isisensoru );
  Serial.print("   MOTOR1=");
  Serial.println ( motor1 );

      //===== Yayını Gönderme =====//
  unsigned long now = millis();
  if (now - last_sent >= interval) 
  {  
    last_sent = now;
    RF24NetworkHeader header(master00);   
    bool ok = network.write(header, &isisensoru, sizeof(isisensoru)); 
  }   
}
