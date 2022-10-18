//#define SERIAL_TX_BUFFER_SIZE 128
//#define SERIAL_RX_BUFFER_SIZE 128
#include <SoftwareSerial.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <FreqMeasure.h>
#define ONE_WIRE_BUS 7

SoftwareSerial HM10(2,3); //TX dan RX
OneWire oneWire(ONE_WIRE_BUS);	
DallasTemperature sensors(&oneWire);

String inDataSebelum = "";
String sendData = "";
String Data = "";
double sum=0;
int count=0;
int rps=0;
float suhu=0;
float tegangan=0;

void setup() {
  Serial.begin(115200);
  // pinMode(A0, INPUT_PULLUP);
  HM10.begin(115200);
  FreqMeasure.begin();
  sensors.setResolution(9);
  sensors.requestTemperatures();

}

void loop() {
  HM10.listen();
  
  //Serial Common Data
  serial_com();

  //bluetooth
  bluetooth();
}

void serial_com()
  {    
//    tegangan
  tegangan = tegangan84v();
    
// kecepatan
  if (FreqMeasure.available()) {
    // average several reading together
    sum = sum + FreqMeasure.read();
    count = count + 1;
    if (count > 1) {
      float frequency = FreqMeasure.countToFrequency(sum / count);
      // need to calibrate more
      rps = (frequency*120/8.1)/60;
      if (rps < 0){
        rps = 0;
        }
      sum = 0;
      count = 0;
    }
  }
  else
    {rps = 0;}
    
//   suhu 
    if (sensors.isConversionComplete()){
    suhu = ds18b20();
    sensors.requestTemperatures();
    }
    
    Serial.print("{ \"t\":\"");
    Serial.print(tegangan);
    Serial.print("\", ");
    Serial.print("\"s\":\"");
    Serial.print(suhu);
    Serial.print("\", ");
    Serial.print("\"r\":\"");
    Serial.print(rps);
    Serial.print("\"");
  }

float ds18b20(){
    return sensors.getTempCByIndex(0);
}

float tegangan84v(){
    float vin = (analogRead(A0));
    float voltage = vin * (5.0 / 1023.0);
    return voltage *16.8; 
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
