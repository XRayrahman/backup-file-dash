//#include <Arduino_JSON.h>
#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include <Arduino_JSON.h>


SoftwareSerial HM10(2,3);

String inDataSebelum = "";
String sendData = "";
double start_time = 0;
double end_time = 0;
double delta_time = 0.00000;
double delta_time_seconds = 0.00000;
//bool isRun;
float rps;
long val;
//bool f = true;
float step_in = 0.00;

void setup() {
  Serial.begin(115200);
  pinMode(A0, INPUT_PULLUP);
  HM10.begin(9600);
  pinMode(13, OUTPUT);
  digitalWrite(13,LOW);
}

void loop() {
  HM10.listen();

//  StaticJsonDocument<800> doc;
  DynamicJsonDocument doc(1024);

//  kecepatan
  step_in = analogRead(A0);
  step_in = step_in * (5.0/1023.0);
  start_time = millis();

//  tegangan
  float vin = (analogRead(A5));
  float voltage = vin * (5.0 / 1023.0);
  voltage = voltage *16.8; 

  while (step_in > 3.55)
  {
    step_in = analogRead(A0);
    step_in = step_in * (5.0/1023.0);
    end_time = millis(); 
    delta_time = end_time - start_time;
    delta_time_seconds = delta_time/1000;

    if(delta_time_seconds > 1){
      rps = 0.00;  
      break;
    }
    
    rps = 1/delta_time_seconds;
  } 
//  rps = step_in;   // untuk dev                                                                                     
  
//  dataObject["t"] = voltage;
//  dataObject["r"] = rps;
//  Serial.println(dataObject);

  Serial.print("{ \"t\":\"");
  Serial.print(voltage);
  Serial.print("\", ");
  Serial.print("\"r\":\"");
  Serial.print(rps, 3);
  Serial.print("\"");
  
  while (HM10.available()){
    // sendData = HM10.readString(); //ada delay read kalau pakai function ini

    // baca setiap char trus dijadikan satu string
    char appData = HM10.read();
    sendData = inDataSebelum + String(appData);
    inDataSebelum = sendData;
  }
  inDataSebelum = "";
  
  if(sendData=="" || sendData== "stop" || sendData== " ") {
//    isRun = false;
    Serial.print(", \"isRun\":false }");
    Serial.println();
  }
  else {
    Serial.print(", ");
    Serial.print(sendData);
    Serial.print(", \"isRun\":true }");
    Serial.println();
  }
  
//  rps = (float)rps/1.10; // ga perlu karena real-life roda berputar terus
  delay(30);
}
