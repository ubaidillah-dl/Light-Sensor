const byte ldr = A0, led=4;
short nilai;

void setup() {
  Serial.begin(9600);
  pinMode(ldr, INPUT);
  pinMode(led, OUTPUT);
}

void loop() {
  nilai=analogRead(ldr);
  Serial.println(nilai);

  if(nilai<500){
    digitalWrite(led, HIGH);
  }else{
    digitalWrite(led, LOW);
    Serial.println("Keadaan Sangat Terang");
  }
}
