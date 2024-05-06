#define CALL_PIN A1
#define BELL_PIN 12
#define PHONE_PIN A0
#define SPEAKER_PIN 11

int callBtnState = 0;
boolean calling = false;
boolean callEnded = false;
int phoneState = 0;

unsigned long callEndTime = 0;
const long speakerDelay = 1000; // delay before enabling speaker (milliseconds)

unsigned long previousMillis = 0;
const long bellInterval = 1000; // interval at which the bell should toggle (milliseconds)

void setup() {
  pinMode(CALL_PIN, INPUT_PULLUP);
  pinMode(BELL_PIN, OUTPUT);

  pinMode(PHONE_PIN, INPUT_PULLUP);
  pinMode(SPEAKER_PIN, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  callBtnState = digitalRead(CALL_PIN);
  phoneState = digitalRead(PHONE_PIN);
  //Serial.println(callBtnState);

  //LOW = Phone is not on stand
  if (phoneState == LOW && calling) {
    calling = false;
    callEnded = true;
    callEndTime = millis();
  }

  if (callEnded && millis() - callEndTime >= speakerDelay) {
    digitalWrite(SPEAKER_PIN, HIGH);
    callEnded = false;
  }

  //HIGH = Phone is on stand
  if (phoneState == HIGH && digitalRead(SPEAKER_PIN) == HIGH) {
    digitalWrite(SPEAKER_PIN, LOW);
  }

  if (callBtnState == LOW && !calling) { //LOW because it starts with HIGH on boot
    calling = true;
  }

  // Non-blocking delay using millis()
  if (calling) {
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= bellInterval) {
      // Save the current time
      previousMillis = currentMillis;

      // Toggle the bell
      if (digitalRead(BELL_PIN) == LOW) {
        digitalWrite(BELL_PIN, HIGH);
      } else {
        digitalWrite(BELL_PIN, LOW);
      }
    }
  } else {
    digitalWrite(BELL_PIN, LOW);
  }
}
