#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>

RF24 radio(7, 8);           
RF24Network network(radio);      
const uint16_t this_node = 02; 
const uint16_t master00 = 00;    
const unsigned long interval = 50;  
unsigned long last_sent; 

byte motor2 = 0;
byte ldrsensoru = 0;

void setup() {
  Serial.begin(9600);
  SPI.begin();
  radio.begin();
  network.begin(90, this_node); 
  radio.setDataRate(RF24_2MBPS);
  pinMode(2,1);
}
void loop() {
  
  network.update();
  
  //===== Yayını alma =====//
  
  while ( network.available() ) {  
    RF24NetworkHeader header;
    network.read(header, &motor2, sizeof(motor2)); 
  } 
  
  //===== Veri İşlemleri =====//
    ldrsensoru = map(analogRead(A0),0,255,0,25);
    if( ldrsensoru > 80 )
    {
      motor2 = 0; 
      digitalWrite(2,1);
    }
    else digitalWrite(2,0);
    analogWrite(3,map(motor2,0,100,0,255));  
    
//    Serial.print("LDR=");
//    Serial.print( ldrsensoru );
//    Serial.print("  MOTOR2=");
//    Serial.print( motor2 );

  
  //===== Yayını Gönderme =====//
  
    unsigned long now = millis();
    if (now - last_sent >= interval) {   
    last_sent = now;
    
    RF24NetworkHeader header(master00);   
    bool ok = network.write(header, &ldrsensoru, sizeof(ldrsensoru)); // 
    }
}
