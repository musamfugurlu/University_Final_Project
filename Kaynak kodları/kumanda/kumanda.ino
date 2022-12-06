/*
  Arduino Wireless Network - Multiple NRF24L01 Tutorial
          == Base/ Master Node 00==
  by Dejan, www.HowToMechatronics.com
  Libraries:
  nRF24/RF24, https://github.com/nRF24/RF24
  nRF24/RF24Network, https://github.com/nRF24/RF24Network
*/

#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>
#include <LiquidCrystal_I2C.h>
byte sol = 2;
byte yukari = 3;
byte sag = 4;
byte asagi = 5;
byte ok = 6;
byte buzzer = 9;
int sira = 1;
LiquidCrystal_I2C lcd( 0x27 , 20  , 4 );
RF24 radio(7, 8);               
RF24Network network(radio);     
const uint16_t this_node = 00;  
const uint16_t node01 = 01;    
const uint16_t node02 = 02;
const uint16_t node03 = 03;
const unsigned long interval = 100; 
unsigned long last_sent; 


byte motor1 = 0;
byte motor2 = 0;
byte motor3 = 0;
byte ldrsensoru = 0;
byte isisensoru = 0;
byte gazsensoru = 0;
byte gelenveri = 0;

void setup() {
  Serial.begin(9600); 
  lcd.begin();
  SPI.begin();
  radio.begin();
  network.begin(90, this_node);
  radio.setDataRate(RF24_2MBPS);
  pinMode(2,INPUT_PULLUP);
  pinMode(3,INPUT_PULLUP);
  pinMode(4,INPUT_PULLUP);
  pinMode(5,INPUT_PULLUP);
  pinMode(6,INPUT_PULLUP);
  pinMode(9,OUTPUT);
}

void loop() { 
  network.update();
  //===== Yayını alma =====// 
  
    while ( network.available() ) {
    RF24NetworkHeader header;
    network.read(header, &gelenveri, sizeof(gelenveri)); 
    if (header.from_node == 01)     isisensoru = gelenveri;
    if (header.from_node == 02)     ldrsensoru = gelenveri;
    if (header.from_node == 03)     gazsensoru = gelenveri;     
   }

   
   //===== Veri İşlemleri =====// Veri işlemlerini yapmamız yeterli gönderme ve alma otomatik
  
  if( isisensoru > 35 || ldrsensoru > 80 || gazsensoru > 40 )
  {
    digitalWrite(buzzer,1);
    
    if( isisensoru > 35 )   {lcd.setCursor(0,1);       lcd.print("    ISI ALARMI !!!  ");}
    if( ldrsensoru > 80 )   {lcd.setCursor(0,1);       lcd.print("    LDR ALARMI !!!  ");}
    if( gazsensoru > 40 )   {lcd.setCursor(0,1);       lcd.print("    GAZ ALARMI !!!  ");}
    
    lcd.setCursor(0,0);       lcd.print("                    ");
    lcd.setCursor(0,2);       lcd.print("                    ");
    lcd.setCursor(0,3);       lcd.print("                    ");
    
    delay(500);
  }
  else 
  {
    digitalWrite(buzzer,0);
    
    if( !digitalRead(yukari) )          sira--;
    else if ( !digitalRead(asagi) )     sira++;
    if( sira<1 )            sira = 1;
    else if ( sira > 3 )    sira = 3;
  
    switch(sira)
    {
      case 1:
        if ( !digitalRead(sol) )      motor1 = motor1 -5;
        else if( !digitalRead(sag) )  motor1 = motor1 +5;
        if( motor1 > 220 )           motor1 = 0;
        else if ( motor1 > 100 && motor1 < 120 )     motor1 = 100;
        lcd.setCursor(10,1);       lcd.print("->");
        lcd.setCursor(10,2);       lcd.print("  ");
        lcd.setCursor(10,3);       lcd.print("  ");
        break;
        
      case 2:
        if ( !digitalRead(sol) )      motor2 = motor2 -5;
        else if( !digitalRead(sag) )  motor2 = motor2 +5;
        if( motor2 > 220 )                       motor2 = 0;
        else if ( motor2 > 100 && motor2 < 120 )     motor2 = 100;
        lcd.setCursor(10,1);       lcd.print("  ");
        lcd.setCursor(10,2);       lcd.print("->");
        lcd.setCursor(10,3);       lcd.print("  ");
        break;
         
      case 3:
        if ( !digitalRead(sol) )      motor3 = motor3 -5;
        else if( !digitalRead(sag) )  motor3 = motor3 +5;
        if( motor3 > 220 )                       motor3 = 0;
        else if ( motor3 > 100 && motor3 < 120 )     motor3 = 100;
        lcd.setCursor(10,1);       lcd.print("  ");
        lcd.setCursor(10,2);       lcd.print("  ");
        lcd.setCursor(10,3);       lcd.print("->");
        break;  
    }
    
    if( isisensoru < 100 ){  lcd.setCursor(6,1);       lcd.print(" ");}
    if( ldrsensoru < 100 ){  lcd.setCursor(6,2);       lcd.print(" ");}
    if( gazsensoru < 100 ){  lcd.setCursor(6,3);       lcd.print(" ");} 
    if( isisensoru < 10 ) {  lcd.setCursor(5,1);       lcd.print(" ");}
    if( ldrsensoru < 10 ) {  lcd.setCursor(5,2);       lcd.print(" ");}
    if( gazsensoru < 10 ) {  lcd.setCursor(5,3);       lcd.print(" ");}
    
    lcd.setCursor(0,0);       lcd.print("SENSORLER   ");
    lcd.setCursor(0,1);       lcd.print("ISI="); 
    lcd.setCursor(4,1);       lcd.print(isisensoru);
    lcd.setCursor(7,1);       lcd.print("C  ");
    lcd.setCursor(0,2);       lcd.print("LDR=");
    lcd.setCursor(4,2);       lcd.print(ldrsensoru);
    lcd.setCursor(7,2);       lcd.print("%  ");
    lcd.setCursor(0,3);       lcd.print("GAZ=");
    lcd.setCursor(4,3);       lcd.print(gazsensoru);
    lcd.setCursor(7,3);       lcd.print("%  ");    
    
    if( motor1 < 100 ){  lcd.setCursor(19,1);       lcd.print(" ");}
    if( motor2 < 100 ){  lcd.setCursor(19,2);       lcd.print(" ");}
    if( motor3 < 100 ){  lcd.setCursor(19,3);       lcd.print(" ");} 
    if( motor1 < 10 ) {  lcd.setCursor(18,1);       lcd.print(" ");}
    if( motor2 < 10 ) {  lcd.setCursor(18,2);       lcd.print(" ");}  
    if( motor3 < 10 ) {  lcd.setCursor(18,3);       lcd.print(" ");}  
  
    lcd.setCursor(12,0);       lcd.print("MOTORLAR");
    lcd.setCursor(12,1);       lcd.print("MTR1=");
    lcd.setCursor(17,1);       lcd.print(motor1);
    lcd.setCursor(12,2);       lcd.print("MTR2=");
    lcd.setCursor(17,2);       lcd.print(motor2);
    lcd.setCursor(12,3);       lcd.print("MTR3="); 
    lcd.setCursor(17,3);       lcd.print(motor3);
  }
  //===== Yayını Gönderme =====//  
  
    unsigned long now = millis();
  if (now - last_sent >= interval) {  
    last_sent = now;
 
    RF24NetworkHeader header1(node01); // Alıcı1 Seçimi 
    bool ok = network.write(header1, &motor1, sizeof(motor1)); // Data Gönderme

    RF24NetworkHeader header2(node02);  // Alıcı2 Seçimi  
    bool ok1 = network.write(header2, &motor2, sizeof(motor2)); // Data Gönderme

    RF24NetworkHeader header3(node03);  // Alıcı3 Seçimi   
    bool ok2 = network.write(header3, &motor3, sizeof(motor3)); // Data Gönderme
  }
}
