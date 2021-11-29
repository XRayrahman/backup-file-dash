//int A2; //pin baca tegangan
signed long int steps = 0;
float step_old = 0;
float rps = 0;
float temp = 0;
unsigned long start_time = 0;
unsigned long end_time = 0;
int round_step_before = 0;

void setup() {
  Serial.begin(9600);
  //  pinMode(sensor,INPUT_UP);
}

void loop() {
  int x = 0;
  //baca kecepatan
  start_time = millis();
  end_time = start_time + 1000;
  //akan jalan selama 1000ms = 1 detik >> menghitung round per second
  while (millis() < end_time)
  {
    float step_in = (analogRead(A0));
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
    //  Serial.print(steps);
    //  Serial.println();
  }
  //store jumlah total step skarang untuk dijadikan jumlah total step sebelum
  temp = steps - step_old;
  step_old = steps;

  float vin = (analogRead(A2));
  float voltage = vin * (5.0 / 1023.0);
  voltage = voltage / 0.05952381; 
  //karna 84 volt (dari rumus pembagian tegangan terus dibalikin lagi)

  //print udah dalam bentuk yang dimengerti json biar enak wkwk
  Serial.print("{\"t\":\"");
  Serial.print(voltage);
  Serial.print("\", ");
  Serial.print("\"r\":\"");
  Serial.print(temp);
  Serial.print("\"}");
  Serial.println();
}
