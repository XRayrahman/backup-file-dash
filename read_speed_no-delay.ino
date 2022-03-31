double start_time = 0;
double end_time = 0;
double delta_time = 0.00000;
double delta_time_minutes = 0.00000;
float rpm;
//bool f = true;
float step_in = 0.00;

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
    delta_time_minutes = delta_time/60000;
    rpm = 1/delta_time_minutes;
  } 
  
  Serial.print(rpm, 5);
  Serial.println();
}
