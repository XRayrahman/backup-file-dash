#include <SoftwareSerial.h>
//#define SERIAL_TX_BUFFER_SIZE 128
//#define SERIAL_RX_BUFFER_SIZE 128

SoftwareSerial HM10(2,3); //TX dan RX

String inDataSebelum = "";
String sendData = "";
String Data = "";
//bool isRun;
float rps;
volatile byte half_revolutions;
unsigned int rpm;
unsigned long timeold;

void setup() {
  Serial.begin(115200);
  pinMode(A0, INPUT_PULLUP);
  attachInterrupt(1, magnet_detect, RISING);//Initialize the intterrupt pin (Arduino digital pin 3)
  half_revolutions = 0;
  rpm = 0;
  timeold = 0;
  HM10.begin(115200);
  pinMode(13, OUTPUT);
  digitalWrite(13,LOW);
}

void loop() {
  HM10.listen();
//  StaticJsonDocument<800> doc;
//  DynamicJsonDocument doc(1024);
  
  //Serial Common Data
  serial_com();

  //bluetooth
  bluetooth();
}

void serial_com()
  {    
//    tegangan
    float vin = (analogRead(A0));
    float voltage = vin * (5.0 / 1023.0);
    voltage = voltage *16.8; 
  
//   suhu 
    float suhu_in = (analogRead(A2));
    float suhu = suhu_in * (5.0/1023.0);
    suhu = (suhu/2)*100;

//  baca kecepatan menggunakan hall sensor
    if (half_revolutions >= 5) { 
//    rpm = 30*1000/(millis() - timeold)*half_revolutions;
      rpm = 12*1000/(millis() - timeold)*half_revolutions;
      timeold = millis();
      half_revolutions = 0;
      rps = rpm/60;
    }
    
    Serial.print("{ \"t\":\"");
    Serial.print(voltage);
    Serial.print("\", ");
    Serial.print("\"s\":\"");
    Serial.print(suhu);
    Serial.print("\", ");
    Serial.print("\"r\":\"");
    Serial.print(rps, 3);
    Serial.print("\"");
  }
  
void bluetooth()
  {
    while (HM10.available()){
      // sendData = HM10.readString(); //ada delay read kalau pakai function ini
      // baca setiap char trus dijadikan satu string
      char appData = HM10.read();
      sendData = inDataSebelum + appData;
      inDataSebelum = sendData;
      }
      
    inDataSebelum = "";
    Data = sendData;
    if(sendData=="" || sendData== "stop" || sendData== " ") {
      Serial.print(", \"isRun\":false }");
      Serial.println();
    }
    else {
      Serial.print(", ");
      Serial.print(String(Data));
      Serial.print(", \"isRun\":true }");
      Serial.println();
    }
  }
  
void magnet_detect()//This function is called whenever a magnet/interrupt is detected by the arduino
 {
   half_revolutions++;
 }
