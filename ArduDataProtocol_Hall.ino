#include <SoftwareSerial.h>
//#define SERIAL_TX_BUFFER_SIZE 128
//#define SERIAL_RX_BUFFER_SIZE 128
#include <FreqMeasure.h>

SoftwareSerial HM10(2,3); //TX dan RX

String inDataSebelum = "";
String sendData = "";
String Data = "";
double sum=0;
int count=0;
int rpm=0;

void setup() {
  Serial.begin(115200);
  pinMode(A0, INPUT_PULLUP);
  HM10.begin(115200);
  FreqMeasure.begin();
}

void loop() {
  HM10.listen();

  //Serial Common Data
  serial_com();

  //bluetooth
  bluetooth();
  delay(50 );
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

  if (FreqMeasure.available()) {
    // average several reading together
    sum = sum + FreqMeasure.read();
    count = count + 1;
    if (count > 5) {
      float frequency = FreqMeasure.countToFrequency(sum / count);
      // need to calibrate more
      rpm = (frequency*120/8)-10;
      if (rpm < 0){
        rpm = 0;
        }
      sum = 0;
      count = 0;
    }
  }
  else
    {rpm = 0;}
    
    Serial.print("{ \"t\":\"");
    Serial.print(voltage);
    Serial.print("\", ");
    Serial.print("\"s\":\"");
    Serial.print(suhu);
    Serial.print("\", ");
    Serial.print("\"r\":\"");
    Serial.print(rpm);
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
