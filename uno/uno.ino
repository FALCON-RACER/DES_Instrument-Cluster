#define sensor 2


volatile int pulseCount = 0;
unsigned long lastTime = 0;
unsigned long interval = 500; // 1초 간격으로 계산


void setup() {
  Serial.begin(9600);
  pinMode(sensor, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(sensor), countPulse, RISING);


  // attachInterrupt(0,countPulse,RISING);
}

void loop() {
  unsigned long currentTime = millis();

  if (currentTime - lastTime >= interval) {
    detachInterrupt(digitalPinToInterrupt(sensor)); // 임시로 인터럽트 해제
    float rpm = (pulseCount / 20.0) * 60.0; // 센서에 따라 변경 필요
    Serial.print("RPM: ");
    Serial.println(rpm);
    pulseCount = 0; // 카운트 초기화
    lastTime = currentTime;
    attachInterrupt(digitalPinToInterrupt(sensor), countPulse, RISING); // 인터럽트 재설정
  }
}

void countPulse() {
  pulseCount++;
}
