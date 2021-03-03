
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
const int numInputs = 14;
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

  for(int i = 3; i<21; i=i+1)
    pinMode(i, INPUT);
  for(int i = 0; i<18; i=i+1)
    inputStates[i] = {0,0};
    
  digitalWrite(0, HIGH);
  digitalWrite(1, LOW);
  digitalWrite(2, HIGH); // encoder power
  //pinMode(RXLED, OUTPUT);  // Set RX LED as an output
}

void loop() 
{
  static int modeSwitch = 0;
  for(int r = 0; r<2 ; r=r+1)
  {
    SetRow(r);
    for(int c = 0; c<6 ; c=c+1)
    {
      inputStates[r*6+c].state = digitalRead(c+3);
    }
  }

  inputStates[12].state = digitalRead(20);
  inputStates[13].state = digitalRead(21);

  int sw = digitalRead(18) + (digitalRead(19)<<1);
  if(sw != modeSwitch) 
  { 
    Serial.print("sw"); Serial.println(sw);
    modeSwitch = sw;
  }

  for(int i = 0; i<numInputs-2 ; i=i+1)
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

  int e1State = getEncoderState(enc1);
  if(e1State!=0) 
  {
    Serial.print("ENC1: "); Serial.println(e1State);
  }

  int e2State = getEncoderState(enc2);
  if(e2State!=0) 
  {
    Serial.print("ENC2: "); Serial.println(e2State);
  }

  for(int c = 0; c<numInputs ; c=c+1)
    inputStates[c].previousState = inputStates[c].state;
  enc1.previousState = enc1.state;
  enc2.previousState = enc2.state;
  inputStates[12].previousState = inputStates[12].state;
  inputStates[13].previousState = inputStates[13].state;
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
