/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the Uno and
  Leonardo, it is attached to digital pin 13. If you're unsure what
  pin the on-board LED is connected to on your Arduino model, check
  the documentation at http://www.arduino.cc

  This example code is in the public domain.

  modified 8 May 2014
  by Scott Fitzgerald
 */

int f = 10;
String inString = "";    // string to hold input

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin 13 as an output.
  pinMode(13, OUTPUT);
  Serial.begin(9600);
  Serial.println("Arduino Square wave Generator for 1 - 500Hz by Pieter Goos");
  Serial.println();
  Serial.print("Frequency: ");
  Serial.print(f);
  Serial.println(" Hz");
}

// the loop function runs over and over again forever
void loop() {
  while (Serial.available() > 0) {
    int inChar = Serial.read();
    if (isDigit(inChar)) {
      // convert the incoming byte to a char and add it to the string:
      inString += (char)inChar;
    }
    // if you get a newline, print the string, then the string's value:
    if (inChar == '\n') {

      f = inString.toInt();
      Serial.print("New frequency: ");
      Serial.print(f);
      Serial.println(" Hz");
      
      // clear the string for new input:
      inString = "";

      
    }
  }
  
  digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(500 / f);              // wait for a second
  digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
  delay(500/f);              // wait for a second
  }
