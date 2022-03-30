float start_time = 0;
float end_time = 0;
float delta_time = 0;
float delta_time_minutes = 0;
float round_step_before = 0;
float start;
float startMillis;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  //step_time = 0;
  float step_in = (analogRead(A3));
  float round_step = step_in * (5.0/1023.0);
  // jika yang dibaca low maka detik akan jalan dalam loop menghitung waktu,
  // bakal store waktu pada variabel
  start_time = millis();
  if (round_step < 0.55 && round_step_before > 4.55)
  {
    end_time = millis(); 
    delta_time = end_time - start_time;
    delta_time_minutes = delta_time/60000;
  } 
  Serial.print("speed : ");
  Serial.print(delta_time_minutes);
  Serial.println();
}
