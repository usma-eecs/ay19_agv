
/*http://www.arduino.cc/en/Tutorial/Ping Our team used the Arduino package located at this link to
interface with the Ping Sensor. 21 Sep 18. West Point, NY.*/

const int pingPin = 7;


#include <Servo.h>
Servo ST1, ST2;
int pResistor1 = A0;    // select the input pin for the potentiometer
int pResistor2 = A1;

int ping_in_val = 0;

void setup() {
  // initialize serial communication:
  Serial.begin(9600);

/*https://www.dimensionengineering.com/info/arduino Our team used the Arduino package located at this link to
interface with the AGV servos. 21 Sep 18. West Point, NY.*/

  //ST1 is right side; ST2 is left side.
  ST1.attach( 10, 1000, 1000);
  ST2.attach(11, 1000, 1000);

  pinMode(pingPin, INPUT);          
}

void stop_moving() {
  // put your main code here, to run repeatedly:
  //90 makes it stop, less than 90 is forward
  int power1 = 90;
  int power2 = 90;
  ST1.write(power1);
  ST2.write(power2);
}

void move_forward() {
  // put your main code here, to run repeatedly:
  //90 makes it stop, less than 90 is forward
  int power1 = 60;
  int power2 = 60;
  ST1.write(power1);
  ST2.write(power2);
}
void move_right() {
  // put your main code here, to run repeatedly:
  //90 makes it stop, less than 90 is forward
  int power2 = 50;
  ST2.write(power2);
}
void move_left() {
  // put your main code here, to run repeatedly:
  //90 makes it stop, less than 90 is forward
  int power1 = 50;
  ST1.write(power1);
}
int adjust(){
  move_right();
  delay(1000);
  stop_moving();
  move_forward();
  delay(1000);
  stop_moving();
  move_left();
  delay(1000);
  stop_moving();
  loop();
}
int DarkRoom(){
   int roomIsDark = true;
   long duration, inches, cm;
    while (roomIsDark){
      Serial.println("SENSOR 1");
      Serial.println(analogRead(pResistor1));
      Serial.println("SENSOR 2");
      Serial.println(analogRead(pResistor2));
      if (analogRead(pResistor1) <= 10 && analogRead(pResistor2) <=10){
         Serial.println("Finding Light");
         ST1.write(50);
         ST2.write(90);
         delay(1000);
       }
       //if (analogRead(pResistor1) > 90 && analogRead(pResistor2) > 90){
         //roomIsDark = false;
       //}
       if (analogRead(pResistor1) - analogRead(pResistor2) >= 5){
         while(analogRead(pResistor1) - analogRead(pResistor2) >= 5){
           Serial.println("Turn left");
           ST1.write(50);
           ST2.write(90);
           delay(1000);
           
           //ST1.write(50);
         }
       }
       if (analogRead(pResistor1) - analogRead(pResistor2) <= -5){
         while(analogRead(pResistor1) - analogRead(pResistor2) <= -5){
           Serial.println("Turn right");
           ST1.write(80);
           ST2.write(50);
           delay(1000);
         }
       }
       else{
         Serial.println("GO STRAIGHT!!!!!!");
         move_forward();
       }
    }
}



void loop() {
  // establish variables for duration of the ping, and the distance result
  // in inches and centimeters:
  long duration, inches, cm;

  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:

  ping_in_val = digitalRead(pingPin);

  move_forward();
    
  Serial.println("I DONT");
  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin, LOW);

  // The same pin is used to read the signal from the PING))): a HIGH pulse
  // whose duration is the time (in microseconds) from the sending of the ping
  // to the reception of its echo off of an object.
  pinMode(pingPin, INPUT);
  duration = pulseIn(pingPin, HIGH);

  // convert the time into a distance
  inches = microsecondsToInches(duration);
  cm = microsecondsToCentimeters(duration);

  Serial.print(inches);
  Serial.print("in, ");
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();


  if(inches<24){
    stop_moving();
    adjust();
  }
  if (analogRead(pResistor1) < 50 && analogRead(pResistor2) < 50){
    Serial.println("It's getting dark in here...");
    Serial.println("WE ARE NOW IN A DARK ROOM");
    delay(2000);
    DarkRoom();
  }

  delay(100);
}




long microsecondsToInches(long microseconds) {
  // According to Parallax's datasheet for the PING))), there are 73.746
  // microseconds per inch (i.e. sound travels at 1130 feet per second).
  // This gives the distance travelled by the ping, outbound and return,
  // so we divide by 2 to get the distance of the obstacle.
  // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
  return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds) {
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the object we
  // take half of the distance travelled.
  return microseconds / 29 / 2;
}
