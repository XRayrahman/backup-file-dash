//#include <Arduino_JSON.h>

double start_time = 0;
double end_time = 0;
double delta_time = 0.00000;
double delta_time_seconds = 0.00000;
float rps;
long val;
//bool f = true;
float step_in = 0.00;
//JSONVar dataObject;

void setup() {
  Serial.begin(9600);
  pinMode(A0, INPUT_PULLUP);
  
}

void loop() {
  step_in = analogRead(A0);
  step_in = step_in * (5.0/1023.0);
  start_time = millis();
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

  float vin = (analogRead(A5));
  float voltage = vin * (5.0 / 1023.0);
  voltage = voltage / 0.05952381; 
//  val = (long)(rps*10);

  rps = (float)rps/1.10;
  
//  dataObject["t"] = voltage;
//  dataObject["r"] = rps;
//  Serial.println(dataObject);

  Serial.print("{\"t\":\"");
  Serial.print(voltage);
  Serial.print("\", ");
  Serial.print("\"r\":\"");
  Serial.print(rps, 3);
  Serial.print("\"}");
  Serial.println();
}
