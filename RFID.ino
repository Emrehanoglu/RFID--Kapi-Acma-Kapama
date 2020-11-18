#include<SPI.h>
#include<MFRC522.h>
#include<Servo.h>

#define RST_PIN 9
#define SS_PIN 10

Servo motor;
MFRC522 rfid(SS_PIN, RST_PIN);
byte ID[4] = {135, 28, 43, 63};

#define kirmizi 4
#define yesil 5
#define mavi 6

#define alarm 3
int uyari = 0;

void setup() {
  pinMode(kirmizi,OUTPUT);
  pinMode(yesil,OUTPUT);
  pinMode(mavi,OUTPUT);
  pinMode(alarm,OUTPUT);
  motor.attach(8);
  
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();

}

void loop() {
  digitalWrite(kirmizi,LOW);
  digitalWrite(yesil,LOW);
  digitalWrite(mavi,LOW);

  if(! rfid.PICC_IsNewCardPresent() ){
    return;
  }

  if(! rfid.PICC_ReadCardSerial() ){
    return;
  }

  if(rfid.uid.uidByte[0] == ID[0] &&
     rfid.uid.uidByte[1] == ID[1] &&
     rfid.uid.uidByte[2] == ID[2] &&
     rfid.uid.uidByte[3] == ID[3]){
      digitalWrite(kirmizi,LOW);
      digitalWrite(yesil,HIGH);
      digitalWrite(mavi,LOW);
      delay(20);
      Serial.print("ID Numarası:");
      delay(20);
      for(int sayac=0 ; sayac<4 ; sayac++){
        Serial.print(rfid.uid.uidByte[sayac]);
        Serial.print(" ");     
      }
      Serial.println(" ");
      delay(500);
      Serial.println("Kapı Açıldı...");
      uyari = 0;
      motor.write(180);
      delay(4000);
      motor.write(0);
      delay(1000);
      Serial.println(" ");      
     }
     else{
      digitalWrite(kirmizi,HIGH);
      digitalWrite(yesil,LOW);
      digitalWrite(mavi,LOW);
      digitalWrite(alarm,HIGH);
      delay(500);
      digitalWrite(alarm,LOW);
      Serial.println("Geçersiz Kart");
      Serial.print("ID Numarası: ");
      delay(20);
      for(int sayac=0 ; sayac<4 ; sayac++){
        Serial.print(rfid.uid.uidByte[sayac]);
        Serial.print(" ");          
      }
      Serial.println(" ");
      delay(500);
      Serial.println("Kapı Kapalı");
      uyari = uyari+1;
      if(uyari < 3){
        Serial.print("Uyarı: ");
        Serial.println(uyari);
      }

      else{
        for(int j=0 ; j<100 ; j++ ){
          digitalWrite(alarm,HIGH);
          digitalWrite(kirmizi,HIGH);
          digitalWrite(yesil,LOW);
          digitalWrite(mavi,LOW);
          delay(50);
          digitalWrite(alarm,LOW);
          digitalWrite(kirmizi,HIGH);
          digitalWrite(yesil,LOW);
          digitalWrite(mavi,LOW);
          delay(50);          
        }
      
      }
      Serial.println(" ");      
     }
     rfid.PICC_HaltA();
}
