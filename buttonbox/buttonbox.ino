#include <Joystick.h>

Joystick_ Joystick(0x06, JOYSTICK_TYPE_JOYSTICK, 54, 0, false, false, false, false, false, false, false, false, false, false, false);

struct input
{
  int state;
  int previousState;
};

struct encoder
{
  int clkPin;
  int dtPin;
  int previousClkState;
  int state;
  int previousState;
};

encoder enc2 = {10, 16, 0,0,0};
encoder enc1 = {14, 15, 0,0,0};

int RXLED = 17;
const int numInputs = 18;
input inputStates[numInputs];

void setup() {
  // put your setup code here, to run once:
  
  // TX LED is set as an output behind the scenes

  Serial.begin(9600); //This pipes to the serial monitor
  Serial.println("Initialize Serial Monitor");

  // row 1, 2, 3
  pinMode(0, OUTPUT);
  pinMode(1, OUTPUT);
  pinMode(2, OUTPUT);

  for(int i = 3; i<20; i=i+1)
    pinMode(i, INPUT);
  pinMode(20, INPUT_PULLUP);
  pinMode(21, INPUT_PULLUP);
  for(int i = 0; i<numInputs; i=i+1)
    inputStates[i] = {0,0};
    
  digitalWrite(0, HIGH);
  digitalWrite(1, LOW);
  digitalWrite(2, HIGH); // encoder power
  //pinMode(RXLED, OUTPUT);  // Set RX LED as an output
  Joystick.begin();
}

void loop() 
{
  static int modeSwitch = 0;
  bool usedelay = false;
  for(int r = 0; r<2 ; r=r+1)
  {
    SetRow(r);
    for(int c = 0; c<6 ; c=c+1)
    {
      inputStates[r*6+c].state = digitalRead(c+3);
    }
  }

  //encoder buttons
  inputStates[12].state = !digitalRead(20);
  inputStates[13].state = !digitalRead(21);
  if((inputStates[12].state == 0 && inputStates[12].previousState == 1) ||
      (inputStates[13].state == 0 && inputStates[13].previousState == 1))
    usedelay = true;
      
  int sw = digitalRead(18) + (digitalRead(19)<<1);
  if(sw != modeSwitch) 
  { 
    Serial.print("sw"); Serial.println(sw);
    modeSwitch = sw;
  }



  int e1State = getEncoderState(enc1);
  switch(e1State) 
  {
    case 1:
      inputStates[14].state = 1;
      usedelay = true;
      break;

    case -1:
      inputStates[15].state = 1;
      usedelay = true;
      break;

    default:
      inputStates[14].state = 0;
      inputStates[15].state = 0;
      break;
  }

  int e2State = getEncoderState(enc2);
  switch(e2State) 
  {
    case 1:
      inputStates[16].state = 1;
      usedelay = true;
      break;

    case -1:
    usedelay = true;
      inputStates[17].state = 1;
      break;

    default:
      inputStates[16].state = 0;
      inputStates[17].state = 0;
      break;
  }

/*  for(int i = 0; i<numInputs ; i=i+1)
  {
    if(inputStates[i].previousState != inputStates[i].state)
    {
      Serial.print("Input.");
      Serial.print(i+1);
      if(inputStates[i].state>0)
        Serial.println(" ON.");
      else
        Serial.println(" OFF.");
    }
  }
  */
  // set joystick buttons:
  for(int c = 0; c<numInputs ; c=c+1)
  {
    if(inputStates[c].previousState != inputStates[c].state)
    {
      Joystick.setButton(modeSwitch*numInputs + c, inputStates[c].state);
      Serial.print("Joystick button.");
      Serial.print(modeSwitch*numInputs + c+1);
      if(inputStates[c].state>0)
        Serial.println(" PRESSED.");
      else
        Serial.println(" RELEASED.");
    }
  }

  // reset states
  for(int c = 0; c<numInputs ; c=c+1)
    inputStates[c].previousState = inputStates[c].state;
  enc1.previousState = enc1.state;
  enc2.previousState = enc2.state;
  inputStates[12].previousState = inputStates[12].state;
  inputStates[13].previousState = inputStates[13].state;

  if(usedelay) delay(50);
}

void SetRow(int row)
{
  switch (row)
  {
    case 0:
      digitalWrite(0, LOW);
      digitalWrite(1, HIGH);
      break;
    case 1:
      digitalWrite(0, HIGH);
      digitalWrite(1, LOW);
      break;
    default:
      break;
  }
}
int getEncoderState (encoder &enc)
{
  int clk = digitalRead(enc.clkPin);
  int ret = enc.state = 0;
  if( clk != enc.previousClkState)
  {
    int dt = digitalRead(enc.dtPin);
    if( dt == clk)
    {
      enc.state = ret = 1;
    }
    else
    {
      enc.state = ret = -1;
    }        
  }

  enc.previousClkState = clk;

  return ret;
}
