#define echoPin 23
#define trigPin 22

void SonicSetup() {
  pinMode(trigPin, OUTPUT); // ตั้งขาทริกเกอร์เป็น OUTPUT
  pinMode(echoPin, INPUT); // ตั้งขาเอคโค่เป็น Input
}

int GetSonic() {
  long duration;
  int distance;
  // เครียร์ขา trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // กำหนดให้ trigPin เป็น HIGH เป็นเวลา 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // อ่านขา echoPin, คืนค่าระยะเวลาการเดินทางของคลื่นเสียงในหน่วยเวลา microseconds
  duration = pulseIn(echoPin, HIGH);
  // คำนวณระยะ
  distance = duration * 0.034 / 2;
  // คืนค่าระยะทาง
  return distance;
}