#include <Servo.h>

Servo myservo;
int prev1 = 0;
int prev2 = 0;
int prev3 = 0;
int prev4 = 0;
int countL = 0;
int countR = 0;
int count = 0;
int initialPosition = 90;
char prevflag = 'M';  // initial M

void setup() {
  pinMode(8, INPUT);
  pinMode(9, INPUT);
  pinMode(10, INPUT);
  pinMode(11, INPUT);
  pinMode(5, OUTPUT);
  pinMode(7, OUTPUT);
  digitalWrite(7, HIGH);
  Serial.begin(9600);
  myservo.attach(12);
  myservo.write(initialPosition);
  delay(1000);
}

void loop() {
  int i1 = 0;
  int i2 = 0;
  int i3 = 0;
  int i4 = 0;
  int targetPosition;

  for (int i = 0; i < 10; i++) {
    if (i1 == 0) {
      i1 = digitalRead(8);
    }
    if (i2 == 0) {
      i2 = digitalRead(9);
    }
    if (i3 == 0) {
      i3 = digitalRead(10);
    }
    if (i4 == 0) {
      i4 = digitalRead(11);
    }
    delay(100);
  }

  if (prev1 == 0 && i1 != prev1) {
    countL++;
  }

  if (prev2 == 0 && i2 != prev2) {
    countR--;
  }

  if (prev3 == 0 && i3 != prev3) {
    countR++;
  }

  if (prev4 == 0 && i4 != prev4) {
    countL--;
  }

  Serial.println("Traffic Right: " + String(countR));
  Serial.println("Traffic Left: " + String(countL));

  if (count == 10) {
    if (countL < countR && prevflag != 'L') {
      Serial.println("Switching to Left");
      digitalWrite(5, HIGH);
      for (int i = 0; i < 30; i++) {
        targetPosition = initialPosition + i;
        myservo.write(targetPosition);
        delay(50);
      }
      prevflag = 'L';
    } else if (countL > countR && prevflag != 'R') {
      Serial.println("Switching to Right");
      digitalWrite(5, HIGH);
      for (int i = 0; i < 30; i++) {
        targetPosition = initialPosition - i;
        myservo.write(targetPosition);
        delay(50);
      }
      prevflag = 'R';
    } else if (countL == countR && prevflag != 'M') {
      Serial.println("Returning to Middle");
        digitalWrite(5, HIGH);  // Assuming you want to move the servo while returning to the middle position

        int currentPos = myservo.read();
        int step = (initialPosition - currentPos) > 0 ? 1 : -1;

        for (int j = currentPos; j != initialPosition; j += step) {
          myservo.write(j);
          delay(50);
        }

        digitalWrite(5, LOW);
        delay(50);
        prevflag = 'M';
    } else {
      Serial.println("No change, waiting for traffic reduce...");
    }
    digitalWrite(5, LOW);
    count = 0;
  }

  prev1 = i1;
  prev2 = i2;
  prev3 = i3;
  prev4 = i4;
  count++;
}
