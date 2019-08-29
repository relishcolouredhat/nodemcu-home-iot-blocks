int r_in  = 4;  
int r_out = 0;
int y_in  = 5;
int y_out = 2;
int g_in  = 16;
int g_out = 14;
int runCounter = 0;

//int inputPins = {r_in,y_in,g_in}
//int outputPins = {r_out,y_out,g_out}

int buttonPins[][2] = {{r_in,r_out},{y_in,y_out},{g_in,g_out}};

//example code
//int ledpin = 2; // D4(gpio2)
//int button = 16; //D0(gpio4)
//int buttonState=0;
int r_state =0;
int y_state =0;
int g_state =0;

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  int buttonCount = sizeof(buttonPins) / sizeof(buttonPins[0]);
  for (byte i = 0; i < buttonCount; i++){
    pinMode(buttonPins[i][0], INPUT);
    pinMode(buttonPins[i][1], OUTPUT);
    digitalWrite(buttonPins[i][1], LOW);
  }
}

void allLed(uint8_t instr){
  int buttonCount = sizeof(buttonPins) / sizeof(buttonPins[0]);
  for (byte i = 0; i < buttonCount; i++){
    digitalWrite(buttonPins[i][1], instr);
    delay(6);
    }  
}


void blinkLed(int ledPin,int t_on,int t_off, int cycles){
  for (byte i =0; i < cycles; i++){
    digitalWrite(ledPin, HIGH);
    delay(t_on);
    digitalWrite(ledPin, LOW);
    delay(t_off);
  } 
}

void notifyLed(int ledPin,int switchPin){
  blinkLed(ledPin,50,25,3);
  switch (digitalRead(switchPin)){
    case 1:
      //blinkLed(g_out,50,25,3);
      allLed(HIGH);
      delay(200);
      allLed(LOW);
      break;
    default:
      delay(50);
      notifyLed(ledPin,switchPin);
  }
}

void notifyLedCustom(int ledPin,int switchPin,int a,int b, int c){
  blinkLed(ledPin,a,b,c);
  switch (digitalRead(switchPin)){
    case 1:
      //blinkLed(g_out,50,25,3);
      allLed(HIGH);
      delay(200);
      allLed(LOW);
      break;
    default:
      delay(50);
      notifyLed(ledPin,switchPin);
  }
}

void gameBlink(){
    allLed(HIGH);
    delay(200);
    allLed(LOW);
    delay(150);
    allLed(HIGH);
    delay(200);
    allLed(LOW);
    delay(1000);

}
int notifyGameLoop(int roundCounter, int randomInt){
  r_state=digitalRead(r_in);
  y_state=digitalRead(y_in);
  g_state=digitalRead(g_in);
  roundCounter++;
  int randomButton = random(0,3);
  //int newRandomInt = random(-1,1)*(randomInt+(randomButton*roundCounter));
  //notifyLedCustom(buttonPins[randomButton][1],buttonPins[randomButton][0],50+newRandomInt,25+randomInt,5);
  notifyLed(buttonPins[randomButton][1],buttonPins[randomButton][0]);
  if (r_state == 1 && y_state == 1){
    return 0;
  }
  notifyGameLoop(roundCounter,newRandomInt);
}

void notifyGame(){
  gameBlink();
  notifyGameLoop(1,0);
  gameBlink();
}

void loop() {
  r_state=digitalRead(r_in);
  y_state=digitalRead(y_in);
  g_state=digitalRead(g_in);

 if (runCounter == 0){
    Serial.println("First run; testing notifications...");
    runCounter++;
    notifyLed(r_out,r_in);
    notifyLed(y_out,y_in);
    notifyLed(g_out,g_in);
 }
  if (r_state == 1 && g_state == 1)
  {
    Serial.println("Two button is pressed!!");
    //digitalWrite(r_out, HIGH); 
    //delay(200);
    allLed(HIGH);
    delay(2000);
  }

   if (r_state == 1 && y_state == 1)
  {
  notifyGame();
  }
 
 if (r_state == 1)
  {
    Serial.println("Red button is pressed!!");
    //digitalWrite(r_out, HIGH); 
    //delay(200);
    blinkLed(r_out,50,25,3);
  }
 if (r_state == 0)
   {
    digitalWrite(r_out, LOW); 
    delay(200);
    }
 if (y_state == 1)
  {
    Serial.println("Yellow button is pressed!!");
    digitalWrite(y_out, HIGH); 
    delay(200);
  }
 if (y_state == 0)
   {
    digitalWrite(y_out, LOW); 
    delay(200);
    }
 if (g_state == 1)
  {
    Serial.println("Green button is pressed!!");
    digitalWrite(g_out, HIGH); 
    delay(200);
  }
 if (g_state == 0)
   {
    digitalWrite(g_out, LOW); 
    delay(200);
    }
}

