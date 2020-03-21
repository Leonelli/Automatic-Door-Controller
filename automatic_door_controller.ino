#include <Stepper.h>
#include <AccelStepper.h>
#include <Keypad.h>

// Define number of steps per rotation:
const int stepsPerRevolution = 2048;

const int buttonPin = 1;


int buttonState = 0;         // variable for reading the pushbutton status
bool direzione = true;
int contagiri = 0;
int N_GIRI_CHIUSURA = 5;

String password = "610#";  // SETTING DEFAULT PASSWORD.
int _position = 0; // VARIABLE FOR DETERMINING THE POSITION.
int wrong = 0;
int total = 0;

// Motor pin definitions:
#define motorPin1  9      // IN1 on the ULN2003 driver
#define motorPin2  10      // IN2 on the ULN2003 driver
#define motorPin3  11     // IN3 on the ULN2003 driver
#define motorPin4  12     // IN4 on the ULN2003 driver

// Define the AccelStepper interface type; 4 wire motor in half step mode:
#define MotorInterfaceType 8

// Initialize with pin sequence IN1-IN3-IN2-IN4 for using the AccelStepper library with 28BYJ-48 stepper motor:
AccelStepper stepper = AccelStepper(MotorInterfaceType, motorPin1, motorPin3, motorPin2, motorPin4);

const int LEDyellow = 13;
const int LEDred = A0;
const int LEDgreen = A1;

int buzzerPIN = 0;

char keys[4][3] = { // LAYOUT OF KEYPAD
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

byte rowPin[4] = {4, 5, 6, 7}; // ROW PINS OF KEYPAD
byte colPin[4] = {3, 2, A4, A5}; // COLUMN PINS OF KEYPAD

Keypad keypad = Keypad(makeKeymap(keys), rowPin, colPin, 4, 3);



void setup() {
  // Set the speed to 5 rpm:
  stepper.setMaxSpeed(1000);

  // Begin Serial communication at a baud rate of 9600:
  Serial.begin(9600);

  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(LEDyellow, OUTPUT);
  pinMode(LEDgreen, OUTPUT);
  pinMode(LEDred, OUTPUT);
  pinMode(buzzerPIN, OUTPUT);

}
void loop() {
  char pressed = keypad.getKey(); // TAKING THE INPUT FROM KEYPAD
  String key[4];
  digitalWrite(LEDyellow, HIGH);
  digitalWrite(LEDred, HIGH);
  digitalWrite(LEDgreen, HIGH);


  if (pressed) // IF THE KEY IS PRESSED
  {
    Serial.println(pressed);
    if (pressed == '*')
    {
      _position = 0;
    }
    else if (pressed == password[_position])
    {
      Serial.println((String)pressed + "==" + password[_position]);
      Serial.println((String)_position);
      key[_position] = pressed;
      _position++;
    }

    else if (pressed != password[_position] )
    { // IN CASE OF WRONG INPUT INCREMENT BOTH WRONG AND POSITION.
      wrong++;
      _position ++;
    }

    if (_position == 4)
    { // WHEN POSITION == 3 THEN CHECK THE FOLLOWING
      if ( wrong > 0) // IF ANY WRONG INPUT IF GIVEN THEN INCREMENT TOTAL AND
        // SET WRONG AND POSITION TO ZERO.
      {
        total++;
        wrong = 0;
        _position = 0;
        Serial.println("wrong pin");
        tone(buzzerPIN, 1000, 500);
        digitalWrite(LEDred, LOW);
        delay(500);
        noTone(buzzerPIN);
      }

      else if (_position == 4 && wrong == 0)
      { // IF NO WRONG VALUE IS GIVEN THEN DISPLAY THE ACCEPTED PASSWORD AND
        // MOVE THE SERVO MOTOR.
        _position = 0;
        wrong = 0;

        digitalWrite(LEDyellow, HIGH);
        //if (digitalRead(buttonPin) == LOW) {
        if (direzione) {
          stepper.setCurrentPosition(0);
          tone(buzzerPIN, 400, 1500);
          digitalWrite(LEDyellow, LOW);
          digitalWrite(LEDgreen, LOW);
          delay(1500);
          noTone(buzzerPIN);
          while (stepper.currentPosition() != 4096 * N_GIRI_CHIUSURA) {
            stepper.setSpeed(1100);
            stepper.runSpeed();
          }
          direzione = !direzione;
        }
        else {
          stepper.setCurrentPosition(0);
          tone(buzzerPIN, 400, 1500);
          digitalWrite(LEDyellow, LOW);
          digitalWrite(LEDgreen, LOW);
          delay(1500);
          noTone(buzzerPIN);
          while (stepper.currentPosition() != -4096 * N_GIRI_CHIUSURA) {
            stepper.setSpeed(-1100);
            stepper.runSpeed();

          }
          direzione = !direzione;
        }
        //}
      }
      if (total == 4) // IF TOTAL OF 3 ATTEMPTS ARE DONE BY ENTERING WRONG PASS
        //WORD THEN SOUND A BUZZER AND SET TOTAL TO 0.
      {
        total = 0;
        //buzzer_beep();
        //delay(500);
      }
    }
  }
}
