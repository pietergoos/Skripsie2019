int f = 10;
int b = 0;
String inString = "";
int cnt = 0;
int mode = 0;
int pinStates[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
boolean bm = true;
boolean bs = false;
boolean i = false;
int binout[] = {0,0,0};
int bytout[] = {0, 0, 0, 0, 0, 0, 0, 0};

void setup() {
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  Serial.begin(9600);
  Serial.println("Arduino test device by Pieter Goos");
  Serial.println("To output square wave on pin 13 use command f N (1 <= N <= 500)");
  Serial.println("To output 3 pin binary values on pins 10-12 use command b N (0 <= N <= 7)");
  Serial.println("To output increment the values on pins 10-12 use command i");
  Serial.println("To shift the 3 pin value as a shift register use command s");
  Serial.println("To shift 8 bits (pins 5 - 12) use S");
  Serial.println("Note: The frequency controls the speed of all functions!");
  Serial.println("Enter Commands below:");
}

void loop() {
  while (Serial.available() > 0) {
    int inChar = Serial.read();

    if (cnt == 0){
      mode = inChar;
    } else {
      if (isDigit(inChar)) {
      // convert the incoming byte to a char and add it to the string:
      inString += (char)inChar;
      }
    }
    cnt++;
    // if you get a newline, print the string, then the string's value:
    if (inChar == '\n') {
      switch(mode){
        case 'f': //Frequency
          if( inString.toInt() >= 1 && inString.toInt() <= 500){
            f = inString.toInt();
            Serial.print("New Square Wave Freq.: ");
            Serial.print(f);
            Serial.println(" Hz");
          } else {
            Serial.println("Value must be between 1-500");
          }
          break;
        case 'b': //BCD
          bm = true;
          bs = false;
          if( inString.toInt() >= 0 && inString.toInt() <= 7){
            b = inString.toInt();
            Serial.print("New 3bit number: ");
            Serial.println(b);
          } else {
            Serial.println("Value must be between 0-7");
          }
          break;
        case 's': //Shift register
          if( bm == true){
            bm = false;
          }else{
            bm = true;
          }
          break;
        case 'S': //Large shift Reg
          bm = false;
          if( bs == true){
            bs = false;/*
            digitalWrite(5, 0);
            digitalWrite(6, 0);
            digitalWrite(7, 0);
            digitalWrite(8, 0);
            digitalWrite(9, 0);
            */
          }else{
            bs = true;
          }
          break;
        case 'i': //increment BCD
          bm = true;
          bs = false;
          if( i == true){
            i = false;
          }else{
            i = true;
          }
          break;
        default:
          Serial.println("Invalid command");
          break;
      }     
      // clear the string for new input:
      inString = "";
      cnt = 0;
    }
  }

    if (bs == false){
      digitalWrite(5, 0);
      digitalWrite(6, 0);
      digitalWrite(7, 0);
      digitalWrite(8, 0);
      digitalWrite(9, 0);
    }
  
    //BCD
    if(bm==true){
      BCD(b);
    } else { 
      if (bs == false){
      shiftBCD();
      } else {
        shiftByCD();
      }
    }
    
    //Square wave
    alternatePin(13);
    delay(500 / f);
    alternatePin(13);
    delay(500 / f);

    //Increment BCD val
    if (i == true && bm == true){
      b++;
      if (b == 8){
        b = 0;
      }
    }

    
}

void alternatePin(int pinNo){
    if (pinStates[pinNo] == 0){
      digitalWrite(pinNo, HIGH);
      pinStates[pinNo] = 1;
    }else{
      digitalWrite(pinNo, LOW);
      pinStates[pinNo] = 0;
    }
}

void BCD(int val){
  switch(val){
    case 0:
      binout[0] = 0;
      binout[1] = 0;
      binout[2] = 0;
      break;
    case 1:
      binout[0] = 0;
      binout[1] = 0;
      binout[2] = 1;
      break;
    case 2:
      binout[0] = 0;
      binout[1] = 1;
      binout[2] = 0;
      break;
    case 3:
      binout[0] = 0;
      binout[1] = 1;
      binout[2] = 1;
      break;
    case 4:
      binout[0] = 1;
      binout[1] = 0;
      binout[2] = 0;
      break;
    case 5:
      binout[0] = 1;
      binout[1] = 0;
      binout[2] = 1;
      break;
    case 6:
      binout[0] = 1;
      binout[1] = 1;
      binout[2] = 0;
      break;
    case 7:
      binout[0] = 1;
      binout[1] = 1;
      binout[2] = 1;
      break;
  }
  digitalWrite(10, binout[0]);
  digitalWrite(11, binout[1]);
  digitalWrite(12, binout[2]);
  bytout[0] = binout[0];
  bytout[1] = binout[1];
  bytout[2] = binout[2];
  bytout[3] = 0;
  bytout[4] = 0;
  bytout[5] = 0;
  bytout[6] = 0;
  bytout[7] = 0;
}

void shiftBCD(void){
  int temp = binout[2];
  binout[2] = binout[1];
  binout[1] = binout[0];
  binout[0] = temp;

  
  digitalWrite(10, binout[0]);
  digitalWrite(11, binout[1]);
  digitalWrite(12, binout[2]);
  bytout[0] = binout[0];
  bytout[1] = binout[1];
  bytout[2] = binout[2];
  bytout[3] = 0;
  bytout[4] = 0;
  bytout[5] = 0;
  bytout[6] = 0;
  bytout[7] = 0;
}

void shiftByCD(void){
  int temp = bytout[7];

  for(int j = 7; j > 0; j--){
    bytout[j] = bytout[j-1];
  }
  bytout[0] = temp;

  for(int k = 0; k < 8; k++){
    digitalWrite(k+5, bytout[k]);
  }
}

