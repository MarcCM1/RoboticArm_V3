#include <Servo.h>

const int servPinzaPin = 5;
const int servFalAltaPin = 6;
const int servFalMediaPin = 9;
const int servFalBajaPin = 10;
const int servBasePin = 11;

const int joystick1X = A4;
const int joystick1Y = A5;
const int joystick2X = A6;
const int joystick2Y = A7;
const int joystick1B = A2;
const int joystick2B = A3;

const int but1 = A1;
const int but2 = A0;

const int stateLedB = 7;
const int stateLedG = 8;
const int stateLedR = 12;

const int conmut1 = 4;
const int conmut2 = 2;

const int sensor = 13;

Servo servPinza;
Servo servFalAlta;
Servo servFalMedia;
Servo servFalBaja;
Servo servBase;

const int defaultPos = 90;
const int servMinPos = 10;
const int servMaxPos = 170;

int servPinzaPos = 90;
int servFalAltaPos = 90;
int servFalMediaPos = 90;
int servFalBajaPos = 90;
int servBasePos = 90;

bool moving = false;
bool wait = false;
int mode = 1;

const int maxRecordings = 10;

int servPinzaRecordings[maxRecordings];
int servFalAltaRecordings[maxRecordings];
int servFalMediaRecordings[maxRecordings];
int servFalBajaRecordings[maxRecordings];
int servBaseRecordings[maxRecordings];

int nRecordings = -1;

void setup() {
  Serial.begin(9600);
  Serial.println("Programa listo!");

  servPinza.attach(servPinzaPin);
  servFalAlta.attach(servFalAltaPin);
  servFalMedia.attach(servFalMediaPin);
  servFalBaja.attach(servFalBajaPin);
  servBase.attach(servBasePin);

  pinMode(joystick1X, INPUT);
  pinMode(joystick1Y, INPUT);
  pinMode(joystick2X, INPUT);
  pinMode(joystick2Y, INPUT);
  pinMode(joystick1B, INPUT_PULLUP);
  pinMode(joystick2B, INPUT_PULLUP);

  pinMode(but1, INPUT);
  pinMode(but2, INPUT);

  pinMode(stateLedB, OUTPUT);
  pinMode(stateLedG, OUTPUT);
  pinMode(stateLedR, OUTPUT);

  pinMode(conmut1, INPUT);
  pinMode(conmut2, INPUT);

  pinMode(sensor, INPUT);

  servPinza.write(defaultPos);
  servFalAlta.write(defaultPos);
  servFalMedia.write(defaultPos);
  servFalBaja.write(defaultPos);
  servBase.write(defaultPos);

  digitalWrite(stateLedB, LOW);
  digitalWrite(stateLedG, LOW);
  digitalWrite(stateLedR, LOW);
}

void setLedColor(int R, int G, int B){
  digitalWrite(stateLedB, B);
  digitalWrite(stateLedG, G);
  digitalWrite(stateLedR, R);
}

void changeMode(){
  if((digitalRead(conmut1) == 1) && (digitalRead(conmut2) == 0)){
    mode = 1;
  }
  else if((digitalRead(conmut1) == 0) && (digitalRead(conmut2) == 0)){
    mode = 2;
  }
  else if((digitalRead(conmut1) == 0) && (digitalRead(conmut2) == 1)){
    mode = 3;
  }
}

void resetPosition(){
  while((servFalBajaPos != defaultPos) || (servFalMediaPos != defaultPos) || (servFalAltaPos != defaultPos) || (servPinzaPos != defaultPos) || (servBasePos != defaultPos)){
    //Servo falange baja
    if(servFalBajaPos != defaultPos){
      if(servFalBajaPos < defaultPos){
        servFalBajaPos++;
      }
      else{
        servFalBajaPos--;
      }

      servFalBaja.write(servFalBajaPos);
    }

    //Servo falange media
    if(servFalMediaPos != defaultPos){
      if(servFalMediaPos < defaultPos){
        servFalMediaPos++;
      }
      else{
        servFalMediaPos--;
      }
      
      servFalMedia.write(servFalMediaPos);
    }

    //Servo falange alta
    if(servFalAltaPos != defaultPos){
      if(servFalAltaPos < defaultPos){
        servFalAltaPos++;
      }
      else{
        servFalAltaPos--;
      }
      
      servFalAlta.write(servFalAltaPos);
    }

    //Servo pinza
    if(servPinzaPos != defaultPos){
      if(servPinzaPos < defaultPos){
        servPinzaPos++;
      }
      else{
        servPinzaPos--;
      }
      
      servPinza.write(servPinzaPos);
    }

    //Servo base
    if(servBasePos != defaultPos){
      if(servBasePos < defaultPos){
        servBasePos++;
      }
      else{
        servBasePos--;
      }
      
      servBase.write(servBasePos);
    }

    delay(10);
  }
}

void manualControl(){
  //Reset
  if((digitalRead(joystick1B) == 0) && (digitalRead(joystick2B) == 0)){
      resetPosition();
  }
  //Movement
  else{
    //Servo base
    if((analogRead(joystick1Y) >= 923) && (servBasePos < servMaxPos)){
      servBasePos++;
      delay(15);
    }
    else if((analogRead(joystick1Y) <= 100) && (servBasePos > servMinPos)){
      servBasePos--;
      delay(15);
    }

    //Servo falange baja
    if((analogRead(joystick1X) >= 923) && (servFalBajaPos < servMaxPos)){
      servFalBajaPos++;
      delay(15);
    }
    else if((analogRead(joystick1X) <= 100) && (servFalBajaPos > servMinPos)){
      servFalBajaPos--;
      delay(15);
    }
                
    //Servo falange media
    if((analogRead(joystick2Y) <= 100) && (servFalMediaPos < servMaxPos)){
      servFalMediaPos++;
      delay(15);
    }
    else if((analogRead(joystick2Y) >= 923) && (servFalMediaPos > servMinPos)){
      servFalMediaPos--;
      delay(15);
    }

    //Servo falange alta
    if((analogRead(joystick2X) <= 100) && (servFalAltaPos < servMaxPos)){
      servFalAltaPos++;
      delay(15);
    }
    else if((analogRead(joystick2X) >= 923) && (servFalAltaPos > servMinPos)){
      servFalAltaPos--;
      delay(15);
    }

    //Servo pinza
    if((digitalRead(joystick2B) == 0) && (servPinzaPos < servMaxPos)){
      servPinzaPos++;
      delay(20);
    }
    else if((digitalRead(joystick1B) == 0) && (servPinzaPos > servMinPos)){
      servPinzaPos--;
      delay(20);
    }

    servPinza.write(servPinzaPos);
    servFalAlta.write(servFalAltaPos);
    servFalMedia.write(servFalMediaPos);
    servFalBaja.write(servFalBajaPos);
    servBase.write(servBasePos);
  }
}

void servMoveTo(int SB, int SFB, int SFM, int SFA, int SPz){
  while((SFA != servFalAltaPos) || (SFM != servFalMediaPos) || (SFB != servFalBajaPos) || (SB != servBasePos)){
    //Servo falange alta
    if(SFA != servFalAltaPos){
      if(SFA > servFalAltaPos){
        servFalAltaPos++;
      }
      else{
        servFalAltaPos--;
      }
    }

    //Servo falange media
    if(SFM != servFalMediaPos){
      if(SFM > servFalMediaPos){
        servFalMediaPos++;
      }
      else{
        servFalMediaPos--;
      }
    }

    //Servo falange baja
    if(SFB != servFalBajaPos){
      if(SFB > servFalBajaPos){
        servFalBajaPos++;
      }
      else{
        servFalBajaPos--;
      }
    }

    //Servo base
    if(SB != servBasePos){
      if(SB > servBasePos){
        servBasePos++;
      }
      else{
        servBasePos--;
      }
    }
    
    servFalAlta.write(servFalAltaPos);
    servFalMedia.write(servFalMediaPos);
    servFalBaja.write(servFalBajaPos);
    servBase.write(servBasePos);
    delay(10);
  }
  delay(100);
  
  while(SPz != servPinzaPos){
    if(SPz > servPinzaPos){
      servPinzaPos++;
    }
    else{
      servPinzaPos--;
    }

    servPinza.write(servPinzaPos);
    delay(5);
  }

  delay(50);
}

void recordPosition(){
  nRecordings++;

  servPinzaRecordings[nRecordings] = servPinzaPos;
  servFalAltaRecordings[nRecordings] = servFalAltaPos;
  servFalMediaRecordings[nRecordings] = servFalMediaPos;
  servFalBajaRecordings[nRecordings] = servFalBajaPos;
  servBaseRecordings[nRecordings] = servBasePos;

  if(mode == 2){
    setLedColor(HIGH, HIGH, LOW);
    delay(250);
    setLedColor(HIGH, LOW, LOW);
    delay(100);
  }
  else if(mode == 3){
    setLedColor(LOW, HIGH, HIGH);
    delay(250);
    setLedColor(HIGH, LOW, HIGH);
    delay(100);
  }
}

void loop(){
  changeMode();

  //MANUAL
  if(mode == 1){
    setLedColor(LOW, HIGH, LOW);
    resetPosition();

    while(mode == 1){
      changeMode();
      if(mode != 1){
        break;
      }

      manualControl();
    }
  }
  //RECORD-MOVE
  else if(mode == 2){
    setLedColor(HIGH, LOW, LOW);
    moving = false;
    resetPosition();

    while(mode == 2){
      //Record
      if(moving == false){
        setLedColor(HIGH, LOW, LOW);
        resetPosition();
        nRecordings = -1;

        while(digitalRead(but2) == 0){
          changeMode();
          if(mode != 2){
            break;
          }

          manualControl();

          if(digitalRead(but1) == 1){
            recordPosition();

            while(digitalRead(but1) == 1){
              setLedColor(HIGH, HIGH, HIGH);
            }

            setLedColor(HIGH, LOW, LOW);
          }

          if(nRecordings == (maxRecordings - 1)){
            break;
          }
        }

        if(mode == 2){
          moving = true;

          delay(250);

          while(digitalRead(but2) == 0){
            setLedColor(HIGH, HIGH, HIGH);
          }
        }
        else{
          moving = false;
        }
      }
      //Move
      else{
        setLedColor(HIGH, HIGH, LOW);

        while(digitalRead(but2) == 0){
          for(int fase = 0; fase <= nRecordings; fase++){
            changeMode();
            if(mode != 2){
              break;
            }

            servMoveTo(servBaseRecordings[fase], servFalBajaRecordings[fase], servFalMediaRecordings[fase], servFalAltaRecordings[fase], servPinzaRecordings[fase]);
            
            setLedColor(HIGH, LOW, LOW);
            delay(50);
            setLedColor(HIGH, HIGH, LOW);
          }

          if(mode != 2){
            break;
          }
        }

        while(digitalRead(but2) == 1){
          setLedColor(HIGH, HIGH, HIGH);
        }

        moving = false;
      }
    }
  }
  //RECORD-WAIT-MOVE
  else if(mode == 3){
    setLedColor(HIGH, LOW, HIGH);
    moving = false;
    wait = false;
    resetPosition();

    while(mode == 3){
      //Record
      if((moving == false) && (wait == false)){
        setLedColor(HIGH, LOW, HIGH);
        resetPosition();
        nRecordings = -1;

        while(digitalRead(but2) == 0){
          changeMode();
          if(mode != 3){
            break;
          }

          manualControl();

          if(digitalRead(but1) == 1){
            recordPosition();

            while(digitalRead(but1) == 1){
              setLedColor(HIGH, HIGH, HIGH);
            }

            setLedColor(HIGH, LOW, HIGH);
          }

          if(nRecordings == (maxRecordings - 1)){
            break;
          }
        }

        if(mode == 3){
          wait = true;

          delay(250);

          while(digitalRead(but2) == 1){
            setLedColor(HIGH, HIGH, HIGH);
          }
        }
        else{
          moving = false;
          wait = false;
        }
      }
      //Wait
      else if((moving == false) && (wait == true)){
        resetPosition();

        while(digitalRead(sensor) == 1){
          setLedColor(LOW, HIGH, HIGH);

          if(digitalRead(but2) == 1){
            break;
          }

          changeMode();
          if(mode != 3){
            break;
          }
        }

        if(digitalRead(but2) == 1){
          moving = false;
          wait = false;

          delay(250);
          while(digitalRead(but2) == 1){
            setLedColor(HIGH, HIGH, HIGH);
          }
        }
        else if(mode == 3){
          moving = true;
          wait = false;
        }
        else{
          moving = false;
          wait = false;
        }
      }
      //Move
      else if((moving == true) && (wait == false)){
        setLedColor(LOW, LOW, HIGH);

        delay(250);

        for(int fase = 0; fase <= nRecordings; fase++){
          changeMode();
          if(mode != 3){
            break;
          }

          servMoveTo(servBaseRecordings[fase], servFalBajaRecordings[fase], servFalMediaRecordings[fase], servFalAltaRecordings[fase], servPinzaRecordings[fase]);
        }

        if(mode == 3){
          moving = false;
          wait = true;
          resetPosition();
        }
        else{
          moving = false;
          wait = false;
        }
      }
    }
  }
  
  //Serial.print("SB: "); Serial.print(servBasePos); Serial.print("| SFB: "); Serial.print(servFalBajaPos); Serial.print("| SFM: "); Serial.print(servFalMediaPos); Serial.print("| SFA: "); Serial.print(servFalAltaPos); Serial.print("| SG: "); Serial.println(servPinzaPos);
}
