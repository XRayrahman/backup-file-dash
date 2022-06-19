#include <SoftwareSerial.h>
#define SERIAL_TX_BUFFER_SIZE 128

#define SERIAL_RX_BUFFER_SIZE 128


SoftwareSerial HM10(2,3); //TX dan RX

String inDataSebelum = "";
String sendData = "";
String Data = "";
double start_time = 0;
double end_time = 0;
double delta_time = 0.00000;
double delta_time_seconds = 0.00000;
//bool isRun;
float rps;
long val;
//bool f = true;
float step_in = 0.00;
int round_step_before = 0;
int steps = 0;
float step_old = 0;


void setup() {
  Serial.begin(115200);
  pinMode(A0, INPUT_PULLUP);
  HM10.begin(115200);
  pinMode(13, OUTPUT);
  digitalWrite(13,LOW);
}

void loop() {
  HM10.listen();

//  StaticJsonDocument<800> doc;
//  DynamicJsonDocument doc(1024);

//  tegangan
  float vin = (analogRead(A0));
  float voltage = vin * (5.0 / 1023.0);
  voltage = voltage *16.8; 

// suhu 
  float suhu_in = (analogRead(A2));
  float suhu = suhu_in * (5.0/1023.0);
  suhu = (suhu/2)*100;
  
  //baca kecepatan
  start_time = millis();
  end_time = start_time + 1000;
  //akan jalan selama 1000ms = 1 detik >> menghitung round per second
  //banyaknya round per detik
  while (millis() < end_time)
  {
    float step_in = (analogRead(A5));
    int round_step = step_in * (5.0 / 1023.0);
    round_step = round_step / 5;
    //  Serial.print(round_step);
    //  Serial.print(voltage);
    //  main di PULLDOWN, harusnya bisa pakai cara pinMode diatas kalau ada PULLDOWN
    if (round_step < 0.5 && round_step_before > 0.75)
    {
      steps = steps + 1;
    }
    //store step skarang untuk dijadikan step sebelum
    round_step_before = round_step;
  }
  //store jumlah total step skarang untuk dijadikan jumlah total step sebelum
  rps = (steps - step_old)*2;
  step_old = steps;

//  kecepatan
  // delta waktu per round
  
//  step_in = analogRead(A0);
//  step_in = step_in * (5.0/1023.0);
//  start_time = millis();
//  while (step_in > 3.55)
//  {
//    step_in = analogRead(A0);
//    step_in = step_in * (5.0/1023.0);
//    end_time = millis(); 
//    delta_time = end_time - start_time;
//    delta_time_seconds = delta_time/1000;
//
//    if(delta_time_seconds > 1){
//      rps = 0.00;  
//      break;
//    }
//    
//    rps = 1/delta_time_seconds;
//  } 

//  rps = step_in;   // untuk dev    

  Serial.print("{ \"t\":\"");
  Serial.print(voltage);
  Serial.print("\", ");
  Serial.print("\"s\":\"");
  Serial.print(suhu);
  Serial.print("\", ");
  Serial.print("\"r\":\"");
  Serial.print(rps, 3);
  Serial.print("\"");

  //bluetooth
  while (HM10.available()){
    // sendData = HM10.readString(); //ada delay read kalau pakai function ini

    // baca setiap char trus dijadikan satu string
    char appData = HM10.read();
//    Serial.println(appData);
    
    sendData = inDataSebelum + appData;
    inDataSebelum = sendData;
  }
  
  inDataSebelum = "";
  Data = sendData;
  if(sendData=="" || sendData== "stop" || sendData== " ") {
//    isRun = false;
    Serial.print(", \"isRun\":false }");
    Serial.println();
  }
  else {
    Serial.print(", ");
    Serial.print(String(Data));
    Serial.print(", \"isRun\":true }");
    Serial.println();
  }
  
//  rps = (float)rps/1.10; // ga perlu karena real-life roda berputar terus
//  delay(30);
}
